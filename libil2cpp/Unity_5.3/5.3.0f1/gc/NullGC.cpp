#include "il2cpp-config.h"

#if IL2CPP_GC_NULL
struct Il2CppObject;

#include <stdlib.h>
#include "il2cpp-api.h"
#include "vm/GC.h"
#include "utils/Memory.h"

void
il2cpp_gc_base_init (void)
{
}

void
il2cpp_gc_disable (void)
{
}

void
il2cpp_gc_enable (void)
{
}


void*
il2cpp_gc_alloc_fixed (size_t size, void *descr)
{
	return IL2CPP_MALLOC_ZERO (size);
}

void*
il2cpp_gc_make_descr_from_bitmap (size_t *bitmap, int numbits)
{
	return NULL;
}

void*
il2cpp_gc_make_descr_for_string (size_t *bitmap, int numbits)
{
	return NULL;
}

void*
il2cpp_gc_make_descr_for_object (size_t *bitmap, int numbits)
{
	return NULL;
}

void il2cpp_gc_stop_world ()
{
}

void il2cpp_gc_start_world ()
{
}

void*
il2cpp_gc_make_descr_for_array (int vector, size_t *elem_bitmap, int numbits, size_t elem_size)
{
	return NULL;
}

void
il2cpp_gc_weak_link_remove (void **link_addr)
{
	*link_addr = NULL;
}

Il2CppObject*
il2cpp_gc_weak_link_get (void **link_addr)
{
	return (Il2CppObject*)*link_addr;
}

void
il2cpp_gc_weak_link_add (void **link_addr, Il2CppObject *obj, bool track)
{
	*link_addr = obj;
}

void
il2cpp_gc_remove_weak_track_handle (uint32_t gchandle)
{

}

void
il2cpp_gc_add_weak_track_handle (Il2CppObject *obj, uint32_t handle)
{

}

bool
il2cpp_gc_unregister_thread ()
{
	return true;
}

bool
il2cpp_gc_register_thread (void *baseptr)
{
	return true;
}

bool
il2cpp_gc_is_gc_thread (void)
{
	return false;
}

void
il2cpp_gc_free_fixed (void* addr)
{
	IL2CPP_FREE (addr);
}

int
il2cpp_gc_invoke_finalizers ()
{
	return 0;
}

bool
il2cpp_gc_pending_finalizers (void)
{
	return false;
}

void
il2cpp_gc_change_weak_track_handle (Il2CppObject *old_obj, Il2CppObject *obj, uint32_t gchandle)
{
}

void
il2cpp_gc_collect (int generation)
{
}

size_t
il2cpp_gc_get_generation (void* addr)
{
	return 0;
}

void
il2cpp_gc_add_memory_pressure (int64_t value)
{

}

int64_t
il2cpp::vm::GC::GetUsedHeapSize (void)
{
	return 0;
}

int64_t
il2cpp::vm::GC::GetAllocatedHeapSize (void)
{
	return 0;
}

int32_t
il2cpp_gc_max_generation ()
{
	return 0;
}

int32_t
il2cpp_gc_collection_count (int32_t generation)
{
	return 0;
}

void il2cpp_gc_foreach_heap_section(void* user_data, HeapSectionCallback callback)
{
}

size_t il2cpp_gc_get_section_count()
{
	return 0;
}

void* il2cpp_gc_call_with_alloc_lock_held(GCCallWithAllocLockCallback callback, void* user_data)
{
	return callback(user_data);
}

#endif
