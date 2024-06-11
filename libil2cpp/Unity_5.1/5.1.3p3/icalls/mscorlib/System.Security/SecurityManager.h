#pragma once

#include "object-internals.h"

struct mscorlib_System_Reflection_MethodBase;
struct mscorlib_System_Security_RuntimeDeclSecurityActions;

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

class SecurityManager
{
public:
	static bool get_CheckExecutionRights ();
	static void set_CheckExecutionRights (bool value);
	static void set_SecurityEnabled (bool value);
	static bool GetLinkDemandSecurity (mscorlib_System_Reflection_MethodBase * method, mscorlib_System_Security_RuntimeDeclSecurityActions * _____cdecl, mscorlib_System_Security_RuntimeDeclSecurityActions * mdecl);
	static bool get_SecurityEnabled ();
};

} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
