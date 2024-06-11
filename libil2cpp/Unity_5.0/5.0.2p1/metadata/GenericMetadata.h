#pragma once

#include <stdint.h>
#include "metadata/Il2CppTypeVector.h"

struct Il2CppGenericClass;
struct Il2CppGenericContext;
struct Il2CppGenericInst;
struct Il2CppGenericMethod;
union Il2CppRGCTXData;
struct Il2CppRGCTXDefinition;
struct Il2CppType;
struct MethodInfo;
struct ParameterInfo;
struct TypeInfo;

namespace il2cpp
{
namespace metadata
{

class GenericMetadata
{
public:
	static ParameterInfo* InflateParameters (ParameterInfo* parameters, uint8_t parameterCount, Il2CppGenericContext* context, bool inflateMethodVars);
	static Il2CppGenericClass* GetGenericClass (TypeInfo* elementClass, Il2CppGenericInst* inst);

	static MethodInfo* Inflate (MethodInfo* methodDefinition, TypeInfo* declaringClass, Il2CppGenericContext* context);
	static Il2CppGenericMethod* Inflate (const Il2CppGenericMethod* genericMethod, Il2CppGenericContext* context);

	static Il2CppRGCTXData* InflateRGCTX (const Il2CppRGCTXDefinition* definitionData, Il2CppGenericContext* context);

	// temporary while we generate generics
	static void RegisterGenericClass (Il2CppGenericClass *gclass);

	static const Il2CppType* InflateIfNeeded (const Il2CppType* type, const Il2CppGenericContext* context, bool inflateMethodVars);
};

} /* namespace vm */
} /* namespace il2cpp */
