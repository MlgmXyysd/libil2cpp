#pragma once

#include "il2cpp-config.h"
#include <stdint.h>

// This file contains the structures specifying how we store converted metadata.
// These structures have 3 constraints:
// 1. These structures will be stored in an external file, and as such must not contain any pointers.
//    All references to other metadata should occur via an index into a corresponding table.
// 2. These structures are assumed to be const. Either const structures in the binary or mapped as
//    readonly memory from an external file. Do not add any 'calculated' fields which will be written to at runtime.
// 3. These structures should be optimized for size. Other structures are used at runtime which can
//    be larger to store cached information

typedef int32_t TypeIndex;
typedef int32_t FieldIndex;
typedef int32_t DefaultValueIndex;
typedef int32_t DefaultValueDataIndex;
typedef int32_t CustomAttributeIndex;
typedef int32_t ParameterIndex;
typedef int32_t MethodIndex;
typedef int32_t PropertyIndex;
typedef int32_t EventIndex;
typedef int32_t GenericContainerIndex;
typedef int32_t GenericParameterIndex;
typedef int16_t GenericParameterConstraintIndex;
typedef int32_t RGCTXIndex;
typedef int32_t StringIndex;
typedef int32_t StringLiteralIndex;
typedef int32_t GenericInstIndex;

const TypeIndex kTypeIndexInvalid = -1;
const DefaultValueDataIndex kDefaultValueIndexNull = -1;
const EventIndex kEventIndexInvalid = -1;
const FieldIndex kFieldIndexInvalid = -1;
const MethodIndex kMethodIndexInvalid = -1;
const PropertyIndex kPropertyIndexInvalid = -1;
const GenericContainerIndex kGenericContainerIndexInvalid = -1;
const GenericParameterIndex kGenericParameterIndexInvalid = -1;
const StringLiteralIndex kStringLiteralIndexInvalid = -1;

// Encoded index (1 bit)
// MethodDef - 0
// MethodSpec - 1
typedef uint32_t EncodedMethodIndex;

static inline bool IsGenericMethodIndex (EncodedMethodIndex index)
{
	return (index & 0x80000000U) != 0;
}

static inline uint32_t GetDecodedMethodIndex (EncodedMethodIndex index)
{
	return index & 0x7FFFFFFFU;
}


struct Il2CppFieldDefinition
{
	StringIndex nameIndex;
	TypeIndex typeIndex;
	CustomAttributeIndex customAttributeIndex;
};

struct Il2CppFieldDefaultValue
{
	FieldIndex fieldIndex;
	TypeIndex typeIndex;
	DefaultValueDataIndex dataIndex;
};

struct Il2CppParameterDefinition
{
	StringIndex nameIndex;
	uint32_t token;
	CustomAttributeIndex customAttributeIndex;
	TypeIndex typeIndex;
};

struct Il2CppMethodDefinition
{
	StringIndex nameIndex;
	TypeIndex returnType;
	ParameterIndex parameterStart;
	CustomAttributeIndex customAttributeIndex;
	GenericContainerIndex genericContainerIndex;
	MethodIndex methodIndex;
	MethodIndex invokerIndex;
	MethodIndex delegateWrapperIndex;
	RGCTXIndex rgctxDefinitionIndex;
	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint16_t parameterCount;
};

struct Il2CppEventDefinition
{
	StringIndex nameIndex;
	TypeIndex typeIndex;
	MethodIndex add;
	MethodIndex remove;
	MethodIndex raise;
	CustomAttributeIndex customAttributeIndex;
};

struct Il2CppPropertyDefinition
{
	StringIndex nameIndex;
	MethodIndex get;
	MethodIndex set;
	uint32_t attrs;
	CustomAttributeIndex customAttributeIndex;
};

struct Il2CppMethodDefinitionReference
{
	TypeIndex typeIndex;
	uint16_t methodOffset; // offset into methods array on the type. A local index, not a global one.
};

struct Il2CppMethodSpec
{
	MethodIndex methodRefIndex;
	GenericInstIndex classIndexIndex;
	GenericInstIndex methodIndexIndex;
};

struct Il2CppStringLiteral
{
	uint32_t length;
	StringLiteralIndex dataIndex;
};

struct Il2CppGenericMethodIndices
{
	MethodIndex methodIndex;
	MethodIndex invokerIndex;
};

struct Il2CppGenericMethodFunctionsDefinitions
{
	MethodIndex genericMethod;
	Il2CppGenericMethodIndices indices;
};

#pragma pack(push, p1,4)
struct Il2CppGlobalMetadataHeader
{
	int32_t sanity;
	int32_t version;
	int32_t stringLiteralOffset; // string data for managed code
	int32_t stringLiteralCount;
	int32_t stringLiteralDataOffset;
	int32_t stringLiteralDataCount;
	int32_t stringOffset; // string data for metadata
	int32_t stringCount;
	int32_t eventsOffset; // Il2CppEventDefinition
	int32_t eventsCount;
	int32_t propertiesOffset; // Il2CppPropertyDefinition
	int32_t propertiesCount;
	int32_t methodsOffset; // Il2CppMethodDefinition
	int32_t methodsCount;
	int32_t fieldDefaultValuesOffset; // Il2CppFieldDefaultValue
	int32_t fieldDefaultValuesCount;
	int32_t fieldDefaultValueDataOffset; // uint8_t
	int32_t fieldDefaultValueDataCount;
	int32_t parametersOffset; // Il2CppParameterDefinition
	int32_t parametersCount;
	int32_t fieldsOffset; // Il2CppFieldDefinition
	int32_t fieldsCount;
	int32_t genericParametersOffset; // Il2CppGenericParameter
	int32_t genericParametersCount;
	int32_t genericParameterConstraintsOffset; // TypeIndex
	int32_t genericParameterConstraintsCount;
};
#pragma pack(pop, p1)
