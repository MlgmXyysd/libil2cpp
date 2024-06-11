#pragma once

#include "il2cpp-config.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <string>
#include <math.h>

#include "il2cpp-api.h"
#include "object-internals.h"
#include "class-internals.h"

#include "utils/RegisterRuntimeInitializeAndCleanup.h"

#include "metadata/GenericMethod.h"
#include "vm/Array.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Exception.h"
#include "vm/InternalCalls.h"
#include "vm/LastError.h"
#include "vm/MarshalAlloc.h"
#include "vm/MetadataCache.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/PlatformInvoke.h"
#include "vm/Profiler.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Thread.h"

#define NO_UNUSED_WARNING(expr) (void)(expr)
#ifdef _MSC_VER
#define IL2CPP_DEBUG_BREAK() __debugbreak()
#else
#define IL2CPP_DEBUG_BREAK()
#endif

struct ProfilerMethodSentry
{
	ProfilerMethodSentry(MethodInfo* method) : m_method(method)
	{
		il2cpp::vm::Profiler::MethodEnter (m_method);
	}

	~ProfilerMethodSentry()
	{
		il2cpp::vm::Profiler::MethodExit (m_method);
	}

private:
	MethodInfo* m_method;
};

struct StackTraceSentry
{
	StackTraceSentry(MethodInfo* method) : m_method(method)
	{
		Il2CppStackFrameInfo frame_info;

		frame_info.method = method;

#if IL2CPP_DEBUGGER_ENABLED
		frame_info.id = -1;
		frame_info.il_offset = 0;
		frame_info.type = FRAME_TYPE_MANAGED;
#endif
		il2cpp::vm::Thread::PushFrame (frame_info);
	}

#if IL2CPP_DEBUGGER_ENABLED
	StackTraceSentry(MethodInfo* method, void* this_ptr, void **params, int32_t params_count, void **locals, int32_t locals_count) : m_method(method)
	{
		Il2CppStackFrameInfo frame_info;

		frame_info.id = -1;
		frame_info.this_ptr = this_ptr;
		frame_info.method = method;
		frame_info.il_offset = 0;
		frame_info.type = FRAME_TYPE_MANAGED;
		frame_info.params = params;
		frame_info.params_count = params_count;
		frame_info.locals = locals;
		frame_info.locals_count = locals_count;
		il2cpp::vm::Thread::PushFrame (frame_info);
	}
#endif

	~StackTraceSentry()
	{
		il2cpp::vm::Thread::PopFrame ();
	}

private:
	MethodInfo* m_method;
};

// declare extern rather than include "gc/gc-internal.h" until WebGL includes Boehm headers
extern void* il2cpp_gc_alloc_fixed (size_t size, void *descr);

// TODO: This file should contain all the functions and type declarations needed for the generated code.
// Hopefully, we stop including everything in the generated code and know exactly what dependencies we have.
// Note that all parameter and return types should match the generated types not the runtime types.

inline void RegisterAssembly(Il2CppAssembly* assembly)
{
	il2cpp::vm::Assembly::Register (assembly);
}

template <size_t typeCount>
inline void RegisterGenericTypes (Il2CppGenericClass* (&types)[typeCount])
{
	il2cpp::vm::MetadataCache::RegisterGenericTypes(types, typeCount);
}

template <size_t typeCount>
inline void RegisterSZArrays(TypeInfo* (&types)[typeCount])
{
	il2cpp::vm::MetadataCache::RegisterSZArrays(types, typeCount);
}

template <size_t typeCount>
inline void RegisterArrays(TypeInfo* (&types)[typeCount])
{
	il2cpp::vm::MetadataCache::RegisterArrays(types, typeCount);
}

template <size_t methodCount>
inline void RegisterGenericMethods (Il2CppGenericMethod* (&methods)[methodCount])
{
	il2cpp::vm::MetadataCache::RegisterGenericMethods(methods, methodCount);
}

template <size_t instCount>
inline void RegisterGenericInsts (Il2CppGenericInst* (&instData)[instCount])
{
	il2cpp::vm::MetadataCache::RegisterGenericInsts (instData, instCount);
}

