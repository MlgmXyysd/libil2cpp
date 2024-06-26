#include "il2cpp-config.h"

#include "object-internals.h"
#include "class-internals.h"
#include "PlatformInvoke.h"
#include "Exception.h"
#include "LibraryLoader.h"
#include "MetadataCache.h"
#include "Object.h"
#include "Method.h"
#include "Type.h"
#include "os/LibraryLoader.h"
#include "os/MarshalStringAlloc.h"
#include "utils/Memory.h"
#include "utils/StringViewStream.h"
#include "utils/StringUtils.h"

#include <stdint.h>
#include <sstream>
#include <algorithm>

namespace il2cpp
{
namespace vm
{

void PlatformInvoke::SetFindPluginCallback(Il2CppSetFindPlugInCallback method)
{
	LibraryLoader::SetFindPluginCallback(method);
}

Il2CppMethodPointer PlatformInvoke::Resolve(const PInvokeArguments& pinvokeArgs)
{
	void* dynamicLibrary = LibraryLoader::LoadLibrary(pinvokeArgs.moduleName);
	if (dynamicLibrary == NULL)
	{
		std::basic_stringstream<Il2CppNativeChar> message;
		message << "Unable to load DLL '" << pinvokeArgs.moduleName << "': The specified module could not be found.";
		Exception::Raise(Exception::GetDllNotFoundException(il2cpp::utils::StringUtils::NativeStringToUtf8(message.str()).c_str()));
	}

	Il2CppMethodPointer function = os::LibraryLoader::GetFunctionPointer(dynamicLibrary, pinvokeArgs);
	if (function == NULL)
	{
		std::basic_stringstream<Il2CppNativeChar> message;
		message << "Unable to find an entry point named '" << pinvokeArgs.entryPoint << "' in '" << pinvokeArgs.moduleName << "'.";
		Exception::Raise(Exception::GetEntryPointNotFoundException(il2cpp::utils::StringUtils::NativeStringToUtf8(message.str()).c_str()));
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

Il2CppChar* PlatformInvoke::MarshalCSharpStringToCppWString(Il2CppString* managedString)
{
	if (managedString == NULL)
		return NULL;

	int32_t stringLength = String::GetLength(managedString);
	Il2CppChar* nativeString = MarshalAllocateStringBuffer<Il2CppChar>(stringLength + 1);
	for (int32_t i = 0; i < managedString->length; ++i)
		nativeString[i] = managedString->chars[i];
	
	nativeString[managedString->length] = '\0';

	return nativeString;
}

void PlatformInvoke::MarshalCSharpStringToCppWStringFixed(Il2CppString* managedString, Il2CppChar* buffer, int numberOfCharacters)
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

il2cpp_hresult_t PlatformInvoke::MarshalCSharpStringToCppBStringNoThrow(Il2CppString* managedString, Il2CppChar** bstr)
{
	IL2CPP_ASSERT(bstr);

	if (managedString == NULL)
	{
		*bstr = NULL;
		return IL2CPP_S_OK;
	}

	int32_t stringLength = String::GetLength(managedString);
	Il2CppChar* stringChars = String::GetChars(managedString);
	return os::MarshalStringAlloc::AllocateBStringLength(stringChars, stringLength, bstr);
}

Il2CppChar* PlatformInvoke::MarshalCSharpStringToCppBString(Il2CppString* managedString)
{
	Il2CppChar* bstr;
	const il2cpp_hresult_t hr = MarshalCSharpStringToCppBStringNoThrow(managedString, &bstr);
	Exception::RaiseIfFailed(hr);
	return bstr;
}

Il2CppString* PlatformInvoke::MarshalCppStringToCSharpStringResult(const char* value)
{
	if (value == NULL)
		return NULL;

	return String::New(value);
}

static int32_t Utf16StringLength(const Il2CppChar* value)
{
	if (value == NULL)
		return 0;

	const Il2CppChar* ptr = value;
	int32_t length = 0;
	while (*ptr)
	{
		ptr++;
		length++;
	}

	return length;
}

Il2CppString* PlatformInvoke::MarshalCppWStringToCSharpStringResult(const Il2CppChar* value)
{
	if (value == NULL)
		return NULL;

	return String::NewUtf16(value, Utf16StringLength(value));
}

Il2CppString* PlatformInvoke::MarshalCppBStringToCSharpStringResult(const Il2CppChar* value)
{
	if (value == NULL)
		return NULL;

	int32_t length;
	const il2cpp_hresult_t hr = os::MarshalStringAlloc::GetBStringLength(value, &length);
	Exception::RaiseIfFailed(hr);

	return String::NewUtf16(value, length);
}

void PlatformInvoke::MarshalFreeBString(Il2CppChar* value)
{
	const il2cpp_hresult_t hr = os::MarshalStringAlloc::FreeBString(value);
	Exception::RaiseIfFailed(hr);
}

char* PlatformInvoke::MarshalStringBuilder(Il2CppStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

#if !NET_4_0
	size_t stringLength = String::GetLength(stringBuilder->str);

	// not sure if this is necessary but it's better to be safe than sorry
	IL2CPP_ASSERT(static_cast<int32_t>(stringLength) >= stringBuilder->length);
	if (static_cast<int32_t>(stringLength) < stringBuilder->length)
		stringLength = stringBuilder->length;

	std::string utf8String = utils::StringUtils::Utf16ToUtf8(stringBuilder->str->chars, stringBuilder->length);

	if (stringLength < utf8String.length())
		stringLength = utf8String.length();

	char* nativeString = MarshalAllocateStringBuffer<char>(stringLength + 1);
	strcpy(nativeString, utf8String.c_str());

	return nativeString;

#else
	size_t stringLength = 0;
	Il2CppStringBuilder* currentBuilder = stringBuilder;

	std::vector<std::string> utf8Chunks;
	std::vector<Il2CppStringBuilder*> builders;

	while (true)
	{
		if (currentBuilder == NULL)
			break;

		const Il2CppChar *str = (Il2CppChar*)il2cpp::vm::Array::GetFirstElementAddress(currentBuilder->chunkChars);
		std::string utf8String = utils::StringUtils::Utf16ToUtf8(str, (int)currentBuilder->chunkChars->max_length);

		utf8Chunks.push_back(utf8String);
		builders.push_back(currentBuilder);

		size_t lenToCount = std::max((size_t)currentBuilder->chunkChars->max_length, utf8String.size());

		stringLength += lenToCount;

		currentBuilder = currentBuilder->chunkPrevious;
	}

	char* nativeString = MarshalAllocateStringBuffer<char>(stringLength + 1);

	// We need to zero out the memory because the chunkChar array lengh may have been larger than the chunkLength
	// and when this happens we'll have a utf8String that is smaller than the the nativeString we allocated.  When we go to copy the
	// chunk utf8String into the nativeString it won't fill everything and we can end up with w/e junk value was in that memory before
	memset(nativeString, 0, sizeof(char) * (stringLength + 1));

	if (stringLength > 0)
	{
		int offsetAdjustment = 0;
		for (int i = (int)utf8Chunks.size() - 1; i >= 0; i--)
		{
			std::string utf8String = utf8Chunks[i];

			const char* utf8CString = utf8String.c_str();

			memcpy(nativeString + builders[i]->chunkOffset + offsetAdjustment, utf8CString, (int)utf8String.size());

			offsetAdjustment += (int)utf8String.size() - builders[i]->chunkLength;
		}
	}

	return nativeString;
#endif

}

Il2CppChar* PlatformInvoke::MarshalWStringBuilder(Il2CppStringBuilder* stringBuilder)
{
	if (stringBuilder == NULL)
		return NULL;

#if !NET_4_0
	int32_t stringLength = String::GetLength(stringBuilder->str);

	// not sure if this is necessary but it's better to be safe than sorry
	IL2CPP_ASSERT(stringLength >= stringBuilder->length);
	if (stringLength < stringBuilder->length)
		stringLength = stringBuilder->length;

	Il2CppChar* nativeString = MarshalAllocateStringBuffer<Il2CppChar>(stringLength + 1);
	for (int32_t i = 0; i < stringBuilder->length; ++i)
		nativeString[i] = stringBuilder->str->chars[i];

	nativeString[stringBuilder->length] = '\0';

	return nativeString;
#else
	size_t stringLength = 0;
	Il2CppStringBuilder* currentBuilder = stringBuilder;
	while (true)
	{
		if (currentBuilder == NULL)
			break;

		stringLength += (size_t)currentBuilder->chunkChars->max_length;

		currentBuilder = currentBuilder->chunkPrevious;
	}

	Il2CppChar* nativeString = MarshalAllocateStringBuffer<Il2CppChar>(stringLength + 1);

	if (stringLength > 0)
	{
		currentBuilder = stringBuilder;
		while (true)
		{
			if (currentBuilder == NULL)
				break;

			const Il2CppChar *str = (Il2CppChar*)il2cpp::vm::Array::GetFirstElementAddress(currentBuilder->chunkChars);

			memcpy(nativeString + currentBuilder->chunkOffset, str, (int)currentBuilder->chunkChars->max_length * sizeof(Il2CppChar));

			currentBuilder = currentBuilder->chunkPrevious;
		}
	}

	nativeString[stringLength] = '\0';

	return nativeString;
#endif
}

void PlatformInvoke::MarshalStringBuilderResult(Il2CppStringBuilder* stringBuilder, char* buffer)
{
	if (stringBuilder == NULL || buffer == NULL)
		return;

#if !NET_4_0
	Il2CppString* managedString = MarshalCppStringToCSharpStringResult(buffer);
	stringBuilder->str = managedString;
	stringBuilder->length = String::GetLength(managedString);
#else
	UTF16String utf16String = utils::StringUtils::Utf8ToUtf16(buffer);

	IL2CPP_OBJECT_SETREF(stringBuilder, chunkChars, il2cpp::vm::Array::New(il2cpp_defaults.char_class, (int)utf16String.size() + 1));

	for (int i = 0; i < (int)utf16String.size(); i++)
		il2cpp_array_set(stringBuilder->chunkChars, Il2CppChar, i, utf16String[i]);

	il2cpp_array_set(stringBuilder->chunkChars, Il2CppChar, (int)utf16String.size(),'\0');

	stringBuilder->chunkLength = (int)utf16String.size();
	stringBuilder->chunkOffset = 0;
	IL2CPP_OBJECT_SETREF(stringBuilder, chunkPrevious, NULL);
#endif
}

void PlatformInvoke::MarshalWStringBuilderResult(Il2CppStringBuilder* stringBuilder, Il2CppChar* buffer)
{
	if (stringBuilder == NULL || buffer == NULL)
		return;

#if !NET_4_0
	Il2CppString* managedString = MarshalCppWStringToCSharpStringResult(buffer);
	stringBuilder->str = managedString;
	stringBuilder->length = String::GetLength(managedString);
#else
	int len = (int)utils::StringUtils::StrLen(buffer);

	IL2CPP_OBJECT_SETREF(stringBuilder, chunkChars, il2cpp::vm::Array::New(il2cpp_defaults.char_class, len + 1));

	for (int i = 0; i < len; i++)
		il2cpp_array_set(stringBuilder->chunkChars, Il2CppChar, i, buffer[i]);

	il2cpp_array_set(stringBuilder->chunkChars, Il2CppChar, len, '\0');

	stringBuilder->chunkLength = len;
	stringBuilder->chunkOffset = 0;
	IL2CPP_OBJECT_SETREF(stringBuilder, chunkPrevious, NULL);
#endif
}

Il2CppIntPtr PlatformInvoke::MarshalDelegate(Il2CppDelegate* d)
{
	if (d == NULL)
		return Il2CppIntPtr::Zero;
	
	if (d->method->is_inflated)
		vm::Exception::Raise(vm::Exception::GetNotSupportedException("IL2CPP does not support marshaling delegates that point to generic methods."));

	IL2CPP_ASSERT(d->method->methodDefinition);

	Il2CppMethodPointer reversePInvokeWrapper = MetadataCache::GetReversePInvokeWrapperFromIndex(d->method->methodDefinition->reversePInvokeWrapperIndex);
	if (reversePInvokeWrapper == NULL)
	{
		// Okay, we cannot marshal it for some reason. Figure out why.
		if (Method::IsInstance(d->method))
			vm::Exception::Raise(vm::Exception::GetNotSupportedException("IL2CPP does not support marshaling delegates that point to instance methods to native code."));

		vm::Exception::Raise(vm::Exception::GetNotSupportedException("To marshal a managed method, please add an attribute named 'MonoPInvokeCallback' to the method definition."));
	}

	Il2CppIntPtr functionPointer;
	functionPointer.m_value = (void*)reversePInvokeWrapper;
	return functionPointer;
}

Il2CppDelegate* PlatformInvoke::MarshalFunctionPointerToDelegate(void* functionPtr, Il2CppClass* delegateType)
{
	if (!Class::HasParent(delegateType, il2cpp_defaults.delegate_class))
		Exception::Raise(Exception::GetArgumentException("t", "Type must derive from Delegate."));

	if (Class::IsGeneric(delegateType) || Class::IsInflated(delegateType))
		Exception::Raise(Exception::GetArgumentException("t", "The specified Type must not be a generic type definition."));

	Il2CppMethodPointer managedToNativeWrapperMethodPointer = MetadataCache::GetDelegateWrapperManagedToNativeFromIndex(delegateType->typeDefinition->reversePInvokeWrapperIndex);
	if (managedToNativeWrapperMethodPointer == NULL)
		Exception::Raise(Exception::GetMarshalDirectiveException(utils::StringUtils::Printf("Cannot marshal P/Invoke call through delegate of type '%s.%s'", Class::GetNamespace(delegateType), Class::GetName(delegateType)).c_str()));

	Il2CppObject* delegate = il2cpp::vm::Object::New(delegateType);
	Il2CppMethodPointer nativeFunctionPointer = (Il2CppMethodPointer)functionPtr;

	const MethodInfo* method = MetadataCache::GetNativeDelegate (nativeFunctionPointer);
	if (method == NULL)
	{
		MethodInfo* newMethod = (MethodInfo*)IL2CPP_CALLOC (1, sizeof (MethodInfo));
		newMethod->methodPointer = nativeFunctionPointer;
		newMethod->invoker_method = NULL;
		MetadataCache::AddNativeDelegate(nativeFunctionPointer, newMethod);
		method = newMethod;
	}

	Type::ConstructDelegate((Il2CppDelegate*)delegate, delegate, managedToNativeWrapperMethodPointer, method);

	return (Il2CppDelegate*)delegate;
}

typedef void(*MarshalFunc)(void*, void*);

void PlatformInvoke::MarshalStructToNative(void* managedStructure, void* marshaledStructure, Il2CppClass* type)
{
	MarshalFunc marshalFunc = (MarshalFunc)MetadataCache::GetMarshalToNativeFuncFromIndex (type->typeDefinition->marshalingFunctionsIndex);
	marshalFunc(managedStructure, marshaledStructure);
}

void PlatformInvoke::MarshalStructFromNative(void* marshaledStructure, void* managedStructure, Il2CppClass* type)
{
	MarshalFunc marshalFunc = (MarshalFunc)MetadataCache::GetMarshalFromNativeFuncFromIndex (type->typeDefinition->marshalingFunctionsIndex);
	marshalFunc(marshaledStructure, managedStructure);
}

bool PlatformInvoke::MarshalFreeStruct(void* marshaledStructure, Il2CppClass* type)
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
