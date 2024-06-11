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
typedef int32_t MethodIndex;
typedef int32_t FieldIndex;
typedef int32_t DefaultValueIndex;
typedef int32_t DefaultValueDataIndex;
typedef int32_t CustomAttributeIndex;

const TypeIndex kTypeIndexInvalid = -1;
const DefaultValueDataIndex kDefaultValueIndexNull = -1;
const FieldIndex kFieldIndexInvalid = -1;

typedef int32_t StringIndex;

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

