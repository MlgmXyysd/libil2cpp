#pragma once

#include <stdint.h>

struct TypeInfo;
struct Il2CppGenericClass;
struct Il2CppGenericContext;

namespace il2cpp
{
namespace vm
{

class GenericClass
{
public:
	// exported

public:
	//internal
	static TypeInfo* GetClass (Il2CppGenericClass *gclass);
	static Il2CppGenericContext* GetContext (Il2CppGenericClass *gclass);
	static bool IsValueType (Il2CppGenericClass *gclass);
};

} /* namespace vm */
} /* namespace il2cpp */