template <size_t methodCount>
inline void RegisterMethodTable (Il2CppGenericMethodFunctions (&methods)[methodCount])
{
	il2cpp::vm::MetadataCache::RegisterMethodTable (methods, methodCount);
}

template <size_t methodCount>
inline void RegisterMethodPointers (methodPointerType (&methods)[methodCount])
{
	il2cpp::vm::MetadataCache::RegisterMethodPointers (methods, methodCount);
}

template <size_t methodCount>
inline void RegisterInvokerPointers (InvokerMethod (&methods)[methodCount])
{
	il2cpp::vm::MetadataCache::RegisterInvokerPointers (methods, methodCount);
}

#include "GeneratedCodeGen.h"

// type registration

static void* il2cpp_codegen_get_thread_static_data (TypeInfo* klass)
{
	return il2cpp::vm::Thread::GetThreadStaticData (klass->thread_static_fields_offset);
}

static Il2CppCodeGenString* il2cpp_codegen_string_new_wrapper (const char* str)
{
	return (Il2CppCodeGenString*)il2cpp::vm::String::NewWrapper (str);
}

static Il2CppCodeGenString* il2cpp_codegen_ldstr (const uint16_t* chars, int32_t length)
{
	return (Il2CppCodeGenString*)il2cpp::vm::String::Load (chars, length);
}

static Il2CppCodeGenType* il2cpp_codegen_type_get_object (const Il2CppType* type)
{
	return (Il2CppCodeGenType*)il2cpp::vm::Reflection::GetTypeObject (type);
}

NORETURN static void il2cpp_codegen_raise_exception (Il2CppCodeGenException *ex)
{
	il2cpp::vm::Exception::Raise ((Il2CppException*)ex);
#if __has_builtin(__builtin_unreachable)
	__builtin_unreachable();
#endif
}

static void il2cpp_codegen_raise_execution_engine_exception_if_method_is_not_found(const MethodInfo* method)
{
	il2cpp::vm::Runtime::RaiseExecutionEngineExceptionIfMethodIsNotFound(method);
}

static Il2CppCodeGenException* il2cpp_codegen_get_argument_exception(const char* param, const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArgumentException(param, msg);
}

static Il2CppCodeGenException* il2cpp_codegen_get_argument_null_exception(const char* param)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArgumentNullException(param);
}

static Il2CppCodeGenException* il2cpp_codegen_get_overflow_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetOverflowException("Arithmetic operation resulted in an overflow.");
}

static Il2CppCodeGenException* il2cpp_codegen_get_not_supported_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetNotSupportedException(msg);
}

static Il2CppCodeGenException* il2cpp_codegen_get_array_type_mismatch_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetArrayTypeMismatchException();
}

static Il2CppCodeGenException* il2cpp_codegen_get_marshal_directive_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMarshalDirectiveException(msg);
}

static Il2CppCodeGenException* il2cpp_codegen_get_missing_method_exception(const char* msg)
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMissingMethodException(msg);
}

static Il2CppCodeGenException* il2cpp_codegen_get_maximum_nested_generics_exception()
{
	return (Il2CppCodeGenException*)il2cpp::vm::Exception::GetMaxmimumNestedGenericsException();
}

// OpCode.IsInst

static Il2CppCodeGenObject* IsInst(Il2CppCodeGenObject *obj, TypeInfo* targetType)
{
	return (Il2CppCodeGenObject*)il2cpp::vm::Object::IsInst((Il2CppObject*)obj, targetType);
}

// OpCode.Castclass

static Il2CppCodeGenObject* Castclass(Il2CppCodeGenObject *obj, TypeInfo* targetType)
{
	if (!obj)
		return NULL;

	Il2CppObject* result = il2cpp::vm::Object::IsInst((Il2CppObject*)obj, targetType);
	if (result)
		return (Il2CppCodeGenObject*)result;
	
	il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(((Il2CppObject*)obj)->klass->element_class, targetType).c_str()));
	return NULL;
}

// OpCode.Box

static inline Il2CppCodeGenObject* Box(TypeInfo* type, void* data)
{
	return (Il2CppCodeGenObject*)il2cpp::vm::Object::Box(type, data);
}

// OpCode.UnBox

