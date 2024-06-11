#pragma once
#include <cstdint>
#include "il2cpp-metadata.h"
#include "utils/Il2CppHashMap.h"
#include "utils/HashUtils.h"
#include "utils/StringUtils.h"

struct MonoMethodInfo
{
    int32_t invoker_index;
    int32_t method_pointer_index;

    MonoMethodInfo() :
        invoker_index(-1), method_pointer_index(-1) {}

    MonoMethodInfo(int32_t invoker_index_, int32_t method_pointer_index_) :
        invoker_index(invoker_index_), method_pointer_index(method_pointer_index_) {}
};

struct MonoRGCTXDefinition
{
    Il2CppRGCTXDataType type;
    const char* imageName;
    int32_t token;
    int32_t generic_parameter_index;
};

struct RuntimeGenericContextInfo
{
    int32_t token;
    int32_t rgctxStart;
    int32_t rgctxCount;
};

struct ImageRuntimeGenericContextTokens
{
    const char* name;
    int size;
    const RuntimeGenericContextInfo* tokens;
};

struct MonoMetadataToken
{
    const char* image;
    int32_t token;
};

struct MonoMethodMetadata
{
    MonoMetadataToken metadataToken;
    MethodIndex reversePInvokeWrapperIndex;
};

struct MonoClassMetadata
{
    MonoMetadataToken metadataToken;
    int32_t numberOfGenericParameters;
    TypeIndex *genericParameterTypeIndices;
    bool isPointer;
    int rank; //if rank == 0, the token is for a non-array type, otherwise the rank is valid and the token represents the element type of the array
    int32_t elementTypeIndex;
};

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

typedef Il2CppHashMap<uint64_t, MonoMethodInfo, il2cpp::utils::PassThroughHash<uint64_t> > MonoMethodInfoMap;

class MonoMethodInfoMapContainerBase
{
protected:
    MonoMethodInfoMap m_map;

public:
    const MonoMethodInfoMap& map() const { return m_map; }
};
