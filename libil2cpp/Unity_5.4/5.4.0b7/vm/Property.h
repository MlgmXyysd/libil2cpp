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
	static uint32_t GetFlags (const PropertyInfo* prop);
	static const MethodInfo* GetGetMethod (const PropertyInfo* prop);
	static const MethodInfo* GetSetMethod (const PropertyInfo* prop);
	static const char* GetName (const PropertyInfo* prop);
	static TypeInfo* GetParent (const PropertyInfo* prop);
	static uint32_t GetToken (const PropertyInfo* prop);
};

} /* namespace vm */
} /* namespace il2cpp */
