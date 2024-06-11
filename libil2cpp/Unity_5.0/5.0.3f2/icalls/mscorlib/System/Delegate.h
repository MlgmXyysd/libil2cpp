#pragma once

#include <stdint.h>

struct Il2CppObject;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;

struct Il2CppDelegate;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class Delegate
{
public:
	static Il2CppDelegate * CreateDelegate_internal (Il2CppReflectionType *,Il2CppObject *, Il2CppReflectionMethod *,bool);
	static void SetMulticastInvoke (Il2CppDelegate *);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
