#pragma once

#include "utils/KeyWrapper.h"

struct Il2CppGenericMethod;

namespace il2cpp
{
namespace metadata
{

struct Il2CppGenericMethodCompare
{
	bool operator() (const KeyWrapper<const Il2CppGenericMethod*>& m1, const KeyWrapper<const Il2CppGenericMethod*>& m2) const;
	static bool Equals (const KeyWrapper<const Il2CppGenericMethod*>& m1, const KeyWrapper<const Il2CppGenericMethod*>& m2);
};

} /* namespace metadata */
} /* namespace il2cpp */
