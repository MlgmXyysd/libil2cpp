#include "il2cpp-config.h"

#include "object-internals.h"
#include "class-internals.h"
#include "PlatformInvoke.h"
#include "Exception.h"
#include "LibraryLoader.h"
#include "MetadataCache.h"
#include "Object.h"
#include "Type.h"
#include "os/LibraryLoader.h"
#include "os/MarshalStringAlloc.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"

#include <stdint.h>
#include <sstream>
#include <cassert>

namespace il2cpp
{
namespace vm
{

void PlatformInvoke::SetFindPluginCallback(Il2CppSetFindPlugInCallback method)
{
	LibraryLoader::SetFindPluginCallback(method);
}

methodPointerType PlatformInvoke::Resolve(const PInvokeArguments& pinvokeArgs)
{
	void* dynamicLibrary = LibraryLoader::LoadLibrary(pinvokeArgs.moduleName);
	if (dynamicLibrary == NULL)
	{
		std::stringstream message;
		message << "Unable to load DLL '" << pinvokeArgs.moduleName << "': The specified module could not be found.";
		Exception::Raise(Exception::GetDllNotFoundException(message.str().c_str()));
	}

	methodPointerType function = os::LibraryLoader::GetFunctionPointer(dynamicLibrary, pinvokeArgs);
	if (function == NULL)
	{
		std::stringstream message;
		message << "Unable to find an entry point named '" << pinvokeArgs.entryPoint << "' in '" << pinvokeArgs.moduleName << "'.";
		Exception::Raise(Exception::GetEntryPointNotFoundException(message.str().c_str()));
	}

	return function;
}

void PlatformInvoke::MarshalFree(void* ptr)
{
	if (ptr != NULL)
		MarshalAlloc::Free(ptr);
}

char* PlatformInvoke::MarshalCSharpStringToCppString(Il2CppString* managedString)
{
	if (managedString == NULL)
		return NULL;

	std::string utf8String = utils::StringUtils::Utf16ToUtf8(managedString->chars);

	char* nativeString = MarshalAllocateStringBuffer<char>(utf8String.size() + 1);
	strcpy(nativeString, utf8String.c_str());

	return nativeString;
}

void PlatformInvoke::MarshalCSharpStringToCppStringFixed(Il2CppString* managedString, char* buffer, int numberOfCharacters)
{
	if (managedString == NULL)
	{
		*buffer = '\0';
	}
	else
	{
		std::string utf8String = utils::StringUtils::Utf16ToUtf8(managedString->chars, numberOfCharacters - 1);
		strcpy(buffer, utf8String.c_str());
	}
}

uint16_t* PlatformInvoke::MarshalCSharpStringToCppWString(Il2CppString* managedString)
{
	if (managedString == NULL)
		return NULL;

	int32_t stringLength = String::GetLength(managedString);
	uint16_t* nativeString = MarshalAllocateStringBuffer<uint16_t>(stringLength + 1);
	for (int32_t i = 0; i < managedString->length; ++i)
		nativeString[i] = managedString->chars[i];
	
	nativeString[managedString->length] = '\0';

	return nativeString;
}

void PlatformInvoke::MarshalCSharpStringToCppWStringFixed(Il2CppString* managedString, uint16_t* buffer, int numberOfCharacters)
{
	if (managedString == NULL)
	{
		*buffer = '\0';
	}
	else
	{
		int32_t stringLength = std::min(String::GetLength(managedString), numberOfCharacters - 1);
		for (int32_t i = 0; i < stringLength; ++i)
			buffer[i] = managedString->chars[i];

		buffer[stringLength] = '\0';
	}
}

uint16_t* PlatformInvoke::MarshalCSharpStringToCppBString(Il2CppString* managedString)
{
	if (managedString == NULL)
		return NULL;

	int32_t stringLength = String::GetLength(managedString);
	uint16_t* stringChars = String::GetChars(managedString);
	uint16_t* bstr;
	const il2cpp_hresult_t hr = os::MarshalStringAlloc::AllocateBStringLength(stringChars, stringLength, &bstr);
	Exception::RaiseIfFailed(hr);
	return bstr;
}

Il2CppString* PlatformInvoke::MarshalCppStringToCSharpStringResult(const char* value)
{
	if (value == NULL)
		return NULL;

	return String::New(value);
}

static int32_t uint16_tStringLength(const uint16_t* value)
{
	if (value == NULL)
		return 0;

	const uint16_t* ptr = value;
	int32_t length = 0;
	while (*ptr)
	{
		ptr++;
		length++;
	}

	return length;
}

Il2CppString* PlatformInvoke::MarshalCppWStringToCSharpStringResult(const uint16_t* value)
{
	if (value == NULL)
		return NULL;

	return String::NewUtf16((uint16_t*)value, uint16_tStringLength(value));
}

Il2CppString* PlatformInvoke::MarshalCppBStringToCSharpStringResult(const uint16_t* value)
{
	if (value == NULL)
		return NULL;

	int32_t length;
	const il2cpp_hresult_t hr = os::MarshalStringAlloc::GetBStringLength((uint16_t*)value, &length);
	Exception::RaiseIfFailed(hr);

	return String::NewUtf16((uint16_t*)value, length);
}

void PlatformInvoke::MarshalFreeBString(const uint16_t* value)
{
	const il2cpp_hresult_t hr = os::MarshalStringAlloc::FreeBString((uint16_t*)value);
	Exception::RaiseIfFailed(hr);
}

char** PlatformInvoke::MarshalAllocateNativeStringArray(size_t size)
{
	return (char**)MarshalAlloc::Allocate(size * sizeof(char*));
}

uint16_t** PlatformInvoke::MarshalAllocateNativeWStringArray(size_t size)
{
	return (uint16_t**)MarshalAlloc::Allocate(size * sizeof(uint16_t*));
}

uint16_t** PlatformInvoke::MarshalAllocateNativeBStringArray(size_t size)
{
	return (uint16_t**)MarshalAlloc::Allocate(size * sizeof(uint16_t*));
}

void PlatformInvoke::MarshalStringArrayOut(char** nativeArray, Il2CppArray* managedArray)
{
	for (il2cpp_array_size_t i = 0; i < managedArray->max_length; ++i)
		il2cpp_array_setref(managedArray, i, MarshalCppStringToCSharpStringResult(nativeArray[i]));
}

void PlatformInvoke::MarshalWStringArrayOut(uint16_t** nativeArray, Il2CppArray* managedArray)
{
	for (il2cpp_array_size_t i = 0; i < managedArray->max_length; ++i)
		il2cpp_array_setref(managedArray, i, MarshalCppWStringToCSharpStringResult(nativeArray[i]));
}

void PlatformInvoke::MarshalBStringArrayOut(uint16_t** nativeArray, Il2CppArray* managedArray)
{
	for (il2cpp_array_size_t i = 0; i < managedArray->max_length; ++i)
		il2cpp_array_setref(managedArray, i, MarshalCppBStringToCSharpStringResult(nativeArray[i]));
}

void PlatformInvoke::MarshalStringArray(Il2CppArray* managedArray, char** nativeArray)
{
	const uint32_t arraySize = Array::GetLength(managedArray);

	for (uint32_t i = 0; i < arraySize; ++i)
	{
		Il2CppString* managedString = il2cpp_array_get(managedArray, Il2CppString*, i);
		nativeArray[i] = MarshalCSharpStringToCppString(managedString);
	}

	nativeArray[arraySize] = NULL;
}

void PlatformInvoke::MarshalWStringArray(Il2CppArray* managedArray, uint16_t** nativeArray)
{
	const uint32_t arraySize = Array::GetLength(managedArray);

	for (uint32_t i = 0; i < arraySize; ++i)
	{
		Il2CppString* managedString = il2cpp_array_get(managedArray, Il2CppString*, i);
		nativeArray[i] = MarshalCSharpStringToCppWString(managedString);
	}

	nativeArray[arraySize] = NULL;
}

void PlatformInvoke::MarshalBStringArray(Il2CppArray* managedArray, uint16_t** nativeArray)
{
	const uint32_t arraySize = Array::GetLength(managedArray);

	for (uint32_t i = 0; i < arraySize; ++i)
	{
		Il2CppString* managedString = il2cpp_array_get(managedArray, Il2CppString*, i);
		nativeArray[i] = MarshalCSharpStringToCppBString(managedString);
	}

	nativeArray[arraySize] = NULL;
}

Il2CppArray* PlatformInvoke::MarshalStringArrayResult(char** nativeArray, size_t size)
{
	if (nativeArray == NULL)
		return NULL;

	Il2CppArray* managedArray = Array::New(il2cpp_defaults.string_class, (il2cpp_array_size_t)size);
	MarshalStringArrayOut(nativeArray, managedArray);

	return managedArray;
}

Il2CppArray* PlatformInvoke::MarshalWStringArrayResult(uint16_t** nativeArray, size_t size)
{
	if (nativeArray == NULL)
		return NULL;

	Il2CppArray* managedArray = Array::New(il2cpp_defaults.string_class, (il2cpp_array_size_t)size);
	MarshalWStringArrayOut(nativeArray, managedArray);

	return managedArray;
}

Il2CppArray* PlatformInvoke::MarshalBStringArrayResult(uint16_t** nativeArray, size_t size)
{
	if (nativeArray == NULL)
		return NULL;

	Il2CppArray* managedArray = Array::New(il2cpp_defaults.string_class, (il2cpp_array_size_t)size);
	MarshalBStringArrayOut(nativeArray, managedArray);

	return managedArray;
}

char* PlatformInvoke::MarshalStringBuilder(Il2CppStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

	size_t stringLength = String::GetLength(stringBuilder->str);

	// not sure if this is necessary but it's better to be safe than sorry
	assert(stringLength >= stringBuilder->length);
	if (stringLength < stringBuilder->length)
		stringLength = stringBuilder->length;

	std::string utf8String = utils::StringUtils::Utf16ToUtf8(stringBuilder->str->chars, stringBuilder->length);
	if (stringLength < utf8String.length())
		stringLength = utf8String.length();

	char* nativeString = MarshalAllocateStringBuffer<char>(stringLength + 1);
	strcpy(nativeString, utf8String.c_str());

	return nativeString;
}

uint16_t* PlatformInvoke::MarshalWStringBuilder(Il2CppStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

	int32_t stringLength = String::GetLength(stringBuilder->str);

	// not sure if this is necessary but it's better to be safe than sorry
	assert(stringLength >= stringBuilder->length);
	if (stringLength < stringBuilder->length)
		stringLength = stringBuilder->length;

	uint16_t* nativeString = MarshalAllocateStringBuffer<uint16_t>(stringLength + 1);
	for (int32_t i = 0; i < stringBuilder->length; ++i)
		nativeString[i] = stringBuilder->str->chars[i];

	nativeString[stringBuilder->length] = '\0';

	return nativeString;
}

void PlatformInvoke::MarshalStringBuilderResult(Il2CppStringBuilder* stringBuilder, char* buffer)
{
	if (stringBuilder == NULL || buffer == NULL)
		return;

	Il2CppString* managedString = MarshalCppStringToCSharpStringResult(buffer);
	stringBuilder->str = managedString;
	stringBuilder->length = String::GetLength(managedString);
}

void PlatformInvoke::MarshalWStringBuilderResult(Il2CppStringBuilder* stringBuilder, uint16_t* buffer)
{
	if (stringBuilder == NULL || buffer == NULL)
		return;

	Il2CppString* managedString = MarshalCppWStringToCSharpStringResult(buffer);
	stringBuilder->str = managedString;
	stringBuilder->length = String::GetLength(managedString);
}

void PlatformInvoke::MarshalFreeStringArray(void** nativeArray, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		MarshalAlloc::Free(nativeArray[i]);

	MarshalAlloc::Free(nativeArray);
}

void PlatformInvoke::MarshalFreeBStringArray(uint16_t** nativeArray, size_t size)
{
	for (size_t i = 0; i < size; ++i)
		os::MarshalStringAlloc::FreeBString(nativeArray[i]);

	MarshalAlloc::Free(nativeArray);
}

Il2CppIntPtr PlatformInvoke::MarshalDelegate(Il2CppDelegate* d)
{
	if (d == NULL)
		return Il2CppIntPtr::Zero;
	
	assert (!d->method->is_inflated);
	assert (d->method->methodDefinition);

	methodPointerType nativeDelegateWrapper = MetadataCache::GetDelegateWrapperNativeToManagedFromIndex (d->method->methodDefinition->delegateWrapperIndex);
	if (nativeDelegateWrapper == NULL)
		vm::Exception::Raise(vm::Exception::GetNotSupportedException("To marshal a manged method, please add an attribute named 'MonoPInvokeCallback' to the method definition."));

	Il2CppIntPtr functionPointer;
	functionPointer.m_value = (void*)nativeDelegateWrapper;
	return functionPointer;
}

Il2CppDelegate* PlatformInvoke::MarshalFunctionPointerToDelegate(void* functionPtr, TypeInfo* delegateType)
{
	Il2CppObject* delegate = il2cpp::vm::Object::New(delegateType);
	methodPointerType nativeFunctionPointer = (methodPointerType)functionPtr;

	const MethodInfo* method = MetadataCache::GetNativeDelegate (nativeFunctionPointer);
	if (method == NULL)
	{
		MethodInfo* newMethod = (MethodInfo*)IL2CPP_CALLOC (1, sizeof (MethodInfo));
		newMethod->method = nativeFunctionPointer;
		newMethod->invoker_method = NULL;
		MetadataCache::AddNativeDelegate(nativeFunctionPointer, newMethod);
		method = newMethod;
	}

	Type::ConstructDelegate((Il2CppDelegate*)delegate, delegate, MetadataCache::GetDelegateWrapperManagedToNativeFromIndex (delegateType->typeDefinition->delegateWrapperFromManagedToNativeIndex), method);

	return (Il2CppDelegate*)delegate;
}

typedef void(*MarshalFunc)(void*, void*);

void PlatformInvoke::MarshalStructToNative(void* managedStructure, void* marshaledStructure, TypeInfo* type)
{
	MarshalFunc marshalFunc = (MarshalFunc)MetadataCache::GetMarshalToNativeFuncFromIndex (type->typeDefinition->marshalingFunctionsIndex);
	marshalFunc(managedStructure, marshaledStructure);
}

void PlatformInvoke::MarshalStructFromNative(void* marshaledStructure, void* managedStructure, TypeInfo* type)
{
	MarshalFunc marshalFunc = (MarshalFunc)MetadataCache::GetMarshalFromNativeFuncFromIndex (type->typeDefinition->marshalingFunctionsIndex);
	marshalFunc(marshaledStructure, managedStructure);
}

bool PlatformInvoke::MarshalFreeStruct(void* marshaledStructure, TypeInfo* type)
{
	typedef void(*CleanupFunc)(void*);

	if (type->typeDefinition == NULL)
		return false;

	CleanupFunc cleanup = (CleanupFunc)MetadataCache::GetMarshalCleanupFuncFromIndex (type->typeDefinition->marshalingFunctionsIndex);

	if (cleanup != NULL)
	{
		cleanup(marshaledStructure);
		return true;
	}

	return false;
}

} /* namespace vm */
} /* namespace il2cpp */
