#pragma once

#include "il2cpp-api-types.h"

namespace il2cpp
{
namespace utils
{

struct Memory
{
	static void SetMemoryCallbacks (Il2CppMemoryCallbacks* callbacks);

	static void* Malloc (size_t size);
	static void Free (void* memory);
	static void* Calloc (size_t count, size_t size);
	static void* Realloc (void* memory, size_t newSize);
};

} /* namespace utils */
} /* namespace il2cpp */

#define IL2CPP_MALLOC(size) il2cpp::utils::Memory::Malloc(size)
#define IL2CPP_MALLOC_ZERO(size) il2cpp::utils::Memory::Calloc(1,size)
#define IL2CPP_FREE(memory) il2cpp::utils::Memory::Free(memory)
#define IL2CPP_CALLOC(count,size) il2cpp::utils::Memory::Calloc(count,size)
#define IL2CPP_REALLOC(memory,newSize) il2cpp::utils::Memory::Realloc(memory,newSize)
