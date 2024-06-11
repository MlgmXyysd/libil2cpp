#pragma once

#include <stdint.h>
#include <string>

struct MethodInfo;
struct PropertyInfo;

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
	static const Il2CppType* GetReturnType (MethodInfo* method);
	static const char* GetName (const MethodInfo *method);
	static std::string GetFullName (const MethodInfo* method);
	static bool IsGeneric (MethodInfo *method);
	static bool IsInflated (MethodInfo *method);
	static bool IsInstance (MethodInfo *method);
	static uint32_t GetParamCount (MethodInfo *method);
	static const Il2CppType* GetParam (MethodInfo *method, uint32_t index);
	static TypeInfo* GetClass (MethodInfo *method);
	static bool HasAttribute (MethodInfo *method, TypeInfo *attr_class);
	static TypeInfo *GetDeclaringType (MethodInfo* method);
	static const Il2CppDebugMethodInfo *GetDebugInfo (const MethodInfo *method);
	static uint32_t GetImplementationFlags (const MethodInfo *method);
	static uint32_t GetFlags (const MethodInfo *method);
	static uint32_t GetToken (const MethodInfo *method);
	static const char* GetParamName(MethodInfo *method, uint32_t index);
	static bool IsSameOverloadSignature(const MethodInfo* method1, const MethodInfo* method2);
	static bool IsSameOverloadSignature(const PropertyInfo* property1, const PropertyInfo* property2);
	static int CompareOverloadSignature(const PropertyInfo* property1, const PropertyInfo* property2);
};

} /* namespace vm */
} /* namespace il2cpp */