static inline void* UnBox(Il2CppCodeGenObject* obj)
{
	if (!obj){
		il2cpp::vm::Exception::RaiseNullReferenceException ();
		#if __has_builtin(__builtin_unreachable)
            __builtin_unreachable();
        #endif
	}

	return il2cpp::vm::Object::Unbox((Il2CppObject*)obj);
}

static inline void* UnBox(Il2CppCodeGenObject* obj, TypeInfo* klass)
{
	if (!obj){
		il2cpp::vm::Exception::RaiseNullReferenceException ();
		#if __has_builtin(__builtin_unreachable)
            __builtin_unreachable();
        #endif
	}

	Il2CppObject* obj2 = (Il2CppObject*)obj;
	if (obj2->klass->element_class != klass->element_class)
		il2cpp::vm::Exception::Raise ((Il2CppException*)il2cpp::vm::Exception::GetInvalidCastException (il2cpp::vm::Exception::FormatInvalidCastException(obj2->klass->element_class, klass->element_class).c_str()));

	return il2cpp::vm::Object::Unbox(obj2);
}

static inline void UnBoxNullable(Il2CppCodeGenObject* obj, TypeInfo* klass, void* storage)
{
	return il2cpp::vm::Object::UnboxNullable(reinterpret_cast<Il2CppObject*>(obj), klass, storage);
}

#include "GeneratedVirtualInvokers.h"
#include "GeneratedInterfaceInvokers.h"
#include "GeneratedGenericVirtualInvokers.h"
#include "GeneratedGenericInterfaceInvokers.h"

// OpCode.Ldtoken

static Il2CppCodeGenRuntimeTypeHandle LoadTypeToken(const Il2CppType* ptr)
{
	Il2CppCodeGenRuntimeTypeHandle handle = { { (void*)ptr } } ;
	return handle;
}

static Il2CppCodeGenRuntimeFieldHandle LoadFieldToken(void* ptr)
{
	Il2CppCodeGenRuntimeFieldHandle handle = { { (void*)ptr } } ;
	return handle;
}

static Il2CppCodeGenRuntimeArgumentHandle LoadArgList()
{
	Il2CppCodeGenRuntimeArgumentHandle handle = { { NULL } } ;
	assert(false && "__arglist usage not supported.");
	return handle;
}

static Il2CppCodeGenRuntimeMethodHandle LoadMethodToken(void* ptr)
{
	Il2CppCodeGenRuntimeMethodHandle handle = { { (void*)ptr } } ;
	return handle;
}

inline TypeInfo* InitializedTypeInfo (TypeInfo* typeInfo)
{
	il2cpp::vm::Class::Init (typeInfo);
	return typeInfo;
}

inline MethodInfo* il2cpp_codegen_genericmethod_get_method (Il2CppGenericMethod* genericMethod)
{
	return il2cpp::metadata::GenericMethod::GetMethod (genericMethod);
}

static inline TypeInfo* il2cpp_codegen_class_from_type (const Il2CppType *type)
{
	return InitializedTypeInfo (il2cpp::vm::Class::FromIl2CppType (type));
}

inline methodPointerType il2cpp_codegen_resolve_icall (const char* name);

static void* InterlockedCompareExchangeImplRef (void** location, void* value, void* comparand)
{
	typedef void* (*InterlockedCompareExchangeImpl_ftn) (void** location, void* value, void* comparand);
	static InterlockedCompareExchangeImpl_ftn _il2cpp_icall_func;
	if (!_il2cpp_icall_func)
		_il2cpp_icall_func = (InterlockedCompareExchangeImpl_ftn)il2cpp_codegen_resolve_icall ("System.Threading.Interlocked::CompareExchange(T&,T,T)");
	return _il2cpp_icall_func(location, value, comparand);
}

static void* InterlockedExchangeImplRef (void** location, void* value)
{
	typedef void* (*InterlockedExchangeImpl_ftn) (void** location, void* value);
	static InterlockedExchangeImpl_ftn _il2cpp_icall_func;
	if ( !_il2cpp_icall_func )
		_il2cpp_icall_func = (InterlockedExchangeImpl_ftn)il2cpp_codegen_resolve_icall ("System.Threading.Interlocked::Exchange(T&,T,T)");
	return _il2cpp_icall_func (location, value);
}

