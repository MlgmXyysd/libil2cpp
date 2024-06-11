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

class Char
{
public:
	static void GetDataTablePointers(unsigned char * *category_data,
		unsigned char * *numeric_data,
		double * *numeric_data_values,
		unsigned short * *to_lower_data_low,
		unsigned short * *to_lower_data_high,
		unsigned short * *to_upper_data_low,
		unsigned short * *to_upper_data_high);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
