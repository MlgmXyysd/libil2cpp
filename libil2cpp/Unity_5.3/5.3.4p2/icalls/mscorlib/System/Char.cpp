#include "il2cpp-config.h"
#include <cassert>
#include "icalls/mscorlib/System/Char.h"
#include "char-conversions.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

void Char::GetDataTablePointers (unsigned char * *category_data,
	unsigned char * *numeric_data,
	double * *numeric_data_values,
	unsigned short * *to_lower_data_low,
	unsigned short * *to_lower_data_high,
	unsigned short * *to_upper_data_low,
	unsigned short * *to_upper_data_high)
{
	*category_data = const_cast<uint8_t*> (CategoryData_v4);
	*numeric_data = const_cast<uint8_t*> (NumericData);
	*numeric_data_values = const_cast<double*> (NumericDataValues);
	*to_lower_data_low = const_cast<uint16_t*> (ToLowerDataLow);
	*to_lower_data_high = const_cast<uint16_t*> (ToLowerDataHigh);
	*to_upper_data_low = const_cast<uint16_t*> (ToUpperDataLow);
	*to_upper_data_high = const_cast<uint16_t*> (ToUpperDataHigh);
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
