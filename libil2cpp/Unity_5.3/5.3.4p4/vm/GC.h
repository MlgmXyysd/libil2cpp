#pragma once

namespace il2cpp
{
namespace vm
{

class GC
{
public:
	static void Collect(int maxGeneration);
	static int32_t CollectALittle();
	static void Enable();
	static void Disable();
	static int64_t GetUsedHeapSize();
	static int64_t GetAllocatedHeapSize();
};

} /* namespace vm */
} /* namespace il2cpp */
