#include "il2cpp-config.h"

#if IL2CPP_GC_BOEHM

#include <stdint.h>
#include "gc_wrapper.h"
#include "GarbageCollector.h"
#include "vm/Profiler.h"
#include <cassert>

static bool s_GCInitialized = false;

#if IL2CPP_ENABLE_PROFILER
using il2cpp::vm::Profiler;
static void on_gc_event (GCEventType eventType);
static void on_heap_resize (GC_word newSize);
#endif

void
il2cpp::gc::GarbageCollector::Initialize ()
{
	if (s_GCInitialized)
		return;
	// This tells the GC that we are not scanning dynamic library data segments and that
	// the GC tracked data structures need ot be manually pushed and marked.
	// Call this before GC_INIT since the initialization logic uses this value.
	GC_set_no_dls (1);

#if IL2CPP_ENABLE_PROFILER
	GC_set_on_event (&on_gc_event);
	GC_set_on_heap_resize (&on_heap_resize);
#endif

	GC_INIT();
#if defined(GC_THREADS)
	GC_set_finalize_on_demand (1);
	GC_set_finalizer_notifier (&il2cpp::gc::GarbageCollector::NotifyFinalizers);
	// We need to call this if we want to manually register threads, i.e. GC_register_my_thread
	GC_allow_register_threads ();
#endif
#ifdef GC_GCJ_SUPPORT
	GC_init_gcj_malloc (0, NULL);
#endif
	s_GCInitialized = true;
}

int32_t
il2cpp::gc::GarbageCollector::GetCollectionCount (int32_t generation)
{
	return (int32_t)GC_get_gc_no ();
}

int32_t
il2cpp::gc::GarbageCollector::GetMaxGeneration ()
{
	return 0;
}

void
il2cpp::gc::GarbageCollector::Collect (int maxGeneration)
{
	assert(maxGeneration == 0); //Consumers should know this won't work.
	GC_gcollect ();
}

int32_t
il2cpp::gc::GarbageCollector::CollectALittle ()
{
	return GC_collect_a_little ();
}

int64_t
il2cpp::gc::GarbageCollector::GetUsedHeapSize (void)
{
	return GC_get_heap_size () - GC_get_free_bytes ();
}

int64_t
il2cpp::gc::GarbageCollector::GetAllocatedHeapSize (void)
{
	return GC_get_heap_size ();
}

void
il2cpp::gc::GarbageCollector::Disable ()
{
	GC_disable ();
}

void
il2cpp::gc::GarbageCollector::Enable ()
{
	GC_enable ();
}

bool
il2cpp::gc::GarbageCollector::RegisterThread (void *baseptr)
{
#if defined(GC_THREADS)
	struct GC_stack_base sb;
	int res;

	res = GC_get_stack_base (&sb);
	if (res != GC_SUCCESS) {
		sb.mem_base = baseptr;
#ifdef __ia64__
		/* Can't determine the register stack bounds */
		assert (false && "mono_gc_register_thread failed ().");
#endif
	}
	res = GC_register_my_thread (&sb);
	if ((res != GC_SUCCESS) && (res != GC_DUPLICATE)) {
		assert (false && "GC_register_my_thread () failed.");
		return false;
	}
#endif
	return true;
}

bool
il2cpp::gc::GarbageCollector::UnregisterThread ()
{
#if defined(GC_THREADS)
	int res;

	res = GC_unregister_my_thread ();
	if (res != GC_SUCCESS)
		assert(false && "GC_unregister_my_thread () failed.");

	return res == GC_SUCCESS;
#else
	return true;
#endif
}

il2cpp::gc::GarbageCollector::FinalizerCallback il2cpp::gc::GarbageCollector::RegisterFinalizerWithCallback(Il2CppObject* obj, FinalizerCallback callback)
{
	FinalizerCallback oldCallback;
	void* oldData;
	GC_REGISTER_FINALIZER_NO_ORDER((char*)obj, callback, NULL, &oldCallback, &oldData);
	assert(oldData == NULL);
	return oldCallback;
}

