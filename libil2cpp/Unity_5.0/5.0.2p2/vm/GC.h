#pragma once

namespace il2cpp
{
namespace vm
{

class GC
{
public:
	static int64_t GetUsedHeapSize();
	static int64_t GetAllocatedHeapSize();
};

} /* namespace vm */
} /* namespace il2cpp */