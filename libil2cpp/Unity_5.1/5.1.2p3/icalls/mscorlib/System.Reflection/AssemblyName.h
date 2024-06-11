#pragma once

#include <stdint.h>

struct Il2CppString;
struct Il2CppReflectionAssemblyName;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{

class AssemblyName
{
public:
	static bool ParseName (Il2CppReflectionAssemblyName* aname, Il2CppString* assemblyName);
};

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
