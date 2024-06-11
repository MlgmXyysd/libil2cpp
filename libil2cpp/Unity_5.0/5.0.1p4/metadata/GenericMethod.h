#pragma once

#include <string>

struct MethodInfo;
struct Il2CppGenericMethod;
struct Il2CppGenericContext;

namespace il2cpp
{
namespace metadata
{

class GenericMethod
{
public:
	// exported

public:
	//internal
	static MethodInfo* GetMethod (Il2CppGenericMethod* gmethod);
	static Il2CppGenericContext* GetContext (Il2CppGenericMethod* gmethod);
	static std::string GetFullName (Il2CppGenericMethod* gmethod);
};

} /* namespace vm */
} /* namespace il2cpp */
