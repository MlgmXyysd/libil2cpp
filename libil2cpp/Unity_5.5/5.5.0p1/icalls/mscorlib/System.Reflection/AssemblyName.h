#pragma once

#include <stdint.h>
#include "il2cpp-config.h"

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

class LIBIL2CPP_CODEGEN_API AssemblyName
{
public:
	static bool ParseName (Il2CppReflectionAssemblyName* aname, Il2CppString* assemblyName);
#if NET_4_0
	static void get_public_token(uint8_t* token, uint8_t* pubkey, int32_t len);
#endif
};

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