template<typename T>
static inline T InterlockedCompareExchangeImpl (T* location, T value, T comparand)
{
	return (T)InterlockedCompareExchangeImplRef ((void**)location, value, comparand);
}

template<typename T>
static inline T InterlockedExchangeImpl (T* location, T value)
{
	return (T)InterlockedExchangeImplRef ((void**)location, value);
}

static inline void ArrayGetGenericValueImpl (Il2CppCodeGenArray* __this, int32_t pos, void* value){
	memcpy(value, ((uint8_t*)__this) + sizeof(Il2CppCodeGenArray) + pos*__this->_typeInfo->element_size, __this->_typeInfo->element_size);
}

static inline void ArraySetGenericValueImpl (Il2CppCodeGenArray * __this, int32_t pos, void* value){
	memcpy(((uint8_t*)__this) + sizeof(Il2CppCodeGenArray) + pos*__this->_typeInfo->element_size, value, __this->_typeInfo->element_size);
}

static inline Il2CppCodeGenArray* SZArrayNew (TypeInfo* arrayType, uint32_t length)
{
	il2cpp::vm::Class::Init (arrayType);
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::NewSpecific (arrayType, length);
}

static inline Il2CppCodeGenArray* GenArrayNew2 (TypeInfo* arrayType, uint32_t length1, uint32_t length2)
{
	il2cpp::vm::Class::Init (arrayType);
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::New2 (arrayType, length1, length2);
}

static inline Il2CppCodeGenArray* GenArrayNew3 (TypeInfo* arrayType, uint32_t length1, uint32_t length2, uint32_t length3)
{
	il2cpp::vm::Class::Init (arrayType);
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::New3 (arrayType, length1, length2, length3);
}

static inline Il2CppCodeGenArray* GenArrayNew4(TypeInfo* arrayType, uint32_t length1, uint32_t length2, uint32_t length3, uint32_t length4)
{
	il2cpp::vm::Class::Init(arrayType);
	return (Il2CppCodeGenArray*)il2cpp::vm::Array::New4(arrayType, length1, length2, length3, length4);
}

static inline void* SZArrayLdElema (Il2CppCodeGenArray* arr, uint32_t index)
{
	TypeInfo* arrayKlass = arr->_typeInfo;
	return (void*)((((uint8_t*)(arr)) + sizeof (Il2CppCodeGenArray)) + (arrayKlass->element_size * (index)));
}

static inline uint8_t* GenArrayAddress2 (Il2CppCodeGenArray* a, uint32_t length1, uint32_t length2)
{
	size_t size = a->_typeInfo->element_size;
	return (((uint8_t*)a) + sizeof(Il2CppCodeGenArray) + a->bounds[1].length * size * (length1) + size * (length2));
}

#define GenArrayGet2(a, length1, length2, type) \
		*(type*)GenArrayAddress2 (a, length1, length2)

#define GenArraySet2(a, length1, length2, value, type) \
	do { \
		*(type*)GenArrayAddress2 (a, length1, length2) = value; \
	} while (0)

static inline uint8_t* GenArrayAddress3 (Il2CppCodeGenArray* a, uint32_t length1, uint32_t length2, uint32_t length3)
{
	size_t size = a->_typeInfo->element_size;
	return (((uint8_t*)a) + sizeof(Il2CppCodeGenArray) + 
		a->bounds[1].length * a->bounds[2].length * size * (length1) + 
		a->bounds[2].length * size * (length2) + 
		size * (length3));
}

#define GenArrayGet3(a, length1, length2, length3, type) \
		*(type*)GenArrayAddress3 (a, length1, length2, length3)

#define GenArraySet3(a, length1, length2, length3, value, type) \
	do { \
		*(type*)GenArrayAddress3 (a, length1, length2, length3) = value; \
	} while (0)

static inline uint8_t* GenArrayAddress4(Il2CppCodeGenArray* a, uint32_t length1, uint32_t length2, uint32_t length3, uint32_t length4)
{
	size_t size = a->_typeInfo->element_size;
	return (((uint8_t*)a) + sizeof(Il2CppCodeGenArray) +
		a->bounds[1].length * a->bounds[2].length * a->bounds[3].length * size * (length1) +
		a->bounds[2].length * a->bounds[3].length * size * (length2) +
		a->bounds[3].length * size * (length3) +
		size * (length4));
}

