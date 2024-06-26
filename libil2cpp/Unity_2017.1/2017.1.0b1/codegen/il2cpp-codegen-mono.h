#pragma once

#include "il2cpp-config.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <string>
#include <math.h>
#include <vector>

#include "object-internals.h"
#include "class-internals.h"
#include "tabledefs.h"
#include "icallincludes.h"

#include "vm/PlatformInvoke.h"
#include "vm/StackTrace.h"

#include "il2cpp-mono-support.h"

#include "utils/Exception.h"
#include "utils/Output.h"
#include "utils/Runtime.h"
#include "utils/StringUtils.h"
#include "utils/StringView.h"

#include "mono-api.h"

#ifdef _MSC_VER
#define IL2CPP_DEBUG_BREAK() __debugbreak()
#else
#define IL2CPP_DEBUG_BREAK()
#endif

struct ProfilerMethodSentry
{
    ProfilerMethodSentry(const MethodInfo* method)
#if IL2CPP_ENABLE_PROFILER
        : m_method(method)
#endif
    {
        NOT_IMPLEMENTED("Unity profiler hooks are not implemented yet for the libmonoruntime backend.");
    }

    ~ProfilerMethodSentry()
    {
        NOT_IMPLEMENTED("Unity profiler hooks are not implemented yet for the libmonoruntime backend.");
    }

private:
    const MethodInfo* m_method;
};

struct StackTraceSentry
{
    StackTraceSentry(MonoMethod* method) : m_method(method)
    {
        MonoStackFrameInfo frame_info;

        frame_info.method = method;
        frame_info.actual_method = method;
        frame_info.type = MONO_FRAME_TYPE_MANAGED;
        frame_info.managed = 1;
        frame_info.il_offset = 0;
        frame_info.native_offset = 0;
        frame_info.ji = (void*)-1;

        mono::vm::StackTrace::PushFrame(frame_info);
    }

#if IL2CPP_DEBUGGER_ENABLED
    StackTraceSentry(const MonoMethod* method, void* this_ptr, void **params, int32_t params_count, void **locals, int32_t locals_count) : m_method(method)
    {
    }

#endif

    ~StackTraceSentry()
    {
        mono::vm::StackTrace::PopFrame();
    }

private:
    const MonoMethod* m_method;
};

template<typename T>
struct Il2CppFakeBox : MonoObject
{
    T m_Value;

    Il2CppFakeBox(MonoClass* boxedType, T* value)
    {
        vtable = mono_class_vtable(mono_domain_get(), boxedType);
        synchronisation = NULL;
        m_Value = *value;
    }
};

// TODO: This file should contain all the functions and type declarations needed for the generated code.
// Hopefully, we stop including everything in the generated code and know exactly what dependencies we have.
// Note that all parameter and return types should match the generated types not the runtime types.

#include "GeneratedCodeGen.h"

// type registration

inline Il2CppCodeGenString* il2cpp_codegen_string_new_utf16(const il2cpp::utils::StringView<Il2CppChar>& str)
{
    return (Il2CppCodeGenString*)mono_string_new_utf16(mono_domain_get(), (uint16_t*)str.Str(), (int32_t)str.Length());
}

// This function exists to help with generation of callstacks for exceptions
// on iOS and MacOS x64 with clang 6.0 (newer versions of clang don't have this
// problem on x64). There we call the backtrace function, which does not play nicely
// with NORETURN, since the compiler eliminates the method prologue code setting up
// the address of the return frame (which makes sense). So on iOS we need to make
// the NORETURN define do nothing, then we use this dummy method which has the
// attribute for clang on iOS defined to prevent clang compiler errors for
// method that end by throwing a managed exception.
REAL_NORETURN IL2CPP_NO_INLINE static void il2cpp_codegen_no_return()
{
    IL2CPP_UNREACHABLE;
}

inline NORETURN void il2cpp_codegen_raise_exception(Il2CppCodeGenException *ex)
{
    mono_raise_exception((MonoException*)ex);
    il2cpp_codegen_no_return();
}

inline void il2cpp_codegen_raise_execution_engine_exception_if_method_is_not_found(const MethodInfo* method)
{
    il2cpp_mono_raise_execution_engine_exception_if_method_is_not_found((MonoMethod*)method);
}

inline void il2cpp_codegen_raise_execution_engine_exception(const MethodInfo* method)
{
    mono_raise_exception(mono_get_exception_execution_engine(method->name));
}

