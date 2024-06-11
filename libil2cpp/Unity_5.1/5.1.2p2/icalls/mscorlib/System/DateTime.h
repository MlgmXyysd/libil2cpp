#pragma once

#include <stdint.h>

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class DateTime
{
public:
	static int64_t GetNow(void);
	static int64_t GetTimeMonotonic ();
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
