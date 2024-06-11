#include "il2cpp-config.h"

#include <cassert>

#include "icalls/System/System.Diagnostics/PerformanceCounter.h"
#include "vm/Exception.h"

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace Diagnostics
{


Il2CppIntPtr PerformanceCounter::GetImpl (Il2CppString* category, Il2CppString* counter, Il2CppString* instance, Il2CppString* machine, PerformanceCounterType* ctype, bool* custom)
{
	NOT_IMPLEMENTED_ICALL (PerformanceCounter::GetImpl);
	
	return Il2CppIntPtr();
}

bool PerformanceCounter::GetSample (Il2CppIntPtr impl, bool only_value, CounterSample* sample)
{
	NOT_IMPLEMENTED_ICALL (PerformanceCounter::GetSample);
	
	return false;
}

int64_t PerformanceCounter::UpdateValue (Il2CppIntPtr impl, bool do_incr, int64_t value)
{
	NOT_IMPLEMENTED_ICALL (PerformanceCounter::UpdateValue);
	
	return 0;
}

void PerformanceCounter::FreeData (Il2CppIntPtr impl)
{
	NOT_IMPLEMENTED_ICALL (PerformanceCounter::FreeData);
}

} /* namespace Diagnostics */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
