#include "il2cpp-config.h"
#include "object-internals.h"
#include "GarbageCollector.h"
#include "os/Event.h"
#include "os/Mutex.h"
#include "os/Semaphore.h"
#include "os/Thread.h"
#include "utils/Il2CppHashMap.h"
#include "utils/HashUtils.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/RCW.h"
#include "vm/Runtime.h"
#include "vm/Thread.h"

using namespace il2cpp::os;
using namespace il2cpp::vm;

namespace il2cpp
{

namespace gc
{

// So COM Callable Wrapper can be created for any kind of managed object,
// whether it has finalizer or not. If it doesn't then it's an easy case:
// when creating the CCW, we just register our cleanup method to be the
// finalizer method. In case it does, then we need to be able to invoke
// both our CCW cleanup method, and the finalizer in question.
// We could chain them by registering CCW cleanup method over the finalizer
// method and storing the previous finalizer in CCW cache, but it would 
// screw us over if for example the object is IDisposable and then it 
// calls `GC.SuppressFinalize` from C# - instead of the finalizer getting
// unregistered, our CCW cleanup method gets unregistered, and we now have
// a dangling pointer to the managed heap in the CCW cache. Not pretty.
// Instead, we made GarbageCollector::RegisterFinalizer and GarbageCollector::SuppressFinalizer
// to be CCW cache aware. Now, any managed object goes into 1 of these categories:
//
// 1. Object has no finalizer and it is not in CCW cache. It gets garbage
//    collected and no cleanup is needed.
// 2. Object has a finalizer and it is not in CCW cache. GarbageCollector::RunFinalizer
//    gets registered with the GC for such object.
// 3. Object has no finalizer and it is in CCW cache. CleanupCCW is
//    registered with the GC for such object. Once it is called, it removes
//    the object from the CCW cache.
// 4. Object has a finalizer and it is in CCW cache. CleanupCCW is
//    registered with the GC for such object. Once it is called, it removes
//    the object from the CCW cache and then calls its finalizer.
//
// To know whether we have case 3 or 4, we have the "hasFinalizer" field in
// the CachedCCW object.
//
// When GarbageCollector::RegisterFinalizer or GarbageCollector::SuppressFinalizer
// is called, we have managed objects fitting in two buckets:
//
// 1. Those that do not exist in CCW cache. Finalizer is normally registered with
//    the GC.
// 2. Those that are in the CCW cache. In such case, GC won't know about the call,
//    but instead we'll find the object in the CCW cache and RegisterFinalizer will
//    set "hasFinalizer" field to true, while SuppressFinalizer will set it to false
//
// Any methods that interact with s_CCWCache have to lock s_CCWCacheMutex. 
struct CachedCCW
{
	Il2CppIManagedObjectHolder* managedObjectHolder;
	bool hasFinalizer;
};

typedef Il2CppHashMap<Il2CppObject*, CachedCCW, utils::PointerHash<Il2CppObject> > CCWCache;

static FastMutex s_CCWCacheMutex;
static CCWCache s_CCWCache;

#if IL2CPP_SUPPORT_THREADS

static bool s_StopFinalizer = false;
static il2cpp::os::Thread s_FinalizerThread;
static Il2CppThread* s_FinalizerThreadObject;
static Semaphore s_FinalizerSemaphore(0,32767);
static Event s_FinalizersThreadStartedEvent;
static Event s_FinalizersCompletedEvent(true, false);

static void FinalizerThread (void* arg)
{
	s_FinalizerThreadObject = il2cpp::vm::Thread::Attach (Domain::GetCurrent ());
	s_FinalizerThread.SetName ("GC Finalizer");

	s_FinalizersThreadStartedEvent.Set();

	while (!s_StopFinalizer)
	{
		s_FinalizerSemaphore.Wait ();

		GarbageCollector::InvokeFinalizers ();

		s_FinalizersCompletedEvent.Set ();
	}

	il2cpp::vm::Thread::Detach (s_FinalizerThreadObject);
}

bool GarbageCollector::IsFinalizerThread (Il2CppThread *thread)
{
	return s_FinalizerThreadObject == thread;
}

#else

bool GarbageCollector::IsFinalizerThread (Il2CppThread *thread)
{
	return false;
}

#endif

void GarbageCollector::InitializeFinalizer ()
{
	GarbageCollector::InvokeFinalizers ();
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerThread.Run (&FinalizerThread, NULL);
	s_FinalizersThreadStartedEvent.Wait();
#endif
}


void GarbageCollector::Uninitialize ()
{
#if IL2CPP_SUPPORT_THREADS
	s_StopFinalizer = true;
	NotifyFinalizers ();
	s_FinalizerThread.Join ();
#endif
}

void GarbageCollector::NotifyFinalizers ()
{
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerSemaphore.Post (1, NULL);
#endif
}

void GarbageCollector::RunFinalizer (void *obj, void *data)
{
	NOT_IMPLEMENTED_NO_ASSERT (GarbageCollector::RunFinalizer, "Compare to mono implementation special cases");

	Il2CppException *exc = NULL;
	Il2CppObject *o;
	const MethodInfo* finalizer = NULL;

	o = (Il2CppObject*)obj;

	finalizer = Class::GetFinalizer (o->klass);

	Runtime::Invoke (finalizer, o, NULL, &exc);

	if (exc)
		Runtime::UnhandledException (exc);
}

void GarbageCollector::RegisterFinalizerForNewObject(Il2CppObject* obj)
{
	// Fast path
	// No need to check CCW cache since it's guaranteed to not be in it for a new object
	RegisterFinalizerWithCallback(obj, &GarbageCollector::RunFinalizer);
}

void GarbageCollector::RegisterFinalizer(Il2CppObject* obj)
{
	// Slow path
	// Check in CCW cache first
	os::FastAutoLock lock(&s_CCWCacheMutex);
	CCWCache::iterator it = s_CCWCache.find(obj);

	if (it != s_CCWCache.end())
	{
		it->second.hasFinalizer = true;
	}
	else
	{
		RegisterFinalizerWithCallback(obj, &GarbageCollector::RunFinalizer);
	}
}

void GarbageCollector::SuppressFinalizer(Il2CppObject* obj)
{
	// Slow path
	// Check in CCW cache first
	os::FastAutoLock lock(&s_CCWCacheMutex);
	CCWCache::iterator it = s_CCWCache.find(obj);

	if (it != s_CCWCache.end())
	{
		it->second.hasFinalizer = false;
	}
	else
	{
		RegisterFinalizerWithCallback(obj, NULL);
	}
}

void GarbageCollector::WaitForPendingFinalizers ()
{
	if (!GarbageCollector::HasPendingFinalizers ())
		return;
	
#if IL2CPP_SUPPORT_THREADS
	/* Avoid deadlocks */
	if (vm::Thread::Current () == s_FinalizerThreadObject)
		return;

	s_FinalizersCompletedEvent.Reset ();
	NotifyFinalizers ();
	s_FinalizersCompletedEvent.Wait ();
#else
	GarbageCollector::InvokeFinalizers ();
#endif
}

int32_t GarbageCollector::GetGeneration (void* addr)
{
	return 0;
}

void GarbageCollector::AddMemoryPressure (int64_t value)
{
}

static void CleanupCCW(void* obj, void* data)
{
	bool hasFinalizer;

	// We have to destroy CCW before invoking the finalizer, because we cannot know whether the finalizer will revive the object
	// In cases it does revive it, it's also possible for it to hit CCW cache, and in that case we'd want to create a new CCW object
	// rather than returning the one that we're about to destroy here
	{
		os::FastAutoLock lock(&s_CCWCacheMutex);
		CCWCache::iterator it = s_CCWCache.find(static_cast<Il2CppObject*>(obj));
		IL2CPP_ASSERT(it != s_CCWCache.end());

		Il2CppIManagedObjectHolder* managedObjectHolder = it->second.managedObjectHolder;
		hasFinalizer = it->second.hasFinalizer;
		s_CCWCache.erase(it);

		managedObjectHolder->Destroy();
	}

	if (hasFinalizer)
		GarbageCollector::RunFinalizer(obj, data);
}

Il2CppIUnknown* GarbageCollector::GetOrCreateCCW(Il2CppObject* obj, const Il2CppGuid& iid)
{
	if (obj == NULL)
		return NULL;

	// check for rcw object. COM interface can be extracted from it and there's no need to create ccw
	if (obj->klass->is_import_or_windows_runtime)
		return RCW::QueryInterface<true>(static_cast<Il2CppComObject*>(obj), iid);

	os::FastAutoLock lock(&s_CCWCacheMutex);
	CCWCache::iterator it = s_CCWCache.find(obj);
	Il2CppIManagedObjectHolder* managedObjectHolder;

	if (it == s_CCWCache.end())
	{
		managedObjectHolder = CCW::CreateCCW(obj);
		CachedCCW ccw =
		{
			managedObjectHolder,
			RegisterFinalizerWithCallback(obj, &CleanupCCW) != NULL
		};

		s_CCWCache.insert(std::make_pair(obj, ccw));
	}
	else
	{
		managedObjectHolder = it->second.managedObjectHolder;
	}

	Il2CppIUnknown* result;
	il2cpp_hresult_t hr = managedObjectHolder->QueryInterface(iid, reinterpret_cast<void**>(&result));
	Exception::RaiseIfFailed(hr, true);
	return result;
}

#if NET_4_0
void il2cpp::gc::GarbageCollector::SetSkipThread(bool skip)
{

}
#endif

} // namespace gc
} // namespace il2cpp