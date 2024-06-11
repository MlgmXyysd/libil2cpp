#pragma once

#include <stdint.h>
#include <string>

struct MethodInfo;
struct PropertyInfo;
struct ParameterInfo;

struct Il2CppString;
struct Il2CppType;
struct TypeInfo;
struct Il2CppDebugMethodInfo;

namespace il2cpp
{
namespace vm
{

class Method
{
public:
	static const Il2CppType* GetReturnType (const MethodInfo* method);
	static const char* GetName (const MethodInfo *method);
	static std::string GetFullName (const MethodInfo* method);
	static bool IsGeneric (const MethodInfo *method);
	static bool IsInflated (const MethodInfo *method);
	static bool IsInstance (const MethodInfo *method);
	static uint32_t GetParamCount (const MethodInfo *method);
	static const Il2CppType* GetParam (const MethodInfo *method, uint32_t index);
	static TypeInfo* GetClass (const MethodInfo *method);
	static bool HasAttribute (const MethodInfo *method, TypeInfo *attr_class);
	static TypeInfo *GetDeclaringType (const MethodInfo* method);
	static const Il2CppDebugMethodInfo *GetDebugInfo (const MethodInfo *method);
	static uint32_t GetImplementationFlags (const MethodInfo *method);
	static uint32_t GetFlags (const MethodInfo *method);
	static uint32_t GetToken (const MethodInfo *method);
	static const char* GetParamName (const MethodInfo *method, uint32_t index);
	static bool IsSameOverloadSignature(const MethodInfo* method1, const MethodInfo* method2);
	static bool IsSameOverloadSignature(const PropertyInfo* property1, const PropertyInfo* property2);
	static int CompareOverloadSignature(const PropertyInfo* property1, const PropertyInfo* property2);
	static const char* GetParameterDefaultValue(const MethodInfo *method, const ParameterInfo* parameter, const Il2CppType** type, bool* isExplicitySetNullDefaultValue);
};

} /* namespace vm */
} /* namespace il2cpp */
