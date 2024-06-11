#pragma once

#include <stdint.h>
#include "il2cpp-metadata.h"
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
	static ParameterInfo* InflateParameters (const ParameterInfo* parameters, uint8_t parameterCount, const Il2CppGenericContext* context, bool inflateMethodVars);
	static Il2CppGenericClass* GetGenericClass (TypeInfo* elementClass, const Il2CppGenericInst* inst);
	static Il2CppGenericClass* GetGenericClass (TypeDefinitionIndex elementClassIndex, const Il2CppGenericInst* inst);

	static const MethodInfo* Inflate (const MethodInfo* methodDefinition, TypeInfo* declaringClass, const Il2CppGenericContext* context);
	static const Il2CppGenericMethod* Inflate (const Il2CppGenericMethod* genericMethod, const Il2CppGenericContext* context);

	static Il2CppRGCTXData* InflateRGCTX (RGCTXIndex rgctxEntryIndex, int16_t rgctxEntryCount, const Il2CppGenericContext* context);

	// temporary while we generate generics
	static void RegisterGenericClass (Il2CppGenericClass *gclass);

	static const Il2CppType* InflateIfNeeded (const Il2CppType* type, const Il2CppGenericContext* context, bool inflateMethodVars);

	typedef void(*GenericClassWalkCallback)(TypeInfo* type, void* context);
	static void WalkAllGenericClasses(GenericClassWalkCallback callback, void* context);
};

} /* namespace vm */
} /* namespace il2cpp */
