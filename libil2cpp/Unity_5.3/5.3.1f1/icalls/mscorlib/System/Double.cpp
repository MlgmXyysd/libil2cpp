#include "il2cpp-config.h"
#include <stdlib.h>
#include <cassert>
#include "icalls/mscorlib/System/Double.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

bool Double::ParseImpl (char *ptr, double *result)
{
	char *endptr = NULL;
	*result = 0.0;

	if (*ptr)
		*result = strtod (ptr, &endptr);

	if (!*ptr || (endptr && *endptr))
		return false;
	
	return true;
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
