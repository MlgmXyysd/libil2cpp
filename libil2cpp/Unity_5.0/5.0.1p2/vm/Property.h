#pragma once

#include <stdint.h>

struct MethodInfo;
struct PropertyInfo;
struct TypeInfo;

namespace il2cpp
{
namespace vm
{

class Property
{
public:
	// exported
	static uint32_t GetFlags (PropertyInfo* prop);
	static MethodInfo* GetGetMethod (PropertyInfo* prop);
	static MethodInfo* GetSetMethod (PropertyInfo* prop);
	static const char* GetName (PropertyInfo* prop);
	static TypeInfo* GetParent (PropertyInfo* prop);
};

} /* namespace vm */
} /* namespace il2cpp */