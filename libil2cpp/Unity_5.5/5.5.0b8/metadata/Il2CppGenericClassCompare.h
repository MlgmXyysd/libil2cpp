#pragma once

#include "utils/KeyWrapper.h"

struct Il2CppGenericClass;

namespace il2cpp
{
namespace metadata
{

class Il2CppGenericClassCompare
{
public:
	bool operator() (const KeyWrapper<Il2CppGenericClass*>& t1,const KeyWrapper<Il2CppGenericClass*>& t2) const;
	static bool Compare (const KeyWrapper<Il2CppGenericClass*>& t1,const KeyWrapper<Il2CppGenericClass*>& t2);
};

} /* namespace vm */
} /* namespace il2cpp */
