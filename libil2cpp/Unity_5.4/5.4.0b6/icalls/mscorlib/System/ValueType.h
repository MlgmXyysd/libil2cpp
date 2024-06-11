#pragma once

#include <stdint.h>

struct Il2CppArray;
struct Il2CppObject;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

class ValueType
{
public:
	static bool InternalEquals(Il2CppObject * __this, Il2CppObject * that, Il2CppArray** fields);
	static int InternalGetHashCode(Il2CppObject *,Il2CppArray * *);
};

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
