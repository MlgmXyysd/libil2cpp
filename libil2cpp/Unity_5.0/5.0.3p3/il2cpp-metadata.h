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
typedef int32_t MethodIndex;
typedef int32_t PropertyIndex;
typedef int32_t EventIndex;
typedef int32_t StringIndex;
typedef int32_t StringLiteralIndex;

const TypeIndex kTypeIndexInvalid = -1;
const DefaultValueDataIndex kDefaultValueIndexNull = -1;
const EventIndex kEventIndexInvalid = -1;
const FieldIndex kFieldIndexInvalid = -1;
const MethodIndex kMethodIndexInvalid = -1;
const MethodIndex kPropertyIndexInvalid = -1;
const StringLiteralIndex kStringLiteralIndexInvalid = -1;


struct Il2CppFieldDefinition
{
	StringIndex nameIndex;
	TypeIndex typeIndex;
	int32_t offset;	// If offset is -1, then it's thread static
	CustomAttributeIndex customAttributeIndex;
};

struct Il2CppFieldDefaultValue
{
	FieldIndex fieldIndex;
	TypeIndex typeIndex;
	DefaultValueDataIndex dataIndex;
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

struct Il2CppStringLiteral
{
	uint32_t length;
	StringLiteralIndex dataIndex;
};

#pragma pack(push, p1,4)
struct Il2CppGlobalMetadataHeader
{
	int32_t sanity;
	int32_t version;
	int32_t stringLiteralOffset;
	int32_t stringLiteralCount;
	int32_t stringLiteralDataOffset;
	int32_t stringLiteralDataCount;
};
#pragma pack(pop, p1)
