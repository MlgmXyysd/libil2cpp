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

class NumberFormatter
{
public:
	static void GetFormatterTables(uint64_t * * mantissas,
		int32_t * * exponents,
		int16_t * * digitLowerTable,
		int16_t * * digitUpperTable,
		int64_t * * tenPowersList,
		int32_t * * decHexDigits);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
