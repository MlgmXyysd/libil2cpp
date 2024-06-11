#pragma once

#include "il2cpp-config.h"
#include "metadata.h"
#include "blob.h"
#include "PlatformInvoke.h"
#include "String.h"
#include "MarshalAlloc.h"
#include "Array.h"
#include "Class.h"
#include "Object.h"

#include <string>

struct Il2CppString;
struct Il2CppStringBuilder;

namespace il2cpp
{
namespace vm
{

class PlatformInvoke
{
public:
	static int SumParameterSize(Il2CppTypeEnum parameterTypes[]);
	static methodPointerType Resolve(const PInvokeArguments& pinvokeArgs);

	static void MarshalFree(void* ptr);

	static char* MarshalCSharpStringToCppString(Il2CppString* managedString);
	static void MarshalCSharpStringToCppStringFixed(Il2CppString* managedString, char* buffer, int numberOfCharacters);
	static uint16_t* MarshalCSharpStringToCppWString(Il2CppString* managedString);
	static void MarshalCSharpStringToCppWStringFixed(Il2CppString* managedString, uint16_t* buffer, int numberOfCharacters);

	static Il2CppString* MarshalCppStringToCSharpStringResult(const char* value);
	static Il2CppString* MarshalCppWStringToCSharpStringResult(const uint16_t* value);

	static void MarshalStringArray(Il2CppArray* a, char** nativeArray);
	static void MarshalWStringArray(Il2CppArray* a, uint16_t** nativeArray);

	static Il2CppArray* MarshalStringArrayResult(char** a, size_t size);
	static Il2CppArray* MarshalWStringArrayResult(uint16_t** a, size_t size);

	static void MarshalStringBuilderResult(Il2CppStringBuilder* stringBuilder, char* buffer);
	static void MarshalWStringBuilderResult(Il2CppStringBuilder* stringBuilder, uint16_t* buffer);

	static void MarshalCharArray(Il2CppArray* a, char* nativeArray);
	static Il2CppArray* MarshalCharArrayResult(char* a, size_t size);

	static void MarshalFreeStringArray(void** a, size_t size);

	static Il2CppIntPtr MarshalDelegate(Il2CppDelegate* d);
	static Il2CppDelegate* MarshalFunctionPointerToDelegate(void* functionPtr, TypeInfo* delegateType);

	static void MarshalStructToNative(void* managedStructure, void* marshaledStructure, TypeInfo* type);
	static void MarshalStructFromNative(void* marshaledStructure, void* managedStructure, TypeInfo* type);
	static bool MarshalFreeStruct(void* marshaledStructure, TypeInfo* type);

	template <typename ElementType>
	static ElementType* MarshalArray(Il2CppArray* a)
	{
		if (a == NULL)
			return NULL;

		return (ElementType*)a->vector;
	}

	template <typename ElementType>
	static Il2CppArray* MarshalArrayResult(TypeInfo* type, ElementType* cppArray, size_t size)
	{
		if (cppArray == NULL)
			return NULL;

		Il2CppArray* array = Array::New(type, (il2cpp_array_size_t)size);

		for (size_t i = 0; i < size; ++i)
			il2cpp_array_setwithsize(array, sizeof(ElementType), i, cppArray[i]);

		return array;
	}

	template <typename T>
	static T* MarshalAllocateStringBuffer(size_t numberOfCharacters)
	{
		return (T*)MarshalAlloc::Allocate(numberOfCharacters * sizeof(T));
	}
};

} /* namespace vm */
} /* namespace il2cpp */
