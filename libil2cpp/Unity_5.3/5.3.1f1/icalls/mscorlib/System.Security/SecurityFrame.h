#pragma once

#include <stdint.h>

struct Il2CppArray;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Security
{

class SecurityFrame
{
public:
	static void* /* System.Security.RuntimeSecurityFrame */ _GetSecurityFrame (int32_t skip);
	static Il2CppArray * _GetSecurityStack (int32_t skip);
};

} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
