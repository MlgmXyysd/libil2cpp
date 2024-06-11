#pragma once

struct ParameterInfo;
struct Il2CppObject;

namespace il2cpp
{
namespace vm
{

class Parameter
{
public:
	// internal
	static Il2CppObject* GetDefaultParameterValueObject(const MethodInfo* method, const ParameterInfo* parameter, bool* isExplicitySetNullDefaultValue);
};

} /* namespace vm */
} /* namespace il2cpp */
