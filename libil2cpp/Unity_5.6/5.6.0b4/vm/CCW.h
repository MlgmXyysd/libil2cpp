#pragma once

#include "gc/GarbageCollector.h"

struct Il2CppIUnknown;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API CCW
{
public:
	static Il2CppIManagedObjectHolder* CreateCCW(Il2CppObject* obj);
	static inline Il2CppIUnknown* GetOrCreate(Il2CppObject* obj, const Il2CppGuid& iid)
	{
		return gc::GarbageCollector::GetOrCreateCCW(obj, iid);
	}
};

} /* namespace vm */
} /* namespace il2cpp */
