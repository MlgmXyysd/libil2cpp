#pragma once

#if _DEBUG
#include <map>
#include "os/Mutex.h"
#endif

namespace il2cpp
{
namespace vm
{

class MarshalAlloc
{
public:
	static void* Allocate(size_t size);
	static void* ReAlloc(void* ptr, size_t size);
	static void Free(void* ptr);

	static void* AllocateHGlobal(size_t size);
	static void* ReAllocHGlobal(void* ptr, size_t size);
	static void FreeHGlobal(void* ptr);

#if _DEBUG
	static bool HasUnfreedAllocations();
	static void ClearAllTrackedAllocations();

private:
	static os::Mutex s_Mutex; // Use a slow mutex, as this is only necessary in a debug build.
	static std::map<void*, int> s_Allocations;
#endif

};

} /* namespace vm */
} /* namespace il2cpp */