#define GenArrayGet4(a, length1, length2, length3, length4, type) \
		*(type*)GenArrayAddress4 (a, length1, length2, length3, length4)

#define GenArraySet4(a, length1, length2, length3, length4, value, type) \
	do { \
		*(type*)GenArrayAddress4 (a, length1, length2, length3, length4) = value; \
		} while (0)

#define IL2CPP_ARRAY_BOUNDS_CHECK(a,index) \
	do { \
		if (index < 0 || index >= (a)->max_length) il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetIndexOutOfRangeException()); \
	} while (0)

inline bool il2cpp_class_init (TypeInfo *klass)
{
	return il2cpp::vm::Class::Init (klass);
}

inline int32_t il2cpp_class_interface_offset (const TypeInfo *klass, TypeInfo *itf)
{
	return il2cpp::vm::Class::GetInterfaceOffset (klass, itf);
}

inline bool il2cpp_codegen_class_is_assignable_from (TypeInfo *klass, TypeInfo *oklass)
{
	return il2cpp::vm::Class::IsAssignableFrom (klass, oklass);
}

inline Il2CppObject* il2cpp_codegen_object_new (TypeInfo *klass)
{
	return il2cpp::vm::Object::New (klass);
}

inline methodPointerType il2cpp_codegen_resolve_icall (const char* name)
{
	return il2cpp::vm::InternalCalls::Resolve (name);
}

template <typename FunctionPointerType>
inline FunctionPointerType il2cpp_codegen_resolve_pinvoke(const char* nativeDynamicLibrary, const char* entryPoint, Il2CppCallConvention callingConvention, Il2CppCharSet charSet, int parameterSize, bool isNoMangle)
{
	const PInvokeArguments pinvokeArgs = 
	{
		nativeDynamicLibrary,
		entryPoint,
		callingConvention,
		charSet,
		parameterSize,
		isNoMangle
	};

	return reinterpret_cast<FunctionPointerType>(il2cpp::vm::PlatformInvoke::Resolve(pinvokeArgs));
}

template <typename T>
inline T* il2cpp_codegen_marshal_allocate()
{
	return static_cast<T*>(il2cpp::vm::MarshalAlloc::Allocate(sizeof(T)));
}

template <typename T>
inline T* il2cpp_codegen_marshal_allocate_array(size_t length)
{
	return static_cast<T*>(il2cpp::vm::MarshalAlloc::Allocate((il2cpp_array_size_t)(sizeof(T) * length)));
}

inline char* il2cpp_codegen_marshal_string(Il2CppCodeGenString* il2CppString)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_string_fixed(Il2CppCodeGenString* il2CppString, char* buffer, int numberOfCharacters)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline uint16_t* il2cpp_codegen_marshal_wstring(Il2CppCodeGenString* il2CppString)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppWString((Il2CppString*)il2CppString);
}

inline void il2cpp_codegen_marshal_wstring_fixed(Il2CppCodeGenString* il2CppString, uint16_t* buffer, int numberOfCharacters)
{
	return il2cpp::vm::PlatformInvoke::MarshalCSharpStringToCppWStringFixed((Il2CppString*)il2CppString, buffer, numberOfCharacters);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_string_result(const char* value)
{
	return (Il2CppCodeGenString*)il2cpp::vm::PlatformInvoke::MarshalCppStringToCSharpStringResult(value);
}

inline Il2CppCodeGenString* il2cpp_codegen_marshal_wstring_result(const uint16_t* value)
{
	return (Il2CppCodeGenString*)il2cpp::vm::PlatformInvoke::MarshalCppWStringToCSharpStringResult((uint16_t*)value);
}

inline char* il2cpp_codegen_marshal_string_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

	return il2cpp::vm::PlatformInvoke::MarshalAllocateStringBuffer<char>(il2cpp::vm::String::GetLength(((Il2CppStringBuilder*)stringBuilder)->str));
}

inline uint16_t* il2cpp_codegen_marshal_wstring_builder(Il2CppCodeGenStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

	return il2cpp::vm::PlatformInvoke::MarshalAllocateStringBuffer<uint16_t>(il2cpp::vm::String::GetLength(((Il2CppStringBuilder*)stringBuilder)->str));
}

