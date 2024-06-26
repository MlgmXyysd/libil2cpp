#pragma once

#include <stdint.h>

#include "il2cpp-config.h"
#include "il2cpp-api-types.h"

extern "C"
{
	IL2CPP_EXPORT void il2cpp_init (const char* domain_name);
	IL2CPP_EXPORT void il2cpp_shutdown ();
	IL2CPP_EXPORT void il2cpp_set_config_dir (const char *config_path);
	IL2CPP_EXPORT void il2cpp_set_commandline_arguments(int argc, const char* argv[], const char* basedir);
	IL2CPP_EXPORT const Il2CppImage* il2cpp_get_corlib ();
	IL2CPP_EXPORT void il2cpp_add_internal_call(const char* name, methodPointerType method);
	IL2CPP_EXPORT methodPointerType il2cpp_resolve_icall(const char* name);

	IL2CPP_EXPORT void* il2cpp_alloc(size_t size);
	IL2CPP_EXPORT void il2cpp_free(void* ptr);

	// array
	IL2CPP_EXPORT TypeInfo *il2cpp_array_class_get (TypeInfo *element_class, uint32_t rank);
	IL2CPP_EXPORT uint32_t il2cpp_array_length(Il2CppArray* array);
	IL2CPP_EXPORT uint32_t il2cpp_array_get_byte_length(Il2CppArray *array);
	IL2CPP_EXPORT Il2CppArray* il2cpp_array_new (TypeInfo *elementTypeInfo, il2cpp_array_size_t length);
	IL2CPP_EXPORT Il2CppArray* il2cpp_array_new_specific (TypeInfo *arrayTypeInfo, il2cpp_array_size_t length);
	IL2CPP_EXPORT Il2CppArray* il2cpp_array_new_full (TypeInfo *array_class, il2cpp_array_size_t *lengths, il2cpp_array_size_t *lower_bounds);
	IL2CPP_EXPORT TypeInfo *il2cpp_bounded_array_class_get (TypeInfo *element_class, uint32_t rank, bool bounded);
	IL2CPP_EXPORT int il2cpp_array_element_size (const TypeInfo* array_class);

	// assembly
	IL2CPP_EXPORT const Il2CppImage* il2cpp_assembly_get_image (const Il2CppAssembly *assembly);

	// class
	IL2CPP_EXPORT const Il2CppType* il2cpp_class_enum_basetype (TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_is_generic (const TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_is_assignable_from (TypeInfo *klass, TypeInfo *oklass);
	IL2CPP_EXPORT bool il2cpp_class_is_subclass_of (TypeInfo *klass, TypeInfo *klassc, bool check_interfaces);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_from_il2cpp_type (const Il2CppType* type);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_from_name (const Il2CppImage* image, const char* namespaze, const char *name);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_from_system_type (Il2CppReflectionType *type);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_get_element_class (TypeInfo *klass);
	IL2CPP_EXPORT FieldInfo* il2cpp_class_get_fields (TypeInfo *klass, void* *iter);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_get_interfaces (TypeInfo *klass, void* *iter);
	IL2CPP_EXPORT PropertyInfo* il2cpp_class_get_properties (const TypeInfo *klass, void* *iter);
	IL2CPP_EXPORT PropertyInfo* il2cpp_class_get_property_from_name (const TypeInfo *klass, const char *name);
	IL2CPP_EXPORT FieldInfo* il2cpp_class_get_field_from_name (TypeInfo* klass, const char *name);
	IL2CPP_EXPORT MethodInfo* il2cpp_class_get_methods (const TypeInfo *klass, void* *iter);
	IL2CPP_EXPORT MethodInfo* il2cpp_class_get_method_from_name (TypeInfo *klass, const char* name, int argsCount);
	IL2CPP_EXPORT const char* il2cpp_class_get_name (TypeInfo *klass);
	IL2CPP_EXPORT const char* il2cpp_class_get_namespace (TypeInfo *klass);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_get_parent (TypeInfo *klass);
	IL2CPP_EXPORT int32_t il2cpp_class_instance_size (TypeInfo *klass);
	IL2CPP_EXPORT size_t il2cpp_class_num_fields(const TypeInfo* enumKlass);
	IL2CPP_EXPORT bool il2cpp_class_is_valuetype(const TypeInfo *klass);
	IL2CPP_EXPORT int32_t il2cpp_class_value_size (TypeInfo *klass, uint32_t *align);
	IL2CPP_EXPORT int il2cpp_class_get_flags (const TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_is_abstract (const TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_is_interface (const TypeInfo *klass);
	IL2CPP_EXPORT int il2cpp_class_array_element_size (const TypeInfo *klass);
	IL2CPP_EXPORT TypeInfo* il2cpp_class_from_type (Il2CppType *type);
	IL2CPP_EXPORT const Il2CppType* il2cpp_class_get_type (TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_has_attribute (TypeInfo *klass, TypeInfo *attr_class);
	IL2CPP_EXPORT bool il2cpp_class_has_references (TypeInfo *klass);
	IL2CPP_EXPORT bool il2cpp_class_is_enum (const TypeInfo *klass);
	IL2CPP_EXPORT Il2CppImage* il2cpp_class_get_image (TypeInfo* klass);
	IL2CPP_EXPORT const char *il2cpp_class_get_assemblyname (const TypeInfo *klass);
	// testing only
	IL2CPP_EXPORT size_t il2cpp_class_get_bitmap_size (const TypeInfo *klass);
	IL2CPP_EXPORT void il2cpp_class_get_bitmap (TypeInfo *klass, size_t* bitmap);

	// domain
	IL2CPP_EXPORT Il2CppDomain* il2cpp_domain_get ();
	IL2CPP_EXPORT Il2CppAssembly* il2cpp_domain_assembly_open (Il2CppDomain* domain, const char* name);
	IL2CPP_EXPORT Il2CppAssembly** il2cpp_domain_get_assemblies (const Il2CppDomain* domain, size_t* size);

	// exception
	IL2CPP_EXPORT void il2cpp_raise_exception(Il2CppException*);
	IL2CPP_EXPORT Il2CppException* il2cpp_exception_from_name_msg (const Il2CppImage* image, const char *name_space, const char *name, const char *msg);
	IL2CPP_EXPORT void il2cpp_format_exception(const Il2CppException* ex, char* message, int message_size);
	IL2CPP_EXPORT void il2cpp_format_stack_trace(const Il2CppException* ex, char* output, int output_size);

	// field
	IL2CPP_EXPORT int il2cpp_field_get_flags (FieldInfo *field);
	IL2CPP_EXPORT const char* il2cpp_field_get_name (FieldInfo *field);
	IL2CPP_EXPORT TypeInfo* il2cpp_field_get_parent (FieldInfo *field);
	IL2CPP_EXPORT size_t il2cpp_field_get_offset (FieldInfo *field);
	IL2CPP_EXPORT const Il2CppType* il2cpp_field_get_type (FieldInfo *field);
	IL2CPP_EXPORT void il2cpp_field_get_value (Il2CppObject *obj, FieldInfo *field, void *value);
	IL2CPP_EXPORT Il2CppObject* il2cpp_field_get_value_object (FieldInfo *field, Il2CppObject *obj);
	IL2CPP_EXPORT bool il2cpp_field_has_attribute (FieldInfo *field, TypeInfo *attr_class);
	IL2CPP_EXPORT void il2cpp_field_set_value (Il2CppObject *obj, FieldInfo *field, void *value);
	IL2CPP_EXPORT void il2cpp_field_static_get_value (FieldInfo *field, void *value);
	IL2CPP_EXPORT void il2cpp_field_static_set_value (FieldInfo *field, void *value);

	// gchandle
	IL2CPP_EXPORT uint32_t il2cpp_gchandle_new (Il2CppObject *obj, bool pinned);
	IL2CPP_EXPORT uint32_t il2cpp_gchandle_new_weakref (Il2CppObject *obj, bool track_resurrection);
	IL2CPP_EXPORT Il2CppObject* il2cpp_gchandle_get_target  (uint32_t gchandle);
	IL2CPP_EXPORT void il2cpp_gchandle_free (uint32_t gchandle);

	// liveness
	IL2CPP_EXPORT void* il2cpp_unity_liveness_calculation_begin (TypeInfo* filter, int max_object_count, register_object_callback callback, void* userdata);
	IL2CPP_EXPORT void il2cpp_unity_liveness_calculation_end (void* state);
	IL2CPP_EXPORT void il2cpp_unity_liveness_calculation_from_root (Il2CppObject* root, void* state);
	IL2CPP_EXPORT void il2cpp_unity_liveness_calculation_from_statics (void* state);

	// method
	IL2CPP_EXPORT const Il2CppType* il2cpp_method_get_return_type (MethodInfo* method);
	IL2CPP_EXPORT TypeInfo* il2cpp_method_get_declaring_type (MethodInfo* method);
	IL2CPP_EXPORT const char* il2cpp_method_get_name (MethodInfo *method);
	IL2CPP_EXPORT Il2CppReflectionMethod* il2cpp_method_get_object (MethodInfo *method, TypeInfo *refclass);
	IL2CPP_EXPORT bool il2cpp_method_is_generic (MethodInfo *method);
	IL2CPP_EXPORT bool il2cpp_method_is_inflated (MethodInfo *method);
	IL2CPP_EXPORT bool il2cpp_method_is_instance (MethodInfo *method);
	IL2CPP_EXPORT uint32_t il2cpp_method_get_param_count (MethodInfo *method);
	IL2CPP_EXPORT const Il2CppType* il2cpp_method_get_param (MethodInfo *method, uint32_t index);
	IL2CPP_EXPORT TypeInfo* il2cpp_method_get_class (MethodInfo *method);
	IL2CPP_EXPORT bool il2cpp_method_has_attribute (MethodInfo *method, TypeInfo *attr_class);
	IL2CPP_EXPORT uint32_t il2cpp_method_get_flags (const MethodInfo *method, uint32_t *iflags);
	IL2CPP_EXPORT uint32_t il2cpp_method_get_token (const MethodInfo *method);
	IL2CPP_EXPORT const char *il2cpp_method_get_param_name (MethodInfo *method, uint32_t index);

	// profiler
	IL2CPP_EXPORT void il2cpp_profiler_install (Il2CppProfiler *prof, Il2CppProfileFunc shutdown_callback);
	IL2CPP_EXPORT void il2cpp_profiler_set_events (Il2CppProfileFlags events);
	IL2CPP_EXPORT void il2cpp_profiler_install_enter_leave (Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave);
	IL2CPP_EXPORT void il2cpp_profiler_install_allocation (Il2CppProfileAllocFunc callback);
	IL2CPP_EXPORT void il2cpp_profiler_install_gc (Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback);

	// property
	IL2CPP_EXPORT uint32_t il2cpp_property_get_flags (PropertyInfo *prop);
	IL2CPP_EXPORT MethodInfo* il2cpp_property_get_get_method (PropertyInfo *prop);
	IL2CPP_EXPORT MethodInfo* il2cpp_property_get_set_method (PropertyInfo *prop);
	IL2CPP_EXPORT const char* il2cpp_property_get_name (PropertyInfo *prop);
	IL2CPP_EXPORT TypeInfo* il2cpp_property_get_parent (PropertyInfo *prop);

	// object
	IL2CPP_EXPORT TypeInfo* il2cpp_object_get_class(Il2CppObject* obj);
	IL2CPP_EXPORT uint32_t il2cpp_object_get_size(Il2CppObject* obj);
	IL2CPP_EXPORT MethodInfo* il2cpp_object_get_virtual_method (Il2CppObject *obj, MethodInfo *method);
	IL2CPP_EXPORT Il2CppObject* il2cpp_object_new (const TypeInfo *klass);
	IL2CPP_EXPORT void* il2cpp_object_unbox (Il2CppObject* obj);

	IL2CPP_EXPORT Il2CppObject* il2cpp_value_box (TypeInfo *klass, void* data);

	// monitor
	IL2CPP_EXPORT void il2cpp_monitor_enter (Il2CppObject* obj);
	IL2CPP_EXPORT bool il2cpp_monitor_try_enter (Il2CppObject* obj, uint32_t timeout);
	IL2CPP_EXPORT void il2cpp_monitor_exit (Il2CppObject* obj);
	IL2CPP_EXPORT void il2cpp_monitor_pulse (Il2CppObject* obj);
	IL2CPP_EXPORT void il2cpp_monitor_pulse_all (Il2CppObject* obj);
	IL2CPP_EXPORT void il2cpp_monitor_wait (Il2CppObject* obj);
	IL2CPP_EXPORT bool il2cpp_monitor_try_wait (Il2CppObject* obj, uint32_t timeout);

	// runtime
	IL2CPP_EXPORT Il2CppObject* il2cpp_runtime_invoke (MethodInfo *method, void *obj, void **params, Il2CppObject **exc);
	IL2CPP_EXPORT Il2CppObject* il2cpp_runtime_invoke_convert_args (MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppObject **exc);
	IL2CPP_EXPORT void il2cpp_runtime_class_init (TypeInfo* klass);
	IL2CPP_EXPORT void il2cpp_runtime_object_init (Il2CppObject* obj);

	IL2CPP_EXPORT void il2cpp_runtime_object_init_exception (Il2CppObject* obj, Il2CppObject** exc);
	
	IL2CPP_EXPORT void il2cpp_runtime_unhandled_exception_policy_set (Il2CppRuntimeUnhandledExceptionPolicy value);

	// delegate
	IL2CPP_EXPORT Il2CppAsyncResult* il2cpp_delegate_begin_invoke (Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, Il2CppObject* state);
	IL2CPP_EXPORT Il2CppObject* il2cpp_delegate_end_invoke (Il2CppAsyncResult* asyncResult, void **out_args);

	// string
	IL2CPP_EXPORT int32_t il2cpp_string_length (Il2CppString* str);
	IL2CPP_EXPORT uint16_t* il2cpp_string_chars (Il2CppString* str);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_new (const char* str);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_new_len (const char* str, uint32_t length);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_new_utf16 (const uint16_t *text, int32_t len);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_new_wrapper (const char* str);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_intern(Il2CppString* str);
	IL2CPP_EXPORT Il2CppString* il2cpp_string_is_interned(Il2CppString* str);

	// thread
	IL2CPP_EXPORT char *il2cpp_thread_get_name (Il2CppThread *thread, uint32_t *len);
	IL2CPP_EXPORT Il2CppThread *il2cpp_thread_current ();
	IL2CPP_EXPORT Il2CppThread *il2cpp_thread_attach (Il2CppDomain *domain);
	IL2CPP_EXPORT void il2cpp_thread_detach (Il2CppThread *thread);
	IL2CPP_EXPORT void il2cpp_thread_push_frame (Il2CppThread *thread, Il2CppStackFrameInfo info);
	IL2CPP_EXPORT void il2cpp_thread_pop_frame (Il2CppThread *thread);
	IL2CPP_EXPORT void il2cpp_thread_walk_frame_stack (Il2CppThread *thread, Il2CppFrameWalkFunc func, void *user_data);
	IL2CPP_EXPORT Il2CppStackFrameInfo *il2cpp_thread_get_top_frame (Il2CppThread *thread);
	IL2CPP_EXPORT Il2CppStackFrameInfo *il2cpp_thread_get_frame_at (Il2CppThread *thread, int32_t offset);
	IL2CPP_EXPORT Il2CppThread **il2cpp_thread_get_all_attached_threads (size_t *size);
	IL2CPP_EXPORT bool il2cpp_is_vm_thread (Il2CppThread *thread);
	IL2CPP_EXPORT int32_t il2cpp_thread_get_stack_depth (Il2CppThread *thread);

	// type
	IL2CPP_EXPORT Il2CppObject* il2cpp_type_get_object (const Il2CppType *type);
	IL2CPP_EXPORT int il2cpp_type_get_type (const Il2CppType *type);
	IL2CPP_EXPORT TypeInfo* il2cpp_type_get_class_or_element_class (const Il2CppType *type);
	IL2CPP_EXPORT char* il2cpp_type_get_name (const Il2CppType *type);

	// image
	IL2CPP_EXPORT Il2CppAssembly* il2cpp_image_get_assembly (Il2CppImage *image);
	IL2CPP_EXPORT const char* il2cpp_image_get_name (Il2CppImage *image);
	IL2CPP_EXPORT const char* il2cpp_image_get_filename (const Il2CppImage *image);
	IL2CPP_EXPORT MethodInfo* il2cpp_image_get_entry_point (const Il2CppImage* image);


#if IL2CPP_DEBUGGER_ENABLED
	// debug
	IL2CPP_EXPORT const Il2CppDebugTypeInfo* il2cpp_debug_get_class_info (const TypeInfo *klass);
	IL2CPP_EXPORT const Il2CppDebugDocument* il2cpp_debug_class_get_document (const Il2CppDebugTypeInfo* info);
	IL2CPP_EXPORT const char* il2cpp_debug_document_get_filename (const Il2CppDebugDocument* document);
	IL2CPP_EXPORT const char* il2cpp_debug_document_get_directory (const Il2CppDebugDocument* document);
	IL2CPP_EXPORT const Il2CppDebugMethodInfo* il2cpp_debug_get_method_info (const MethodInfo *method);
	IL2CPP_EXPORT const Il2CppDebugDocument* il2cpp_debug_method_get_document (const Il2CppDebugMethodInfo* info);
	IL2CPP_EXPORT const int32_t* il2cpp_debug_method_get_offset_table (const Il2CppDebugMethodInfo* info);
	IL2CPP_EXPORT size_t il2cpp_debug_method_get_code_size (const Il2CppDebugMethodInfo* info);
	IL2CPP_EXPORT void il2cpp_debug_thread_update_frame_il_offset (Il2CppThread *thread, int32_t il_offset);
	IL2CPP_EXPORT const Il2CppDebugLocalsInfo **il2cpp_debug_method_get_locals_info (const Il2CppDebugMethodInfo* info);
	IL2CPP_EXPORT const TypeInfo *il2cpp_debug_local_get_type (const Il2CppDebugLocalsInfo *info);
	IL2CPP_EXPORT const char *il2cpp_debug_local_get_name (const Il2CppDebugLocalsInfo *info);
	IL2CPP_EXPORT uint32_t il2cpp_debug_local_get_start_offset (const Il2CppDebugLocalsInfo *info);
	IL2CPP_EXPORT uint32_t il2cpp_debug_local_get_end_offset (const Il2CppDebugLocalsInfo *info);
	IL2CPP_EXPORT Il2CppObject *il2cpp_debug_method_get_param_value (const Il2CppStackFrameInfo *info, uint32_t position);
	IL2CPP_EXPORT Il2CppObject *il2cpp_debug_frame_get_local_value (const Il2CppStackFrameInfo *info, uint32_t position);
	IL2CPP_EXPORT void *il2cpp_debug_method_get_breakpoint_data_at (const Il2CppDebugMethodInfo* info, int64_t uid, int32_t offset);
	IL2CPP_EXPORT void il2cpp_debug_method_set_breakpoint_data_at (const Il2CppDebugMethodInfo* info, uint64_t location, void *data);
	IL2CPP_EXPORT void il2cpp_debug_method_clear_breakpoint_data (const Il2CppDebugMethodInfo* info);
	IL2CPP_EXPORT void il2cpp_debug_method_clear_breakpoint_data_at (const Il2CppDebugMethodInfo* info, uint64_t location);
#endif
	
}
