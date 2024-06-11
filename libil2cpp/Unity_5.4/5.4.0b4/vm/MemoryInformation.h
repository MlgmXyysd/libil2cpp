#pragma once

#include "il2cpp-api-types.h"

namespace il2cpp
{

namespace vm
{

namespace MemoryInformation
{
	Il2CppManagedMemorySnapshot* CaptureManagedMemorySnapshot();
	void FreeCapturedManagedMemorySnapshot(Il2CppManagedMemorySnapshot* snapshot);
}

}

}
