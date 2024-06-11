#include "il2cpp-config.h"
#include "object-internals.h"

#include "icalls/mscorlib/System.Reflection/AssemblyName.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"
#include "vm/AssemblyName.h"
#include "utils/StringUtils.h"

using il2cpp::vm::Array;
using il2cpp::vm::Class;
using il2cpp::vm::Object;
using il2cpp::vm::Runtime;
using il2cpp::vm::String;

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
    bool AssemblyName::ParseName(Il2CppReflectionAssemblyName* aname, Il2CppString* assemblyName)
    {
        return vm::AssemblyName::ParseName(aname, utils::StringUtils::Utf16ToUtf8(String::GetChars(assemblyName)));
    }

#if NET_4_0
    void AssemblyName::get_public_token(uint8_t* token, uint8_t* pubkey, int32_t len)
    {
        NOT_IMPLEMENTED_ICALL(AssemblyName::get_public_token);
        IL2CPP_UNREACHABLE;
    }

#endif
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
