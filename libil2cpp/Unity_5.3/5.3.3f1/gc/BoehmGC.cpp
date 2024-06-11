#include "il2cpp-config.h"

#if IL2CPP_GC_BOEHM

#include <stdint.h>
#include "gc-internal.h"
#include "vm/GC.h"
#include "vm/Profiler.h"
#include <cassert>

using il2cpp::vm::Profiler;

static bool s_GCInitialized = false;

#if IL2CPP_ENABLE_PROFILER
static void on_gc_event (GCEventType eventType);
static void on_heap_resize (GC_word newSize);
#endif

void
il2cpp_gc_base_init (void)
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
	GC_set_finalizer_notifier (&il2cpp_gc_finalize_notify);
	// We need to call this if we want to manually register threads, i.e. GC_register_my_thread
	GC_allow_register_threads ();
#endif
#ifdef GC_GCJ_SUPPORT
	GC_init_gcj_malloc (0, NULL);
#endif
	s_GCInitialized = true;
}

void
il2cpp_gc_collect (int generation)
{
	GC_gcollect ();
}

int
il2cpp_gc_max_generation (void)
{
	return 0;
}

int
il2cpp_gc_get_generation (Il2CppObject *object)
{
	return 0;
}

int
il2cpp_gc_collection_count (int generation)
{
	return (int)GC_get_gc_no ();
}

void
il2cpp_gc_add_memory_pressure (int64_t value)
{
}

void
il2cpp::vm::GC::Collect (int maxGeneration)
{
	assert(maxGeneration == 0); //Consumers should know this won't work.
	GC_gcollect ();
}

int64_t
il2cpp::vm::GC::GetUsedHeapSize (void)
{
	return GC_get_heap_size () - GC_get_free_bytes ();
}

int64_t
il2cpp::vm::GC::GetAllocatedHeapSize (void)
{
	return GC_get_heap_size ();
}

void
il2cpp_gc_disable (void)
{
	GC_disable ();
}

void
il2cpp_gc_enable (void)
{
	GC_enable ();
}

bool
il2cpp_gc_is_gc_thread (void)
{
#if defined(GC_THREADS)
	return GC_thread_is_registered () != 0;
#else
	return true;
#endif
}

bool
il2cpp_gc_register_thread (void *baseptr)
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
il2cpp_gc_unregister_thread ()
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

bool
il2cpp_object_is_alive (Il2CppObject* o)
{
	NOT_IMPLEMENTED (il2cpp_object_is_alive);
	// TODO: We could call GC_is_marked, but that assumes the allocation lock is held.
	// Just assert for now until we determine whether we should acquire the lock
	// or whether we assume the caller has acquired it.
	// return GC_is_marked (o) != 0;

	return true;
}

int
il2cpp_gc_register_root (char *start, size_t size, void *descr)
{
	NOT_IMPLEMENTED (il2cpp_gc_register_root);
	// I don't like this API. There are other APIs in Boehm to handle
	// all the mono use cases of this better now. Use them instead as needed.
	// Let's hopefully remove this if it's never needed.
	// GC_add_roots (start, start + size + 1);

	return true;
}


void
il2cpp_gc_deregister_root (char* addr)
{
	NOT_IMPLEMENTED (il2cpp_gc_deregister_root);
	// See above. This API should not be needed.
	// GC_remove_roots (addr, addr + sizeof (gpointer) + 1);
}

void
il2cpp_gc_deregister_root_size (char* addr, size_t size)
{
	NOT_IMPLEMENTED (il2cpp_gc_deregister_root_size);
	// See above.
	// GC_remove_roots (addr, addr + size + 1);
}

void
il2cpp_gc_weak_link_add (void **link_addr, Il2CppObject *obj, bool track)
{
	/* libgc requires that we use HIDE_POINTER... */
	*link_addr = (void*)GC_HIDE_POINTER (obj);
	// need this since our strings are not real objects
	if (GC_is_heap_ptr (obj))
		GC_GENERAL_REGISTER_DISAPPEARING_LINK (link_addr, obj);
}

void
il2cpp_gc_weak_link_remove (void **link_addr)
{
	Il2CppObject*  obj = il2cpp_gc_weak_link_get (link_addr);
	if (GC_is_heap_ptr (obj))
		GC_unregister_disappearing_link (link_addr);
	*link_addr = NULL;
}

static void*
reveal_link (void* link_addr)
{
	void **link_a = (void **)link_addr;
	return GC_REVEAL_POINTER (*link_a);
}

Il2CppObject*
il2cpp_gc_weak_link_get (void **link_addr)
{
	Il2CppObject *obj = (Il2CppObject*)GC_call_with_alloc_lock (reveal_link, link_addr);
	if (obj == (Il2CppObject *) -1)
		return NULL;
	return obj;
}

void*
il2cpp_gc_make_descr_from_bitmap (size_t *bitmap, int numbits)
{
#ifdef GC_GCJ_SUPPORT
	/* It seems there are issues when the bitmap doesn't fit: play it safe */
	if (numbits >= 30)
		return GC_NO_DESCRIPTOR;
	else
		return (void*)GC_make_descriptor ((GC_bitmap)bitmap, numbits);
#else
	return 0;
#endif
}

void*
il2cpp_gc_make_descr_for_string (size_t *bitmap, int numbits)
{
	return il2cpp_gc_make_descr_from_bitmap (bitmap, numbits);
}

void*
il2cpp_gc_make_descr_for_object (size_t *bitmap, int numbits)
{
	return il2cpp_gc_make_descr_from_bitmap (bitmap, numbits);
}

void il2cpp_gc_stop_world ()
{
	GC_stop_world_external ();
}

void il2cpp_gc_start_world ()
{
	GC_start_world_external ();
}

void*
il2cpp_gc_make_descr_for_array (int vector, size_t *elem_bitmap, int numbits, size_t elem_size)
{
#ifdef GC_GCJ_SUPPORT
	/* libgc has no usable support for arrays... */
	return GC_NO_DESCRIPTOR;
#else
	return NULL;
#endif
}

void*
il2cpp_gc_alloc_fixed (size_t size, void *descr)
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
il2cpp_gc_free_fixed (void* addr)
{
	GC_FREE(addr);
}

int
il2cpp_gc_invoke_finalizers ()
{
	return GC_invoke_finalizers ();
}

bool
il2cpp_gc_pending_finalizers (void)
{
	return GC_should_invoke_finalizers () != 0;
}

void
il2cpp_gc_add_weak_track_handle (Il2CppObject *obj, uint32_t handle)
{
	NOT_IMPLEMENTED (il2cpp_gc_add_weak_track_handle);
}

void
il2cpp_gc_remove_weak_track_handle (uint32_t gchandle)
{
	NOT_IMPLEMENTED (il2cpp_gc_remove_weak_track_handle);
}

void
il2cpp_gc_change_weak_track_handle (Il2CppObject *old_obj, Il2CppObject *obj, uint32_t gchandle)
{
	NOT_IMPLEMENTED (il2cpp_gc_change_weak_track_handle);
}

size_t
il2cpp_gc_get_generation (void* addr)
{
	return 0;
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

void il2cpp_gc_foreach_heap_section(void* user_data, HeapSectionCallback callback)
{
	GC_foreach_heap_section(user_data, callback);
}

size_t il2cpp_gc_get_section_count()
{
	return GC_get_heap_section_count();
}

void* il2cpp_gc_call_with_alloc_lock_held(GCCallWithAllocLockCallback callback, void* user_data)
{
	return GC_call_with_alloc_lock(callback, user_data);
}

#endif
