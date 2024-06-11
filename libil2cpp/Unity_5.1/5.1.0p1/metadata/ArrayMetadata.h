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
public:
	// called as part of Class::Init with lock held
	static void SetupArrayRuntimeMetadata (TypeInfo* klass, const il2cpp::os::FastAutoLock& lock);
};

} /* namespace vm */
} /* namespace il2cpp */
