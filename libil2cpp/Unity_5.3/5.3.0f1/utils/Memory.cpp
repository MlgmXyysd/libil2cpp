#include "il2cpp-config.h"
#include "utils/Memory.h"
#include <string>

namespace il2cpp
{
namespace utils
{
static Il2CppMemoryCallbacks s_Callbacks =
{
	malloc,
	free,
	calloc,
	realloc
};

void Memory::SetMemoryCallbacks (Il2CppMemoryCallbacks* callbacks)
{
	memcpy (&s_Callbacks, callbacks, sizeof (Il2CppMemoryCallbacks));
}


void* Memory::Malloc (size_t size)
{
	return s_Callbacks.malloc_func (size);
}

void Memory::Free (void* memory)
{
	return s_Callbacks.free_func (memory);
}

void* Memory::Calloc (size_t count, size_t size)
{
	return s_Callbacks.calloc_func (count, size);
}

void* Memory::Realloc (void* memory, size_t newSize)
{
	return s_Callbacks.realloc_func (memory, newSize);
}

} /* namespace utils */
} /* namespace il2cpp */