void
il2cpp::gc::GarbageCollector::AddWeakLink (void **link_addr, Il2CppObject *obj, bool track)
{
	/* libgc requires that we use HIDE_POINTER... */
	*link_addr = (void*)GC_HIDE_POINTER (obj);
	// need this since our strings are not real objects
	if (GC_is_heap_ptr (obj))
		GC_GENERAL_REGISTER_DISAPPEARING_LINK (link_addr, obj);
}

void
il2cpp::gc::GarbageCollector::RemoveWeakLink (void **link_addr)
{
	Il2CppObject*  obj = GarbageCollector::GetWeakLink (link_addr);
	if (GC_is_heap_ptr (obj))
		GC_unregister_disappearing_link (link_addr);
	*link_addr = NULL;
}

static void*
RevealLink (void* link_addr)
{
	void **link_a = (void **)link_addr;
	return GC_REVEAL_POINTER (*link_a);
}

Il2CppObject*
il2cpp::gc::GarbageCollector::GetWeakLink (void **link_addr)
{
	Il2CppObject *obj = (Il2CppObject*)GC_call_with_alloc_lock (RevealLink, link_addr);
	if (obj == (Il2CppObject *) -1)
		return NULL;
	return obj;
}

void*
il2cpp::gc::GarbageCollector::MakeDescriptorForObject (size_t *bitmap, int numbits)
{
#ifdef GC_GCJ_SUPPORT
	/* It seems there are issues when the bitmap doesn't fit: play it safe */
	if (numbits >= 30)
		return GC_NO_DESCRIPTOR;
	else
		return (void*)GC_make_descriptor((GC_bitmap)bitmap, numbits);
#else
	return 0;
#endif
}

void* il2cpp::gc::GarbageCollector::MakeDescriptorForString ()
{
	return GC_NO_DESCRIPTOR;
}

void* il2cpp::gc::GarbageCollector::MakeDescriptorForArray ()
{
	return GC_NO_DESCRIPTOR;
}

void il2cpp::gc::GarbageCollector::StopWorld ()
{
	GC_stop_world_external ();
}

void il2cpp::gc::GarbageCollector::StartWorld ()
{
	GC_start_world_external ();
}

void*
il2cpp::gc::GarbageCollector::AllocateFixed (size_t size, void *descr)
{
	// Note that we changed the implementation from mono.
	// In our case, we expect that
	// a) This memory will never be moved
	// b) This memory will be scanned for references
	// c) This memory will remain 'alive' until explicitly freed
	// GC_MALLOC_UNCOLLECTABLE fulfills all these requirements
	// It does not accept a descriptor, but there was only one
	// or two places in mono that pass a descriptor to this routine
	// and we can or will support those use cases in a different manner.
	assert(!descr);

	return GC_MALLOC_UNCOLLECTABLE(size);
}

 void
il2cpp::gc::GarbageCollector::FreeFixed (void* addr)
{
	GC_FREE(addr);
}

int32_t
il2cpp::gc::GarbageCollector::InvokeFinalizers ()
{
	return (int32_t)GC_invoke_finalizers ();
}

bool
il2cpp::gc::GarbageCollector::HasPendingFinalizers ()
{
	return GC_should_invoke_finalizers () != 0;
}

#if IL2CPP_ENABLE_PROFILER

void on_gc_event (GCEventType eventType)
{
	Profiler::GCEvent ((Il2CppGCEvent)eventType);
}

void on_heap_resize (GC_word newSize)
{
	Profiler::GCHeapResize ((int64_t)newSize);
}

#endif // IL2CPP_ENABLE_PROFILER

void il2cpp::gc::GarbageCollector::ForEachHeapSection (void* user_data, HeapSectionCallback callback)
{
	GC_foreach_heap_section(user_data, callback);
}

size_t il2cpp::gc::GarbageCollector::GetSectionCount ()
{
	return GC_get_heap_section_count();
}

void* il2cpp::gc::GarbageCollector::CallWithAllocLockHeld (GCCallWithAllocLockCallback callback, void* user_data)
{
	return GC_call_with_alloc_lock(callback, user_data);
}

#endif
