#pragma once

struct Il2CppArray;
struct Il2CppMethodMessage;
struct Il2CppReflectionMethod;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Runtime
{
namespace Remoting
{
namespace Messaging
{

class MonoMethodMessage
{
public:
	static void InitMessage (Il2CppMethodMessage*, Il2CppReflectionMethod*, Il2CppArray*);
};

} /* namespace Messaging */
} /* namespace Remoting */
} /* namespace Runtime */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */