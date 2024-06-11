#pragma once
#include "il2cpp-metadata.h"
#include "utils/Il2CppHashMap.h"
#include "utils/HashUtils.h"
#include "utils/StringUtils.h"

struct MonoMethodInfo
{
    int64_t hash;
    int32_t invoker_index;
    int32_t method_pointer_index;
};

struct MonoRGCTXDefinition
{
    Il2CppRGCTXDataType type;
    StringIndex imageName;
    int32_t token;
    int32_t generic_parameter_index;
};

struct RuntimeGenericContextInfo
{
    uint64_t hash;
    int32_t rgctxStart;
    int32_t rgctxCount;
};

struct MonoMetadataToken
{
    StringIndex image;
    int32_t token;
};

#pragma pack(push, p1,4)
struct MonoMethodMetadata
{
    MonoMetadataToken metadataToken;
    MethodIndex reversePInvokeWrapperIndex;
    uint64_t hash;
};
#pragma pack(pop, p1)

#pragma pack(push, p1,4)
struct MonoClassMetadata
{
    MonoMetadataToken metadataToken;
    int32_t genericParametersOffset;
    int32_t genericParametersCount;
    int32_t isPointer;
    int32_t rank; //if rank == 0, the token is for a non-array type, otherwise the rank is valid and the token represents the element type of the array
    int32_t elementTypeIndex;
};
#pragma pack(pop, p1)

struct MonoFieldMetadata
{
    TypeIndex parentTypeIndex;
    int32_t token;
};

struct MonoGenericInstMetadata
{
    uint32_t type_argc;
    const TypeIndex *type_argv_indices;
};

typedef Il2CppHashMap<uint64_t, const MonoMethodInfo*, il2cpp::utils::PassThroughHash<uint64_t> > MonoMethodInfoMap;
typedef Il2CppHashMap<uint64_t, const RuntimeGenericContextInfo*, il2cpp::utils::PassThroughHash<uint64_t> > MonoRgctxInfoMap;
typedef Il2CppHashMap<uint64_t, const MonoMethodMetadata*, il2cpp::utils::PassThroughHash<uint64_t> > MonoMethodMetadataMap;
