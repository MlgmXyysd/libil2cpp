#pragma once

#include "gc_wrapper.h"

struct Il2CppObject;
struct Il2CppThread;

void il2cpp_gc_init (void);
void il2cpp_gc_base_init (void);
void il2cpp_gc_cleanup (void);
void il2cpp_gc_enable (void);
void il2cpp_gc_disable (void);

void il2cpp_gc_collect (int generation);

bool il2cpp_gc_is_gc_thread (void);

bool il2cpp_gc_register_thread (void *baseptr);
bool il2cpp_gc_unregister_thread ();

void il2cpp_gc_weak_link_add (void **link_addr, Il2CppObject *obj, bool track);
void il2cpp_gc_weak_link_remove (void **link_addr);
Il2CppObject *il2cpp_gc_weak_link_get (void **link_addr);

void il2cpp_gc_add_weak_track_handle (Il2CppObject *obj, uint32_t gchandle);
void il2cpp_gc_change_weak_track_handle (Il2CppObject *old_obj, Il2CppObject *obj, uint32_t gchandle);
void il2cpp_gc_remove_weak_track_handle (uint32_t gchandle);

void* il2cpp_gc_alloc_fixed (size_t size, void *descr);
void il2cpp_gc_free_fixed (void* addr);
size_t il2cpp_gc_get_generation (void* addr);
void il2cpp_gc_add_memory_pressure (int64_t value);

bool il2cpp_gc_pending_finalizers ();
void il2cpp_gc_finalize_notify ();
int il2cpp_gc_invoke_finalizers ();
void il2cpp_gc_run_finalize (void *obj, void *data);
bool il2cpp_gc_has_finalizer_thread_object();

void il2cpp_gc_register_finalizer (Il2CppObject* obj);
void il2cpp_gc_suppress_finalizer (Il2CppObject* obj);
void il2cpp_gc_wait_for_pending_finalizers ();

bool il2cpp_gc_is_gc_thread (Il2CppThread *thread);

int64_t il2cpp_gc_get_used_size ();
int32_t il2cpp_gc_max_generation ();
int32_t il2cpp_gc_collection_count (int32_t generation);


void* il2cpp_gc_make_descr_for_object (size_t *bitmap, int numbits);

/* Used by liveness code */
void il2cpp_gc_stop_world ();
void il2cpp_gc_start_world ();