inline void il2cpp_codegen_marshal_string_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, char* buffer)
{
	il2cpp::vm::PlatformInvoke::MarshalStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

inline void il2cpp_codegen_marshal_wstring_builder_result(Il2CppCodeGenStringBuilder* stringBuilder, uint16_t* buffer)
{
	il2cpp::vm::PlatformInvoke::MarshalWStringBuilderResult((Il2CppStringBuilder*)stringBuilder, buffer);
}

template <typename ElementType>
inline ElementType* il2cpp_codegen_marshal_array(Il2CppCodeGenArray* a)
{
	return il2cpp::vm::PlatformInvoke::MarshalArray<ElementType>((Il2CppArray*)a);
}

template <typename ElementType>
inline Il2CppArray* il2cpp_codegen_marshal_array_result(TypeInfo* type, ElementType* cppArray, size_t size)
{
	return il2cpp::vm::PlatformInvoke::MarshalArrayResult(type, cppArray, size);
}

inline char* il2cpp_codegen_marshal_char_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	char* nativeArray = il2cpp_codegen_marshal_allocate_array<char>(a->max_length);
	il2cpp::vm::PlatformInvoke::MarshalCharArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_char_array_result(char* a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalCharArrayResult(a, size);
}

inline char** il2cpp_codegen_marshal_string_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	// Mono adds a null terminator on the a string array, so we will do the same.
	char** nativeArray = il2cpp_codegen_marshal_allocate_array<char*>(a->max_length + 1);
	il2cpp::vm::PlatformInvoke::MarshalStringArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline uint16_t** il2cpp_codegen_marshal_wstring_array(Il2CppCodeGenArray* a)
{
	if (a == NULL)
		return NULL;

	// Mono adds a null terminator on the a string array, so we will do the same.
	uint16_t** nativeArray = il2cpp_codegen_marshal_allocate_array<uint16_t*>(a->max_length + 1);
	il2cpp::vm::PlatformInvoke::MarshalWStringArray((Il2CppArray*)a, nativeArray);
	return nativeArray;
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_string_array_result(char** a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalStringArrayResult(a, size);
}

inline Il2CppCodeGenArray* il2cpp_codegen_marshal_wstring_array_result(uint16_t** a, size_t size)
{
	return (Il2CppCodeGenArray*)il2cpp::vm::PlatformInvoke::MarshalWStringArrayResult(a, size);
}

inline void il2cpp_codegen_marshal_free(void* ptr)
{
	il2cpp::vm::PlatformInvoke::MarshalFree(ptr);
}

inline void il2cpp_codegen_marshal_free_string_array(void** a, size_t arrayLength)
{
	il2cpp::vm::PlatformInvoke::MarshalFreeStringArray(a, arrayLength);
}

inline methodPointerType il2cpp_codegen_marshal_delegate(Il2CppCodeGenMulticastDelegate* d)
{
	return (methodPointerType)il2cpp::vm::PlatformInvoke::MarshalDelegate((Il2CppDelegate*)d).m_value;
}

template <typename T>
inline T* il2cpp_codegen_marshal_function_ptr_to_delegate(methodPointerType functionPtr, TypeInfo* delegateType)
{
	return (T*)il2cpp::vm::PlatformInvoke::MarshalFunctionPointerToDelegate(reinterpret_cast<void*>(functionPtr), delegateType);
}

inline void il2cpp_codegen_marshal_store_last_error()
{
	il2cpp::vm::LastError::StoreLastError();

}

inline bool il2cpp_codegen_attach()
{
	if (il2cpp::vm::Thread::Current())
		return false;
	il2cpp::vm::Thread::Attach(il2cpp::vm::Domain::GetRoot());
	return true;
}

inline void il2cpp_codegen_detach()
{
	il2cpp::vm::Thread::Detach(il2cpp::vm::Thread::Current());
}

class il2cpp_native_wrapper_vm_thread_attacher
{
public:
	il2cpp_native_wrapper_vm_thread_attacher()
	{
		_threadWasAttached = il2cpp_codegen_attach();
	}

	~il2cpp_native_wrapper_vm_thread_attacher()
	{
		if (_threadWasAttached)
			il2cpp_codegen_detach();
	}

private:
	bool _threadWasAttached;
};

#if _DEBUG
inline void il2cpp_codegen_check_marshalling_allocations()
{
	if (il2cpp::vm::MarshalAlloc::HasUnfreedAllocations())
		il2cpp::vm::Exception::Raise(il2cpp::vm::Exception::GetInvalidOperationException("Error in marshaling allocation. Some memory has been leaked."));
}

inline void il2cpp_codegen_clear_all_tracked_marshalling_allocations()
{
	il2cpp::vm::MarshalAlloc::ClearAllTrackedAllocations();
}
#endif

inline void NullCheck (void* this_ptr)
{
	if(this_ptr != NULL)
		return;

	il2cpp::vm::Exception::RaiseNullReferenceException ();
	#if __has_builtin(__builtin_unreachable)
    	__builtin_unreachable();
    #endif
}

static inline void Initobj (TypeInfo* type, void* data)
{
	if (type->valuetype)
		memset (data, 0, type->instance_size - sizeof (Il2CppObject));
	else
		*static_cast<Il2CppObject**> (data) = NULL;
}

static inline bool MethodIsStatic(MethodInfo* method)
{
	return !il2cpp::vm::Method::IsInstance(method);
}

#define IL2CPP_CLASS_INIT(klass) do {if(!(klass)->initialized) il2cpp_class_init (klass);} while (0)
#define IL2CPP_RUNTIME_CLASS_INIT(klass) do { if((klass)->has_cctor && !(klass)->cctor_finished) il2cpp::vm::Runtime::ClassInit ((klass)); } while (0)

// generic sharing
#define IL2CPP_RGCTX_DATA(rgctxVar,index) (rgctxVar.data[index].klass)
#define IL2CPP_RGCTX_TYPE(rgctxVar,index) (rgctxVar.data[index].type)
#define IL2CPP_RGCTX_METHOD_INFO(rgctxVar,index) (rgctxVar.data[index].method)

inline void ArrayElementTypeCheck(Il2CppCodeGenArray* array, void* value)
{
	if (value != NULL && !il2cpp_codegen_class_is_assignable_from(((Il2CppArray*)array)->obj.klass->element_class, ((Il2CppObject*)value)->klass))
		il2cpp_codegen_raise_exception(il2cpp_codegen_get_array_type_mismatch_exception());
}

inline MethodInfo* GetVirtualMethodInfo (Il2CppCodeGenObject* pThis, MethodInfo* method)
{
	VirtualInvokeData data = il2cpp::vm::Runtime::GetVirtualInvokeData (method, pThis);
	return data.methodInfo;
}

inline MethodInfo* GetInterfaceMethodInfo(Il2CppCodeGenObject* pThis, MethodInfo* method)
{
	VirtualInvokeData data = il2cpp::vm::Runtime::GetInterfaceInvokeData(method, pThis);
	return data.methodInfo;
}

#if IL2CPP_TARGET_WINDOWS
#define STDCALL __stdcall
#define CDECL __cdecl
#define DEFAULT_CALL STDCALL
#else
#define STDCALL
#define CDECL
#define DEFAULT_CALL
#endif

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE
static inline double round(double x)
{
   return x >= 0.0 ? floor(x + 0.5) : ceil(x - 0.5);
}

static inline float roundf(float x)
{
   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}
#elif defined(__ARMCC_VERSION)
static inline double round(double x)
{
   return __builtin_round(x);
}

static inline float roundf(float x)
{
   return __builtin_roundf(x);
}
#endif

// returns true if overflow occurs
static inline bool il2cpp_codegen_check_mul_overflow_i64(int64_t a, int64_t b, int64_t imin, int64_t imax)
{
	// TODO: use a better algorithm without division
	uint64_t ua = (uint64_t) llabs(a);
	uint64_t ub = (uint64_t) llabs(b);

	uint64_t c;
	if ((a > 0 && b > 0) || (a <= 0 && b <= 0))
		c = (uint64_t) llabs(imax);
	else
		c = (uint64_t) llabs(imin);
	
	return ua != 0 && ub > c / ua;
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
