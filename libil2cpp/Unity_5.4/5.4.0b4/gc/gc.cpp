#include "il2cpp-config.h"
#include "object-internals.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/Runtime.h"
#include "vm/Thread.h"
#include "os/Event.h"
#include "os/Semaphore.h"
#include "os/Thread.h"
#include <cassert>

// gc-internal.h includes gc-wrapper.h, which includes gc.h, which includes windows.h, 
// and that conflicts with vm/Thread.h as it defines MemoryBarrier macro. So include this last.
#include "gc-internal.h"

using namespace il2cpp::os;
using namespace il2cpp::vm;

#if IL2CPP_SUPPORT_THREADS

static bool s_StopFinalizer = false;
static il2cpp::os::Thread s_FinalizerThread;
static Il2CppThread* s_FinalizerThreadObject;
static Semaphore s_FinalizerSemaphore;
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

		il2cpp_gc_invoke_finalizers ();

		s_FinalizersCompletedEvent.Set ();
	}

	il2cpp::vm::Thread::Detach (s_FinalizerThreadObject);
}

bool il2cpp_gc_is_gc_thread (Il2CppThread *thread)
{
	return s_FinalizerThreadObject == thread;
}

#else

bool il2cpp_gc_is_gc_thread (Il2CppThread *thread)
{
	return false;
}

#endif

void
il2cpp_gc_init (void)
{
	il2cpp_gc_base_init();
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerThread.Run (&FinalizerThread, NULL);
	s_FinalizersThreadStartedEvent.Wait();
#endif
}


void
il2cpp_gc_cleanup (void)
{
#if IL2CPP_SUPPORT_THREADS
	s_StopFinalizer = true;
	il2cpp_gc_finalize_notify ();
	s_FinalizerThread.Join ();
#endif
}

void il2cpp_gc_finalize_notify ()
{
#if IL2CPP_SUPPORT_THREADS
	s_FinalizerSemaphore.Post (1, NULL);
#endif
}

void
il2cpp_gc_run_finalize (void *obj, void *data)
{
	NOT_IMPLEMENTED_NO_ASSERT (il2cpp_gc_run_finalize, "Compare to mono implementation special cases");

	Il2CppObject *exc = NULL;
	Il2CppObject *o;
	const MethodInfo* finalizer = NULL;

	o = (Il2CppObject*)obj;

	finalizer = Class::GetFinalizer (o->klass);

	Runtime::Invoke (finalizer, o, NULL, &exc);

	if (exc) {
		/* fixme: do something useful */
	}
}

static void register_finalizer (Il2CppObject* obj, void (*callback)(void *, void*))
{
	NOT_IMPLEMENTED_NO_ASSERT (register_finalizer, "Compare to mono implementation special cases");

	if (obj == NULL)
		Exception::Raise (Exception::GetArgumentNullException ("obj"));

#if !IL2CPP_GC_NULL
	GC_REGISTER_FINALIZER_NO_ORDER ((char*)obj, callback, NULL, NULL, NULL);
#endif
}

void il2cpp_gc_register_finalizer (Il2CppObject* obj)
{
	register_finalizer (obj, &il2cpp_gc_run_finalize);
}

void il2cpp_gc_suppress_finalizer (Il2CppObject* obj)
{
	register_finalizer (obj, NULL);
}

void il2cpp_gc_wait_for_pending_finalizers ()
{
	if (!il2cpp_gc_pending_finalizers ())
		return;
	
#if IL2CPP_SUPPORT_THREADS
	/* Avoid deadlocks */
	if (il2cpp::vm::Thread::Current () == s_FinalizerThreadObject)
		return;

	s_FinalizersCompletedEvent.Reset ();
	il2cpp_gc_finalize_notify ();
	s_FinalizersCompletedEvent.Wait ();
#else
	il2cpp_gc_invoke_finalizers ();
#endif
}

bool
il2cpp_gc_has_finalizer_thread_object()
{
#if IL2CPP_SUPPORT_THREADS
	return s_FinalizerThreadObject != NULL;
#endif
	return false;
}
