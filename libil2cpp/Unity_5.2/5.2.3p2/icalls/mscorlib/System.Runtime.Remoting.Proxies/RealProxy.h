#pragma once

struct Il2CppObject;
struct Il2CppReflectionType;
struct Il2CppString;

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
namespace Proxies
{

class RealProxy
{
public:
	static Il2CppObject* InternalGetTransparentProxy(Il2CppObject*, Il2CppString*);
	static Il2CppReflectionType* InternalGetProxyType(Il2CppObject*);
};

} /* namespace Proxies */
} /* namespace Remoting */
} /* namespace Runtime */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
