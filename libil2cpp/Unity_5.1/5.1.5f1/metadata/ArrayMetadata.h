#pragma once

#include <stdint.h>
#include "os/Mutex.h"
struct TypeInfo;

namespace il2cpp
{
namespace metadata
{

class ArrayMetadata
{
public:
	static TypeInfo* GetBoundedArrayClass (TypeInfo* elementClass, uint32_t rank, bool bounded);

	typedef void(*ArrayTypeWalkCallback)(TypeInfo* type, void* context);
	static void WalkSZArrays(ArrayTypeWalkCallback callback, void* context);
	static void WalkArrays(ArrayTypeWalkCallback callback, void* context);

	// called as part of Class::Init with lock held
	static void SetupArrayInterfaces (TypeInfo* klass, const il2cpp::os::FastAutoLock& lock);
	static void SetupArrayVTable (TypeInfo* klass, const il2cpp::os::FastAutoLock& lock);
};

} /* namespace vm */
} /* namespace il2cpp */
