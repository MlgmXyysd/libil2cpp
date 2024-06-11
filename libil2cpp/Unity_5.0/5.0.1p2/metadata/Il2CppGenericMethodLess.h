#pragma once

struct Il2CppGenericMethod;

namespace il2cpp
{
namespace metadata
{

struct Il2CppGenericMethodLess
{
	bool operator() (const Il2CppGenericMethod* m1, const Il2CppGenericMethod* m2) const;
};

} /* namespace metadata */
} /* namespace il2cpp */