inline Il2CppCodeGenException* il2cpp_codegen_get_argument_exception(const char* param, const char* msg)
{
    return (Il2CppCodeGenException*)mono_get_exception_argument(param, msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_argument_null_exception(const char* param)
{
    return (Il2CppCodeGenException*)mono_get_exception_argument_null(param);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_overflow_exception()
{
    return (Il2CppCodeGenException*)mono_get_exception_overflow();
}

inline Il2CppCodeGenException* il2cpp_codegen_get_not_supported_exception(const char* msg)
{
    return (Il2CppCodeGenException*)mono_get_exception_not_supported(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_array_type_mismatch_exception()
{
    return (Il2CppCodeGenException*)mono_get_exception_array_type_mismatch();
}

inline Il2CppCodeGenException* il2cpp_codegen_get_invalid_operation_exception(const char* msg)
{
    return (Il2CppCodeGenException*)mono_get_exception_invalid_operation(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_marshal_directive_exception(const char* msg)
{
    return (Il2CppCodeGenException*)mono_unity_exception_get_marshal_directive(msg);
}

inline Il2CppCodeGenException* il2cpp_codegen_get_missing_method_exception(const char* msg)
{
    return (Il2CppCodeGenException*)mono_get_exception_missing_method(msg, "ctor");
}

inline Il2CppCodeGenException* il2cpp_codegen_get_maximum_nested_generics_exception()
{
    return (Il2CppCodeGenException*)mono_get_exception_not_supported(MAXIMUM_NESTED_GENERICS_EXCEPTION_MESSAGE);
}

inline Il2CppClass* il2cpp_codegen_object_class(Il2CppObject* obj)
{
    return (Il2CppClass*)mono_object_get_class((MonoObject*)obj);
}

// OpCode.IsInst

inline Il2CppCodeGenObject* IsInst(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    return (Il2CppCodeGenObject*)mono_object_isinst((MonoObject*)obj, targetType);
}

inline Il2CppCodeGenObject* IsInstSealed(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    if (!obj)
        return NULL;

    // optimized version to compare sealed classes
    return (Il2CppCodeGenObject*)mono_unity_object_isinst_sealed((MonoObject*)obj, targetType);
}

inline Il2CppCodeGenObject* IsInstClass(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    if (!obj)
        return NULL;

    return mono_unity_class_has_parent_unsafe(mono_object_get_class((MonoObject*)obj), targetType) ? obj : NULL;
}

// OpCode.Castclass

inline Il2CppCodeGenObject* Castclass(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    if (!obj)
        return NULL;

    Il2CppCodeGenObject* result = (Il2CppCodeGenObject*)mono_object_isinst((MonoObject*)obj, targetType);
    if (result)
        return (Il2CppCodeGenObject*)result;

    mono_raise_exception(mono_get_exception_invalid_cast());
    return NULL;
}

inline Il2CppCodeGenObject* CastclassSealed(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    if (!obj)
        return NULL;

    Il2CppCodeGenObject* result = IsInstSealed(obj, targetType);
    if (result)
        return result;

    mono_raise_exception(mono_get_exception_invalid_cast());
    return NULL;
}

inline Il2CppCodeGenObject* CastclassClass(Il2CppCodeGenObject *obj, MonoClass* targetType)
{
    // Should this do a different or optimized check?
    return Castclass(obj, targetType);
}

// OpCode.Box

inline Il2CppCodeGenObject* Box(MonoClass* type, void* data)
{
    return (Il2CppCodeGenObject*)mono_value_box(mono_domain_get(), type, data);
}

// OpCode.UnBox

inline void* UnBox(Il2CppCodeGenObject* obj)
{
    if (!obj)
    {
        mono_raise_exception(mono_get_exception_null_reference());
        il2cpp_codegen_no_return();
    }

    return mono_object_unbox((MonoObject*)obj);
}

inline void* UnBox(Il2CppCodeGenObject* obj, MonoClass* klass)
{
    if (!obj)
    {
        mono_raise_exception(mono_get_exception_null_reference());
        il2cpp_codegen_no_return();
    }

    if (!mono_unity_object_check_box_cast((MonoObject*)obj, klass))
    {
        mono_raise_exception(mono_get_exception_invalid_cast());
        il2cpp_codegen_no_return();
    }

    return mono_object_unbox((MonoObject*)obj);
}

inline void UnBoxNullable(Il2CppCodeGenObject* obj, MonoClass* klass, void* storage)
{
    mono_unity_object_unbox_nullable((MonoObject*)obj, klass, storage);
}

inline uint32_t il2cpp_codegen_sizeof(MonoClass* klass)
{
    if (!mono_class_is_valuetype(klass))
        return sizeof(void*);

    return mono_class_instance_size(klass) - sizeof(Il2CppObject);
}

inline uint32_t il2cpp_codegen_sizeof(Il2CppClass* klass)
{
    if (!mono_class_is_valuetype((MonoClass*)klass))
        return sizeof(void*);

    return mono_class_instance_size((MonoClass*)klass) - sizeof(Il2CppObject);
}

FORCE_INLINE const VirtualInvokeData il2cpp_codegen_get_virtual_invoke_data(MonoMethod* method, void* obj)
{
    VirtualInvokeData invokeData;
    il2cpp_mono_get_invoke_data(method, obj, &invokeData);
    return invokeData;
}

FORCE_INLINE const VirtualInvokeData il2cpp_codegen_get_interface_invoke_data(MonoMethod* method, void* obj, MonoClass* declaringInterface)
{
    VirtualInvokeData invokeData;
    il2cpp_mono_get_invoke_data(method, obj, &invokeData);
    return invokeData;
}

FORCE_INLINE const MethodInfo* il2cpp_codegen_get_generic_virtual_method(const MethodInfo* method, const Il2CppObject* obj)
{
    return (const MethodInfo*)il2cpp_mono_get_virtual_target_method((MonoMethod*)method, (MonoObject*)obj);
}

FORCE_INLINE void il2cpp_codegen_get_generic_virtual_invoke_data(const MethodInfo* method, void* obj, VirtualInvokeData* invokeData)
{
    il2cpp_mono_get_invoke_data((MonoMethod*)method, obj, invokeData);
}

FORCE_INLINE void il2cpp_codegen_get_generic_virtual_invoke_data(const MonoMethod* method, void* obj, VirtualInvokeData* invokeData)
{
    il2cpp_codegen_get_generic_virtual_invoke_data((const MethodInfo*)method, obj, invokeData);
}

FORCE_INLINE const MethodInfo* il2cpp_codegen_get_generic_interface_method(const RuntimeMethod* method, const Il2CppObject* obj)
{
    return (const MethodInfo*)il2cpp_mono_get_virtual_target_method((MonoMethod*)method, (MonoObject*)obj);
}

FORCE_INLINE void il2cpp_codegen_get_generic_interface_invoke_data(MonoMethod* method, void* obj, VirtualInvokeData* invokeData)
{
    il2cpp_mono_get_invoke_data(method, obj, invokeData);
}

FORCE_INLINE void il2cpp_codegen_get_generic_interface_invoke_data(const MethodInfo* method, void* obj, VirtualInvokeData* invokeData)
{
    il2cpp_codegen_get_generic_interface_invoke_data((MonoMethod*)method, obj, invokeData);
}

#include "GeneratedVirtualInvokers.h"
#include "GeneratedInterfaceInvokers.h"
#include "GeneratedGenericVirtualInvokers.h"
#include "GeneratedGenericInterfaceInvokers.h"

// OpCode.Ldtoken

inline Il2CppCodeGenRuntimeTypeHandle LoadTypeToken(const MonoType* ptr)
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0(const_cast<MonoType*>(ptr));

    Il2CppCodeGenRuntimeTypeHandle handle;
    handle.set_value_0(intptr);

    return handle;
}

inline Il2CppCodeGenRuntimeFieldHandle LoadFieldToken(void* ptr)
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0(ptr);

    Il2CppCodeGenRuntimeFieldHandle handle;
    handle.set_value_0(intptr);

    return handle;
}

inline Il2CppCodeGenRuntimeArgumentHandle LoadArgList()
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0(NULL);

    Il2CppCodeGenRuntimeArgumentHandle handle;
    handle.set_args_0(intptr);

    assert(0 && false && "__arglist usage not supported.");
    return handle;
}

inline Il2CppCodeGenRuntimeMethodHandle LoadMethodToken(const RuntimeMethod* ptr)
{
    Il2CppCodeGenIntPtr intptr;
    intptr.set_m_value_0((void*)ptr);

    Il2CppCodeGenRuntimeMethodHandle handle;
    handle.set_value_0(intptr);

    return handle;
}

inline MonoClass* il2cpp_codegen_class_from_type(MonoType* type)
{
    return mono_class_from_mono_type((MonoType*)type);
}

inline MonoClass* il2cpp_codegen_class_from_type(const Il2CppType *type)
{
    return il2cpp_codegen_class_from_type((MonoType*)type);
}

template<typename T>
inline T InterlockedCompareExchangeImpl(T* location, T value, T comparand)
{
    return (T)mono_unity_object_compare_exchange((MonoObject**)location, (MonoObject*)value, (MonoObject*)comparand);
}

template<typename T>
inline T InterlockedExchangeImpl(T* location, T value)
{
    return (T)mono_unity_object_exchange((MonoObject**)location, (MonoObject*)value);
}

inline void ArrayGetGenericValueImpl(Il2CppCodeGenArray* __this, int32_t pos, void* value)
{
    int elementSize = mono_unity_array_get_element_size((MonoArray*)__this);
    memcpy(value, mono_array_addr_with_size((MonoArray*)__this, elementSize, pos), elementSize);
}

inline void ArraySetGenericValueImpl(Il2CppCodeGenArray * __this, int32_t pos, void* value)
{
    int elementSize = mono_unity_array_get_element_size((MonoArray*)__this);
    memcpy(mono_array_addr_with_size((MonoArray*)__this, elementSize, pos), value, elementSize);
}

inline Il2CppCodeGenArray* SZArrayNew(MonoClass* arrayType, uint32_t length)
{
    mono_class_init(arrayType);

    MonoError error;
    MonoArray *retVal = mono_array_new_specific_checked(mono_class_vtable(mono_domain_get(), arrayType), length, &error);
    MonoException *exc = mono_error_convert_to_exception(&error);
    if (exc)
        mono_raise_exception(exc);

    return (Il2CppCodeGenArray*)retVal;
}

inline Il2CppCodeGenArray* GenArrayNew(MonoClass* arrayType, il2cpp_array_size_t* dimensions)
{
    // We need to convert to the platform-sized integer type Mono uses. This doesn't look nice, maybe we should change this.
    std::vector<uintptr_t> monoDimensions = il2cpp_to_mono_array_dimensions(dimensions, mono_class_get_rank(arrayType));

    MonoError error;
    MonoArray *retVal = mono_array_new_full_checked(mono_domain_get(), arrayType, &monoDimensions[0], NULL, &error);
    MonoException *exc = mono_error_convert_to_exception(&error);
    if (exc)
        mono_raise_exception(exc);

    return (Il2CppCodeGenArray*)retVal;
}

// Performance optimization as detailed here: http://blogs.msdn.com/b/clrcodegeneration/archive/2009/08/13/array-bounds-check-elimination-in-the-clr.aspx
// Since array size is a signed int32_t, a single unsigned check can be performed to determine if index is less than array size.
// Negative indices will map to a unsigned number greater than or equal to 2^31 which is larger than allowed for a valid array.
#define IL2CPP_ARRAY_BOUNDS_CHECK(index, length) \
    do { \
        if (((uint32_t)(index)) >= ((uint32_t)length)) mono_raise_exception(mono_get_exception_index_out_of_range()); \
    } while (0)

inline bool il2cpp_codegen_class_is_assignable_from(MonoClass *klass, MonoClass *oklass)
{
    return mono_class_is_assignable_from(klass, oklass);
}

inline bool il2cpp_codegen_class_is_assignable_from(MonoClass *klass, Il2CppClass *oklass)
{
    // Not implemented yet. - This dummy verison is necessary until we switch to use MonoObject instead of IL2CppObject.
    return il2cpp_codegen_class_is_assignable_from(klass, (MonoClass*)oklass);
}

inline Il2CppObject* il2cpp_codegen_object_new(MonoClass *klass)
{
    return (Il2CppObject*)mono_object_new(mono_domain_get(), klass);
}

inline Il2CppMethodPointer il2cpp_codegen_resolve_icall(MonoMethod* icallMethod)
{
    return (Il2CppMethodPointer)mono_lookup_internal_call(icallMethod);
}

template<typename FunctionPointerType>
inline FunctionPointerType il2cpp_codegen_resolve_pinvoke(MonoMethod* pinvokeMethod)
{
    const char *exc_class, *exc_arg;
    FunctionPointerType result = reinterpret_cast<FunctionPointerType>(mono_lookup_pinvoke_call(pinvokeMethod, &exc_class, &exc_arg));
    if (exc_class)
    {
        mono_raise_exception(mono_exception_from_name_msg(mono_unity_image_get_mscorlib(), "System", exc_class, exc_arg));
        il2cpp_codegen_no_return();
    }

    return result;
}

template<typename T>
inline T* il2cpp_codegen_marshal_allocate_array(size_t length)
{
    MonoError unused;
    return (T*)mono_marshal_alloc((il2cpp_array_size_t)(sizeof(T) * length), &unused);
}

inline char* il2cpp_codegen_marshal_string(Il2CppCodeGenString* il2CppString)
{
    return mono::vm::PlatformInvoke::MarshalCSharpStringToCppString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_string_fixed(Il2CppCodeGenString* il2CppString, char* buffer, int numberOfCharacters)
{
    return mono::vm::PlatformInvoke::MarshalCSharpStringToCppStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline Il2CppChar* il2cpp_codegen_marshal_wstring(Il2CppCodeGenString* il2CppString)
{
    return mono::vm::PlatformInvoke::MarshalCSharpStringToCppWString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_wstring_fixed(Il2CppCodeGenString* il2CppString, Il2CppChar* buffer, int numberOfCharacters)
{
    return mono::vm::PlatformInvoke::MarshalCSharpStringToCppWStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline Il2CppChar* il2cpp_codegen_marshal_bstring(Il2CppCodeGenString* il2CppString)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_string_result(const char* value)
{
    return (Il2CppCodeGenString*)mono::vm::PlatformInvoke::MarshalCppStringToCSharpStringResult(value);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_wstring_result(const Il2CppChar* value)
{
    return (Il2CppCodeGenString*)mono::vm::PlatformInvoke::MarshalCppWStringToCSharpStringResult(value);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_bstring_result(const Il2CppChar* value)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_marshal_free_bstring(Il2CppChar* value)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline char* il2cpp_codegen_marshal_string_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
    return mono::vm::PlatformInvoke::MarshalStringBuilder((Il2CppStringBuilder*)stringBuilder);
}

inline Il2CppChar* il2cpp_codegen_marshal_wstring_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
    return mono::vm::PlatformInvoke::MarshalWStringBuilder((Il2CppStringBuilder*)stringBuilder);
}

inline void il2cpp_codegen_marshal_string_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, char* buffer)
{
    mono::vm::PlatformInvoke::MarshalStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

inline void il2cpp_codegen_marshal_wstring_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, Il2CppChar* buffer)
{
    mono::vm::PlatformInvoke::MarshalWStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

inline Il2CppHString il2cpp_codegen_create_hstring(Il2CppCodeGenString* str)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_hstring_result(Il2CppHString hstring)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_marshal_free_hstring(Il2CppHString hstring)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline void il2cpp_codegen_marshal_free(void* ptr)
{
    mono_marshal_free(ptr);
}

inline Il2CppMethodPointer il2cpp_codegen_marshal_delegate(Il2CppCodeGenMulticastDelegate* d)
{
    return (Il2CppMethodPointer)mono::vm::PlatformInvoke::MarshalDelegate((Il2CppDelegate*)d).m_value;
}

template<typename T>
inline T* il2cpp_codegen_marshal_function_ptr_to_delegate(Il2CppMethodPointer functionPtr, MonoClass* delegateType)
{
    return (T*)mono::vm::PlatformInvoke::MarshalFunctionPointerToDelegate(reinterpret_cast<void*>(functionPtr), delegateType);
}

inline void il2cpp_codegen_marshal_store_last_error()
{
    mono_marshal_set_last_error();
}

class il2cpp_native_wrapper_vm_thread_attacher
{
public:
    il2cpp_native_wrapper_vm_thread_attacher() :
        _threadWasAttached(false)
    {
        if (!mono_thread_is_attached())
        {
            mono_thread_attach(mono_get_root_domain());
            _threadWasAttached = true;
        }
    }

    ~il2cpp_native_wrapper_vm_thread_attacher()
    {
        if (_threadWasAttached)
            mono_thread_detach(mono_thread_current());
    }

private:
    bool _threadWasAttached;

    bool mono_thread_is_attached()
    {
        return mono_domain_get() != NULL;
    }
};

#if _DEBUG
inline void il2cpp_codegen_check_marshalling_allocations()
{
}

inline void il2cpp_codegen_clear_all_tracked_marshalling_allocations()
{
}

#endif

inline void NullCheck(void* this_ptr)
{
    if (this_ptr != NULL)
        return;

    mono_raise_exception(mono_get_exception_null_reference());
    il2cpp_codegen_no_return();
}

inline void DivideByZeroCheck(int64_t denominator)
{
    if (denominator != 0)
        return;

    mono_raise_exception(mono_get_exception_divide_by_zero());
    il2cpp_codegen_no_return();
}

inline void Initobj(MonoClass* type, void* data)
{
    mono_unity_object_init(data, type);
}

inline bool MethodIsStatic(const MethodInfo* method)
{
    return mono_unity_method_is_static((MonoMethod*)method);
}

inline bool MethodHasParameters(const MethodInfo* method)
{
    return mono_signature_get_param_count(mono_method_signature((MonoMethod*)method)) != 0;
}

//#define IL2CPP_RUNTIME_CLASS_INIT(klass) do { if((klass)->has_cctor && !(klass)->cctor_finished) il2cpp::vm::Runtime::ClassInit ((klass)); } while (0)
#define IL2CPP_RUNTIME_CLASS_INIT(klass) RuntimeInit(klass)

inline void* il2cpp_codegen_mono_class_rgctx(MonoClass* klass, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion)
{
    return il2cpp_mono_class_rgctx(klass, rgctxType, rgctxIndex, useSharedVersion);
}

inline void* il2cpp_codegen_mono_method_rgctx(MonoMethod* method, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion)
{
    return il2cpp_mono_method_rgctx(method, rgctxType, rgctxIndex, useSharedVersion);
}

inline void ArrayElementTypeCheck(Il2CppCodeGenArray* array, void* value)
{
    if (!value)
        return;

    MonoClass *aclass = mono_unity_array_get_class((MonoArray*)array);
    MonoClass *eclass = mono_unity_class_get_element_class(aclass);
    MonoClass *oclass = mono_unity_object_get_class((MonoObject*)value);

    if (!mono_class_is_assignable_from(eclass, oclass))
        mono_raise_exception(mono_get_exception_array_type_mismatch());
}

inline const MethodInfo* GetVirtualMethodInfo(Il2CppCodeGenObject* pThis, MonoMethod* slot)
{
    if (!pThis)
        mono_raise_exception(mono_get_exception_null_reference());

    return (MethodInfo*)mono_object_get_virtual_method((MonoObject*)pThis, slot);
}

inline const MethodInfo* GetInterfaceMethodInfo(Il2CppCodeGenObject* pThis, MonoMethod *slot, MonoClass* declaringInterface)
{
    if (!pThis)
        mono_raise_exception(mono_get_exception_null_reference());

    return (MethodInfo*)mono_object_get_virtual_method((MonoObject*)pThis, slot);
}

#if IL2CPP_COMPILER_MSVC
#define STDCALL __stdcall
#define CDECL __cdecl
#define DEFAULT_CALL STDCALL
#else
#define STDCALL
#define CDECL
#define DEFAULT_CALL
#endif

#if defined(__ARMCC_VERSION)
inline double bankers_round(double x)
{
    return __builtin_round(x);
}

inline float bankers_roundf(float x)
{
    return __builtin_roundf(x);
}

#else
inline double bankers_round(double x)
{
    double integerPart;
    if (x >= 0.0)
    {
        if (modf(x, &integerPart) == 0.5)
            return (int64_t)integerPart % 2 == 0 ? integerPart : integerPart + 1.0;
        return floor(x + 0.5);
    }
    else
    {
        if (modf(x, &integerPart) == -0.5)
            return (int64_t)integerPart % 2 == 0 ? integerPart : integerPart - 1.0;
        return ceil(x - 0.5);
    }
}

inline float bankers_roundf(float x)
{
    double integerPart;
    if (x >= 0.0f)
    {
        if (modf(x, &integerPart) == 0.5)
            return (int64_t)integerPart % 2 == 0 ? (float)integerPart : (float)integerPart + 1.0f;
        return floorf(x + 0.5f);
    }
    else
    {
        if (modf(x, &integerPart) == -0.5)
            return (int64_t)integerPart % 2 == 0 ? (float)integerPart : (float)integerPart - 1.0f;
        return ceilf(x - 0.5f);
    }
}

#endif

// returns true if overflow occurs
inline bool il2cpp_codegen_check_mul_overflow_i64(int64_t a, int64_t b, int64_t imin, int64_t imax)
{
    // TODO: use a better algorithm without division
    uint64_t ua = (uint64_t)llabs(a);
    uint64_t ub = (uint64_t)llabs(b);

    uint64_t c;
    if ((a > 0 && b > 0) || (a <= 0 && b <= 0))
        c = (uint64_t)llabs(imax);
    else
        c = (uint64_t)llabs(imin);

    return ua != 0 && ub > c / ua;
}

inline void il2cpp_codegen_memory_barrier()
{
    mono_unity_memory_barrier();
}

template<typename T>
inline T VolatileRead(T* location)
{
    T result = *location;
    il2cpp_codegen_memory_barrier();
    return result;
}

template<typename T>
inline void VolatileWrite(T* location, T value)
{
    il2cpp_codegen_memory_barrier();
    *location = value;
}

inline void il2cpp_codegen_initialize_method(uint32_t index)
{
    il2cpp_mono_initialize_method_metadata(index);
}

inline bool il2cpp_codegen_type_implements_virtual_method(MonoClass* type, MonoMethod *slot)
{
    return mono_unity_method_get_class(slot) == type;
}

inline Il2CppCodeGenMethodBase* il2cpp_codegen_get_method_object(const MethodInfo* methodInfo)
{
    MonoMethod *method = (MonoMethod*)methodInfo;

    if (unity_mono_method_is_inflated(method))
        method = mono_unity_method_get_generic_definition(method);

    return (Il2CppCodeGenMethodBase*)mono_unity_method_get_object(method);
}

inline Il2CppCodeGenMethodBase* il2cpp_codegen_get_method_object(const MonoMethod* methodInfo)
{
    return il2cpp_codegen_get_method_object((const MethodInfo*)methodInfo);
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, const MethodInfo*);
    MonoString* assemblyQualifiedTypeName = mono_unity_string_append_assembly_name_if_necessary((MonoString*)typeName, assemblyName);

    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, NULL);
    return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, bool, const MethodInfo*);
    MonoString* assemblyQualifiedTypeName = mono_unity_string_append_assembly_name_if_necessary((MonoString*)typeName, assemblyName);

    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, NULL);
    return type;
}

inline Il2CppCodeGenType* il2cpp_codegen_get_type(Il2CppMethodPointer getTypeFunction, Il2CppCodeGenString* typeName, bool throwOnError, bool ignoreCase, const char* assemblyName)
{
    typedef Il2CppCodeGenType* (*getTypeFuncType)(Il2CppCodeGenObject*, Il2CppCodeGenString*, bool, bool, const MethodInfo*);
    MonoString* assemblyQualifiedTypeName = mono_unity_string_append_assembly_name_if_necessary((MonoString*)typeName, assemblyName);

    // Try to find the type using a hint about about calling assembly. If it is not found, fall back to calling GetType without the hint.
    Il2CppCodeGenType* type = ((getTypeFuncType)getTypeFunction)(NULL, (Il2CppCodeGenString*)assemblyQualifiedTypeName, throwOnError, ignoreCase, NULL);
    if (type == NULL)
        return ((getTypeFuncType)getTypeFunction)(NULL, typeName, throwOnError, ignoreCase, NULL);
    return type;
}

inline Il2CppCodeGenAssembly* il2cpp_codegen_get_executing_assembly(const MonoMethod* methodInfo)
{
    return (Il2CppCodeGenAssembly*)mono_assembly_get_object(mono_domain_get(), mono_unity_class_get_assembly(mono_unity_method_get_class((MonoMethod*)methodInfo)));
}

// Atomic

inline void* il2cpp_codegen_atomic_compare_exchange_pointer(void* volatile* dest, void* exchange, void* comparand)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

template<typename T>
inline T* il2cpp_codegen_atomic_compare_exchange_pointer(T* volatile* dest, T* newValue, T* oldValue)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

// COM

inline void il2cpp_codegen_com_marshal_variant(Il2CppObject* obj, Il2CppVariant* variant)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline Il2CppObject* il2cpp_codegen_com_marshal_variant_result(Il2CppVariant* variant)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_com_destroy_variant(Il2CppVariant* variant)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array(Il2CppChar type, Il2CppArray* managedArray)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppArray* il2cpp_codegen_com_marshal_safe_array_result(Il2CppChar variantType, MonoClass* type, Il2CppSafeArray* safeArray)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppSafeArray* il2cpp_codegen_com_marshal_safe_array_bstring(Il2CppArray* managedArray)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppArray* il2cpp_codegen_com_marshal_safe_array_bstring_result(MonoClass* type, Il2CppSafeArray* safeArray)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_com_destroy_safe_array(Il2CppSafeArray* safeArray)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline void il2cpp_codegen_com_create_instance(const Il2CppGuid& clsid, Il2CppIUnknown** identity)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline void il2cpp_codegen_com_register_rcw(Il2CppComObject* rcw)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_from_iunknown(Il2CppIUnknown* unknown, Il2CppClass* fallbackClass)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_from_iinspectable(Il2CppIInspectable* unknown, Il2CppClass* fallbackClass)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

template<typename T>
inline T* il2cpp_codegen_com_get_or_create_rcw_for_sealed_class(Il2CppIUnknown* unknown, Il2CppClass* objectClass)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_il2cpp_com_object_cleanup(Il2CppComObject* rcw)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

template<typename InterfaceType>
inline InterfaceType* il2cpp_codegen_com_get_or_create_ccw(Il2CppObject* obj)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppCodeGenIntPtr il2cpp_codegen_com_get_iunknown_for_object(Il2CppObject* obj)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return Il2CppCodeGenIntPtr();
}

inline void il2cpp_codegen_com_raise_exception(il2cpp_hresult_t hr)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline void il2cpp_codegen_com_raise_exception_if_failed(il2cpp_hresult_t hr, bool defaultToCOMException)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline Il2CppException* il2cpp_codegen_com_get_exception(il2cpp_hresult_t hr, bool defaultToCOMException)
{
    NOT_IMPLEMENTED("Not implemented yet.");
}

inline Il2CppException* il2cpp_codegen_com_get_exception_for_invalid_iproperty_cast(Il2CppObject* value, const char* a, const char* b)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
}

inline Il2CppIActivationFactory* il2cpp_codegen_windows_runtime_get_activation_factory(const il2cpp::utils::StringView<Il2CppNativeChar>& runtimeClassName)
{
    NOT_IMPLEMENTED("COM is not yet supported with the libmonoruntime backend.");
    return NULL;
}

// delegate

inline Il2CppAsyncResult* il2cpp_codegen_delegate_begin_invoke(Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, Il2CppObject* state)
{
    return il2cpp_mono_delegate_begin_invoke(delegate, params, asyncCallback, state);
}

inline Il2CppObject* il2cpp_codegen_delegate_end_invoke(Il2CppAsyncResult* asyncResult, void **out_args)
{
    return il2cpp_mono_delegate_end_invoke(asyncResult, out_args);
}

inline const Il2CppGenericInst* il2cpp_codegen_get_generic_class_inst(Il2CppClass* genericClass)
{
    NOT_IMPLEMENTED("Windows runtime is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline Il2CppClass* il2cpp_codegen_inflate_generic_class(Il2CppClass* genericClassDefinition, const Il2CppGenericInst* genericInst)
{
    //return il2cpp::vm::Class::GetInflatedGenericInstanceClass(genericClassDefinition, genericInst);
    NOT_IMPLEMENTED("Windows runtime is not yet supported with the libmonoruntime backend.");
    return NULL;
}

inline void il2cpp_codegen_write_to_stdout(const char* str)
{
    il2cpp::utils::Output::WriteToStdout(str);
}

inline void il2cpp_codegen_write_to_stderr(const char* str)
{
    il2cpp::utils::Output::WriteToStderr(str);
}

inline REAL_NORETURN void il2cpp_codegen_abort()
{
    il2cpp::utils::Runtime::Abort();
    il2cpp_codegen_no_return();
}

inline std::string il2cpp_codegen_format_exception(const Il2CppException* ex)
{
    return il2cpp::utils::Exception::FormatException(ex);
}

inline int32_t il2cpp_codegen_abs(uint32_t value)
{
    return abs(static_cast<int32_t>(value));
}

inline int32_t il2cpp_codegen_abs(int32_t value)
{
    return abs(value);
}

inline int64_t il2cpp_codegen_abs(uint64_t value)
{
    return llabs(static_cast<int64_t>(value));
}

inline int64_t il2cpp_codegen_abs(int64_t value)
{
    return llabs(value);
}

// Exception support macros
#define IL2CPP_LEAVE(Offset, Target) \
    __leave_target = Offset; \
    goto Target;

#define IL2CPP_END_FINALLY(Id) \
    goto __CLEANUP_ ## Id;

#define IL2CPP_CLEANUP(Id) \
    __CLEANUP_ ## Id:

#define IL2CPP_RETHROW_IF_UNHANDLED(ExcType) \
    if(__last_unhandled_exception) { \
        ExcType _tmp_exception_local = __last_unhandled_exception; \
        __last_unhandled_exception = 0; \
        il2cpp_codegen_raise_exception(_tmp_exception_local); \
        }

#define IL2CPP_JUMP_TBL(Offset, Target) \
    if(__leave_target == Offset) { \
        __leave_target = 0; \
        goto Target; \
        }

#define IL2CPP_END_CLEANUP(Offset, Target) \
    if(__leave_target == Offset) \
        goto Target;

#define IL2CPP_RAISE_MANAGED_EXCEPTION(message) \
    do {\
        il2cpp_codegen_raise_exception((Il2CppCodeGenException*)message);\
        il2cpp_codegen_no_return();\
    } while (0)


template<typename T>
inline void Il2CppCodeGenWriteBarrier(T** targetAddress, T* object)
{
    // TODO
}

inline MonoAssembly* il2cpp_codegen_mono_corlib()
{
    return mono_unity_assembly_get_mscorlib();
}

inline MonoClass* il2cpp_codegen_mono_class(const char* imageName, uint32_t classToken)
{
    return mono_class_get(mono_assembly_get_image(il2cpp_mono_assembly_from_name(imageName)), classToken);
}

inline MonoClass* il2cpp_codegen_mono_class(MonoAssembly* assembly, uint32_t classToken)
{
    return mono_class_get(mono_assembly_get_image(assembly), classToken);
}

inline MonoMethod* il2cpp_codegen_mono_method(const char* imageName, uint32_t methodToken)
{
    return mono_get_method(mono_assembly_get_image(il2cpp_mono_assembly_from_name(imageName)), methodToken, NULL);
}

inline MonoMethod* il2cpp_codegen_mono_method(MonoAssembly* assembly, uint32_t methodToken)
{
    return mono_get_method(mono_assembly_get_image(assembly), methodToken, NULL);
}

inline void il2cpp_codegen_mono_set_static_field(MonoClass* klass, MonoClassField* field, void* value)
{
    mono_field_static_set_value(mono_class_vtable(mono_domain_get(), klass), field, value);
}

inline void il2cpp_codegen_mono_get_static_field(MonoClass* klass, MonoClassField* field, void* value)
{
    mono_field_static_get_value(mono_class_vtable(mono_domain_get(), klass), field, value);
}

inline void* il2cpp_codegen_mono_get_static_field_address(MonoClass* klass, MonoClassField* field)
{
    return mono_unity_get_field_address(NULL, mono_class_vtable(mono_domain_get(), klass), field);
}

inline MonoClassField* il2cpp_codegen_mono_class_get_field(MonoClass* klass, uint32_t fieldToken)
{
    return mono_class_get_field(klass, fieldToken);
}

inline Il2CppMethodPointer il2cpp_codegen_get_method_pointer(const MethodInfo* methodInfo)
{
    MonoError unused;
    il2cpp_mono_method_initialize_function_pointers((MonoMethod*)methodInfo, &unused);
    return (Il2CppMethodPointer)mono_unity_method_get_method_pointer((MonoMethod*)methodInfo);
}

inline MonoType* il2cpp_codegen_method_return_type(const MonoMethod* method)
{
    return mono_signature_get_return_type(mono_method_signature(const_cast<MonoMethod*>(method)));
}

inline MonoType* il2cpp_codegen_method_return_type(const MethodInfo* method)
{
    return il2cpp_codegen_method_return_type((const MonoMethod*)method);
}

template<class T>
T il2cpp_mono_cast_nullable_method_param(const MonoMethod *method, int index, void *value)
{
    if (value)
        return *((T*)value);

    T retVal;
    MonoClass *klass = mono_unity_signature_get_class_for_param(mono_method_signature((MonoMethod*)method), index);
    mono_nullable_init((uint8_t*)&retVal, NULL, klass);
    return retVal;
}

inline const MethodInfo* il2cpp_codegen_vtable_slot_method(const MonoClass* klass, MonoMethod* slot)
{
    return (const MethodInfo*)slot;
}

inline Il2CppMethodPointer il2cpp_codegen_vtable_slot_method_pointer(const MonoClass* klass, MonoMethod* slot)
{
    MonoError unused;
    il2cpp_mono_method_initialize_function_pointers(slot, &unused);
    return (Il2CppMethodPointer)mono_unity_method_get_method_pointer(slot);
}
