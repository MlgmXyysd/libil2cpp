#if RUNTIME_MONO

#include <cassert>

#include "class-internals.h"
#include "object-internals.h"

#include "il2cpp-mapping.h"
#include "il2cpp-mono-support.h"

#include "../libmono/mono-api.h"
#include "utils/dynamic_array.h"
#if IL2CPP_ENABLE_NATIVE_STACKTRACES
#include "utils/NativeSymbol.h"
#endif // IL2CPP_ENABLE_NATIVE_STACKTRACES
#include "utils/MarshalingUtils.h"

Il2CppIntPtr Il2CppIntPtr::Zero;

extern const Il2CppCodeRegistration g_CodeRegistration;
extern int g_RuntimeGenericContextMappingSize;
extern const ImageRuntimeGenericContextTokens g_RuntimeGenericContextMapping[];
extern const MonoRGCTXDefinition g_RuntimeGenericContextIndices[];
extern const Il2CppMethodSpec g_Il2CppMethodSpecTable[];
#if IL2CPP_ENABLE_NATIVE_STACKTRACES
extern MethodIndex g_MethodIndices[];
extern MethodIndex g_GenericMethodIndices[];
#endif // IL2CPP_ENABLE_NATIVE_STACKTRACES

// Mono-specific metadata emitted by IL2CPP
extern const int g_MetadataUsageListsCount;
extern Il2CppMetadataUsageList g_MetadataUsageLists[];
extern const int g_MetadataUsagePairsCount;
extern Il2CppMetadataUsagePair g_MetadataUsagePairs[];
extern void** const g_MetadataUsages[];
extern const int g_MonoTypeMetadataCount;
extern MonoClassMetadata g_MonoTypeMetadataTable[];
extern const int g_MonoFieldMetadataCount;
extern MonoFieldMetadata g_MonoFieldMetadataTable[];
extern const int g_MonoMethodMetadataCount;
extern MonoMethodMetadata g_MonoMethodMetadataTable[];
extern const MonoGenericInstMetadata* const g_MonoGenericInstMetadataTable[];
extern const int g_MonoStringMetadataCount;
extern MonoMetadataToken g_MonoStringMetadataTable[];
extern const Il2CppCodeGenOptions s_Il2CppCodeGenOptions;

extern const MonoMethodInfoMapContainerBase* g_pGenericMethodsMapContainer;
extern const MonoMethodInfoMapContainerBase* g_pMethodsMapContainer;

extern const int g_Il2CppInteropDataCount;
extern Il2CppInteropData g_Il2CppInteropData[];

static MonoGenericContext GetSharedContext(const MonoGenericContext* context);

static MonoGenericInst* GetSharedGenericInst(MonoGenericInst* inst)
{
    std::vector<MonoType*> types;
    unsigned int type_argc = mono_unity_generic_inst_get_type_argc(inst);
    for (uint32_t i = 0; i < type_argc; ++i)
    {
        MonoType *type = mono_unity_generic_inst_get_type_argument(inst, i);

        if (mono_type_is_reference(type))
        {
            types.push_back(mono_class_get_type(mono_get_object_class()));
        }
        else if (mono_unity_type_is_generic_instance(type))
        {
            MonoGenericClass *gclass =  mono_unity_type_get_generic_class(type);
            MonoGenericContext context = mono_unity_generic_class_get_context(gclass);
            MonoGenericContext sharedContext = GetSharedContext(&context);
            MonoClass *container_class = mono_unity_generic_class_get_container_class(gclass);
            MonoError unused;
            MonoClass *klass = mono_class_inflate_generic_class_checked(container_class, &sharedContext, &unused);
            types.push_back(mono_class_get_type(klass));
        }
        else if (mono_unity_type_is_enum_type(type) && s_Il2CppCodeGenOptions.enablePrimitiveValueTypeGenericSharing)
        {
            types.push_back(mono_class_get_type(mono_unity_type_get_element_class(type)));
        }
        else if (mono_unity_type_is_boolean(type) && s_Il2CppCodeGenOptions.enablePrimitiveValueTypeGenericSharing)
        {
            types.push_back(mono_class_get_type(mono_unity_defaults_get_byte_class()));
        }
        else
        {
            types.push_back(mono_unity_generic_inst_get_type_argument(inst, i));
        }
    }

    return mono_metadata_get_generic_inst(type_argc, &types[0]);
}

static MonoGenericContext GetSharedContext(const MonoGenericContext* context)
{
    MonoGenericContext newContext = {0};
    if (context->class_inst != NULL)
        newContext.class_inst = GetSharedGenericInst(context->class_inst);

    if (context->method_inst != NULL)
        newContext.method_inst = GetSharedGenericInst(context->method_inst);

    return newContext;
}

static MonoMethod* InflateGenericMethodWithContext(MonoMethod* method, MonoGenericContext* context, bool useSharedVersion)
{
    if (useSharedVersion)
    {
        MonoGenericContext sharedContext = GetSharedContext(context);
        return mono_class_inflate_generic_method(method, &sharedContext);
    }

    return mono_class_inflate_generic_method(method, context);
}

const Il2CppInteropData* FindInteropDataFor(MonoClass* klass)
{
    uint32_t token = mono_class_get_type_token(klass);
    const char* imageName = mono_unity_class_get_image_name(klass);
    for (int i = 0; i < g_Il2CppInteropDataCount; ++i)
    {
        if (token == g_Il2CppInteropData[i].typeToken.token && strcmp(imageName, g_Il2CppInteropData[i].typeToken.image) == 0)
            return &g_Il2CppInteropData[i];
    }

    return NULL;
}

static void StructureToPtr(MonoObject* structure, void* ptr, bool deleteOld)
{
    if (structure == NULL)
        mono_raise_exception(mono_get_exception_argument_null("structure"));

    if (ptr == NULL)
        mono_raise_exception(mono_get_exception_argument_null("ptr"));

    MonoClass* klass = mono_object_get_class(structure);
    const Il2CppInteropData* interopData = FindInteropDataFor(klass);

    if (interopData != NULL && interopData->pinvokeMarshalToNativeFunction != NULL)
    {
        if (deleteOld)
            il2cpp::utils::MarshalingUtils::MarshalFreeStruct(ptr, interopData);

        void* objectPtr = mono_unity_class_is_class_type(klass) ? structure : mono_object_unbox(structure);
        il2cpp::utils::MarshalingUtils::MarshalStructToNative(objectPtr, ptr, interopData);
        return;
    }

    // If there's no custom marshal function, it means it's either a primitive, or invalid argument

    uint32_t nativeSize = mono_unity_class_get_native_size(klass);
    if (nativeSize != -1)
    {
        // StructureToPtr is supposed to throw on strings and enums
        if (!mono_class_is_enum(klass) && !mono_unity_class_is_string(klass))
        {
            memcpy(ptr, mono_object_unbox(structure), nativeSize);
            return;
        }
    }

    // If we got this far, throw an exception
    MonoException* exception;

    if (mono_class_is_generic(klass))
    {
        exception = mono_get_exception_argument("structure", "The specified object must not be an instance of a generic type.");
    }
    else
    {
        exception = mono_get_exception_argument("structure", "The specified structure must be blittable or have layout information.");
    }

    mono_raise_exception(exception);
}

static void* MarshalInvokerMethod(Il2CppMethodPointer func, const MonoMethod* method, void* thisArg, void** params)
{
    if (strcmp(mono_unity_method_get_name(method), "PtrToStructure") == 0)
    {
        const Il2CppInteropData* interopData = FindInteropDataFor(mono_unity_method_get_class(method));
        if (interopData != NULL && mono_unity_class_is_class_type(mono_unity_method_get_class(method)))
            il2cpp::utils::MarshalingUtils::MarshalStructFromNative((void*)(*(intptr_t*)params[0]), params[1], interopData);
        else if (interopData != NULL)
            il2cpp::utils::MarshalingUtils::MarshalStructFromNative((void*)(*(intptr_t*)params[0]), mono_object_unbox((MonoObject*)params[1]), interopData);
        else // The type is blittable
            memcpy(mono_object_unbox((MonoObject*)params[1]), (void*)(*(intptr_t*)params[0]), mono_unity_class_get_native_size(mono_unity_method_get_class(method)));
    }
    else if (strcmp(mono_unity_method_get_name(method), "StructureToPtr") == 0)
        StructureToPtr((MonoObject*)params[0], (void*)(*(intptr_t*)params[1]), *(bool*)params[2]);
    else
        assert(0 && "Handle another special marshaling method");

    return (MonoObject*)params[1];
}

void il2cpp_mono_method_initialize_function_pointers(MonoMethod* method, MonoError* error)
{
    bool* isSpecialMarshalingMethod = NULL;
    assert(method != NULL);

    mono_error_init(error);

    int32_t invokerIndex = -1;
    int32_t methodPointerIndex = -1;

    if (mono_unity_method_get_invoke_pointer(method) || mono_unity_method_get_method_pointer(method))
        return;

    //char *methodName = mono_method_get_name_full(method, true, false, MONO_TYPE_NAME_FORMAT_IL);

    if (unity_mono_method_is_inflated(method))
    {
        // Use the fully shared version to look up the method pointer, as we will invoke the fully shared version.
        MonoMethod* definition = mono_method_get_method_definition(method);
        MonoMethod* sharedMethod = InflateGenericMethodWithContext(definition, mono_method_get_context(method), true);

        //char *newMethodName = mono_method_get_name_full(sharedMethod, true, false, MONO_TYPE_NAME_FORMAT_IL);

        uint64_t hash = mono_unity_method_get_hash(sharedMethod);
        const MonoMethodInfoMap& infoMap = g_pGenericMethodsMapContainer->map();
        MonoMethodInfoMap::const_iterator itInfo = infoMap.find(hash);
        if (itInfo != infoMap.end())
        {
            invokerIndex = itInfo->second.invoker_index;
            methodPointerIndex = itInfo->second.method_pointer_index;
        }
        else
        {
            mono_error_set_execution_engine(error, "Unable to create the method '%s' at runtime", mono_unity_method_get_name(method));
            return;
        }

        assert(invokerIndex != -1 && "The method does not have an invoker, is it a generic method?");
        assert((uint32_t)invokerIndex < g_CodeRegistration.invokerPointersCount);
        mono_unity_method_set_invoke_pointer(method, (void*)g_CodeRegistration.invokerPointers[invokerIndex]);

        assert((uint32_t)methodPointerIndex < g_CodeRegistration.genericMethodPointersCount);
        mono_unity_method_set_method_pointer(method, (void*)g_CodeRegistration.genericMethodPointers[methodPointerIndex]);
    }
    else
    {
        const MonoMethodInfoMap& infoMap = g_pMethodsMapContainer->map();
        uint64_t hash = mono_unity_method_get_hash(method);
        MonoMethodInfoMap::const_iterator itInfo = infoMap.find(hash);
        if (itInfo != infoMap.end())
        {
            invokerIndex = itInfo->second.invoker_index;
            methodPointerIndex = itInfo->second.method_pointer_index;
        }
        else if (strcmp(mono_unity_method_get_name(method), "PtrToStructure") == 0 || strcmp(mono_unity_method_get_name(method), "StructureToPtr") == 0)
        {
            mono_unity_method_set_invoke_pointer(method, (void*)&MarshalInvokerMethod);
            return;
        }
        else
        {
            // Throw a missing method exception if we did not convert it. Once we know this code works for all runtime invoke cases,
            // we should throw this exception more often. Until then, we will leave theassert below when we don't find a method.
            mono_error_set_method_load(error, mono_unity_method_get_class(method), mono_unity_method_get_name(method), "This method was not converted ahead-of-time by IL2CPP.");
            return;
        }

        assert(invokerIndex != -1 && "The method does not have an invoker, is it a generic method?");
        assert((uint32_t)invokerIndex < g_CodeRegistration.invokerPointersCount);
        mono_unity_method_set_invoke_pointer(method, (void*)g_CodeRegistration.invokerPointers[invokerIndex]);

        assert((uint32_t)methodPointerIndex < g_CodeRegistration.methodPointersCount);
        mono_unity_method_set_method_pointer(method, (void*)g_CodeRegistration.methodPointers[methodPointerIndex]);
    }
}

MonoAssembly* il2cpp_mono_assembly_from_name(const char* name)
{
    // First look for the cached assembly in the domain.
    MonoAssembly* assembly = mono_domain_assembly_open(mono_domain_get(), name);
    if (assembly == NULL)
    {
        // If we can't find it, look in the path we've set to find assemblies.
        MonoImageOpenStatus unused;
        assembly = mono_assembly_load_with_partial_name(name, &unused);
    }

    return assembly;
}

MonoMethod* il2cpp_mono_get_virtual_target_method(MonoMethod* method, MonoObject* obj)
{
    // This is *very* slow, and likely not good enough for production. It is good enough for proof of concept though.
    MonoMethod* target_method = mono_object_get_virtual_method(obj, method);

    if (mono_unity_class_is_array(mono_unity_method_get_class(target_method)))
        target_method = mono_unity_method_get_aot_array_helper_from_wrapper(target_method);

    return target_method;
}

void il2cpp_mono_get_invoke_data(MonoMethod* method, void* obj, VirtualInvokeData* invokeData)
{
    MonoMethod* target_method = il2cpp_mono_get_virtual_target_method(method, (MonoObject*)obj);

    MonoError error;
    il2cpp_mono_method_initialize_function_pointers(target_method, &error);
    if (!mono_error_ok(&error))
        mono_error_raise_exception(&error);
    invokeData->methodPtr = (Il2CppMethodPointer)mono_unity_method_get_method_pointer(target_method);
    invokeData->method = (MethodInfo*)target_method;
}

static MonoClass* InflateGenericParameter(MonoGenericContainer* genericContainer, MonoGenericContext* context, const MonoRGCTXDefinition* rgctxDefintion)
{
    MonoClass* genericParameter = mono_unity_generic_container_get_parameter_class(genericContainer, rgctxDefintion->generic_parameter_index);
    return mono_unity_class_inflate_generic_class(genericParameter, context);
}

static MonoClass* LookupGenericClassMetadata(MonoGenericContext* context, const MonoRGCTXDefinition* rgctxDefintion)
{
    return mono_class_get_full(mono_assembly_get_image(il2cpp_mono_assembly_from_name(rgctxDefintion->imageName)), rgctxDefintion->token, context);
}

static void* LookupMetadataFromRGCTX(const MonoRGCTXDefinition* definition, bool useSharedVersion, MonoGenericContext* context, MonoGenericContainer* genericContainer)
{
    if (definition->type == IL2CPP_RGCTX_DATA_CLASS)
    {
        if (mono_metadata_token_table(definition->token) == MONO_TABLE_GENERICPARAM)
            return InflateGenericParameter(genericContainer, context, definition);
        else if (mono_metadata_token_table(definition->token) == MONO_TABLE_TYPESPEC)
            return LookupGenericClassMetadata(context, definition);
        else
            assert(0 && "RGCTX case is not implemented yet.");
    }
    else if (definition->type == IL2CPP_RGCTX_DATA_ARRAY)
    {
        if (mono_metadata_token_table(definition->token) == MONO_TABLE_GENERICPARAM)
            return mono_array_class_get(InflateGenericParameter(genericContainer, context, definition), 1);
        else if (mono_metadata_token_table(definition->token) == MONO_TABLE_TYPESPEC)
            return mono_array_class_get(LookupGenericClassMetadata(context, definition), 1);
        else
            assert(0 && "RGCTX case is not implemented yet.");
    }
    else if (definition->type == IL2CPP_RGCTX_DATA_METHOD)
    {
        MonoMethod* methodDefinition = mono_get_method(mono_assembly_get_image(il2cpp_mono_assembly_from_name(definition->imageName)), definition->token, NULL);
        return InflateGenericMethodWithContext(methodDefinition, context, useSharedVersion);
    }
    else if (definition->type == IL2CPP_RGCTX_DATA_TYPE)
    {
        if (mono_metadata_token_table(definition->token) == MONO_TABLE_GENERICPARAM)
            return mono_class_get_type(InflateGenericParameter(genericContainer, context, definition));
        else if (mono_metadata_token_table(definition->token) == MONO_TABLE_TYPESPEC)
            return mono_class_get_type(LookupGenericClassMetadata(context, definition));
        else
            assert(0 && "RGCTX case is not implemented yet.");
    }
    else
    {
        assert(0 && "RGCTX case is not implemented yet.");
    }

    return NULL;
}

void* LookupMetadataFromToken(uint32_t token, const char* imageName, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion, MonoGenericContext* context, MonoGenericContainer* genericContainer)
{
    const RuntimeGenericContextInfo* rgctxInfo = NULL;
    for (int imageIndex = 0; imageIndex < g_RuntimeGenericContextMappingSize; ++imageIndex)
    {
        if (strcmp(g_RuntimeGenericContextMapping[imageIndex].name, imageName) == 0)
        {
            for (int typeIndex = 0; typeIndex < g_RuntimeGenericContextMapping[imageIndex].size; ++typeIndex)
            {
                if (token == g_RuntimeGenericContextMapping[imageIndex].tokens[typeIndex].token)
                {
                    rgctxInfo = &g_RuntimeGenericContextMapping[imageIndex].tokens[typeIndex];
                    if (rgctxInfo->rgctxStart != -1)
                    {
                        const MonoRGCTXDefinition* definition = &g_RuntimeGenericContextIndices[rgctxInfo->rgctxStart + rgctxIndex];
                        if (definition->type == rgctxType)
                            return LookupMetadataFromRGCTX(definition, useSharedVersion, context, genericContainer);
                    }
                }
            }
        }
    }

    return NULL;
}

void* il2cpp_mono_class_rgctx(MonoClass* klass, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion)
{
    return LookupMetadataFromToken(mono_class_get_type_token(klass), mono_unity_class_get_image_name(klass), rgctxType, rgctxIndex, useSharedVersion,
        mono_class_get_context(klass), mono_class_get_generic_container(mono_unity_class_get_generic_definition(klass)));
}

void* il2cpp_mono_method_rgctx(MonoMethod* method, Il2CppRGCTXDataType rgctxType, int rgctxIndex, bool useSharedVersion)
{
    return LookupMetadataFromToken(mono_unity_method_get_token(method), mono_unity_class_get_image_name(mono_unity_method_get_class(method)), rgctxType, rgctxIndex, useSharedVersion,
        mono_method_get_context(method), mono_method_get_generic_container(mono_unity_method_get_generic_definition(method)));
}

static MonoClass* ClassFromIndex(TypeIndex index);

static MonoClass* InflateGenericClass(MonoClass* generic_class, uint32_t num_indices, TypeIndex *generic_indices)
{
    std::vector<MonoType*> arguments(num_indices);
    for (uint32_t i = 0; i < num_indices; ++i)
        arguments[i] = mono_class_get_type(ClassFromIndex(generic_indices[i]));

    MonoGenericInst* generic_instance = mono_metadata_get_generic_inst(num_indices, &arguments[0]);
    MonoGenericContext context;
    context.class_inst = generic_instance;
    context.method_inst = NULL;
    MonoError unused;
    MonoClass* inflated_class = mono_class_inflate_generic_class_checked(generic_class, &context, &unused);
    mono_class_init(inflated_class);

    return inflated_class;
}

static MonoClass* ClassFromMetadata(const MonoClassMetadata *classMetadata)
{
    if (classMetadata->rank > 0)
    {
        MonoClass *klass = ClassFromIndex(classMetadata->elementTypeIndex);
        MonoClass *aklass = mono_array_class_get(klass, classMetadata->rank);
        mono_class_init(aklass);
        return aklass;
    }

    MonoClass *klass = mono_class_get(mono_assembly_get_image(il2cpp_mono_assembly_from_name(classMetadata->metadataToken.image)), classMetadata->metadataToken.token);
    mono_class_init(klass);
    return klass;
}

static MonoClass* ClassFromIndex(TypeIndex index)
{
    if (index == kTypeIndexInvalid)
        return NULL;

    assert(index < g_MonoTypeMetadataCount && "Invalid type index ");

    MonoClassMetadata* classMetadata = &g_MonoTypeMetadataTable[index];

    MonoClass* klass = ClassFromMetadata(classMetadata);
    if (classMetadata->numberOfGenericParameters != 0)
        klass = InflateGenericClass(klass, classMetadata->numberOfGenericParameters, classMetadata->genericParameterTypeIndices);

    if (classMetadata->isPointer)
        klass = mono_ptr_class_get(mono_class_get_type(klass));

    return klass;
}

static MonoClassField* FieldFromIndex(EncodedMethodIndex index)
{
    assert(index >= 0 && index <= static_cast<uint32_t>(g_MonoFieldMetadataCount));

    MonoFieldMetadata* fieldMetadata = &g_MonoFieldMetadataTable[index];
    return mono_class_get_field(ClassFromIndex(fieldMetadata->parentTypeIndex), fieldMetadata->token);
}

static MonoType* TypeFromIndex(TypeIndex index)
{
    if (index == kTypeIndexInvalid)
        return NULL;

    return mono_class_get_type(ClassFromIndex(index));
}

MonoMethod* MethodFromIndex(MethodIndex index)
{
    assert(index >= 0 && static_cast<int32_t>(index) <= g_MonoMethodMetadataCount);

    MonoMetadataToken* method = &g_MonoMethodMetadataTable[index].metadataToken;
    return mono_get_method(mono_assembly_get_image(il2cpp_mono_assembly_from_name(method->image)), method->token, NULL);
}

static std::vector<MonoType*> GenericArgumentsFromInst(const MonoGenericInstMetadata *inst)
{
    std::vector<MonoType*> arguments;

    uint32_t numberOfGenericArguments = inst->type_argc;
    if (numberOfGenericArguments == 0)
        return arguments;

    for (uint32_t i = 0; i < numberOfGenericArguments; ++i)
    {
        const TypeIndex argIndex = inst->type_argv_indices[i];
        arguments.push_back(mono_class_get_type(ClassFromIndex(argIndex)));
    }

    return arguments;
}

static MonoGenericInst* GenericInstFromIndex(GenericInstIndex index)
{
    if (index == -1)
        return NULL;

    const MonoGenericInstMetadata* inst = g_MonoGenericInstMetadataTable[index];

    // Replace this with dynamic_array later when we get the libil2cpp utilities build sorted out.
    std::vector<MonoType*> genericArguments = GenericArgumentsFromInst(inst);

    size_t numberOfGenericArguments = genericArguments.size();
    if (numberOfGenericArguments == 0)
        return NULL;

    return mono_metadata_get_generic_inst((int)numberOfGenericArguments, &genericArguments[0]);
}

MonoMethod* GenericMethodFromIndex(MethodIndex index)
{
    const Il2CppMethodSpec* methodSpec = &g_Il2CppMethodSpecTable[index];

    MonoMethod* methodDefinition = MethodFromIndex(methodSpec->methodDefinitionIndex);
    MonoGenericContext generic_context = { GenericInstFromIndex(methodSpec->classIndexIndex) , GenericInstFromIndex(methodSpec->methodIndexIndex) };

    return mono_class_inflate_generic_method(methodDefinition, &generic_context);
}

static MonoString* StringFromIndex(StringIndex index)
{
    assert(index >= 0 && static_cast<int32_t>(index) <= g_MonoStringMetadataCount);

    MonoMetadataToken* stringMetadata = &g_MonoStringMetadataTable[index];
    return mono_ldstr(mono_domain_get(), mono_assembly_get_image(il2cpp_mono_assembly_from_name(stringMetadata->image)), mono_metadata_token_index(stringMetadata->token));
}

void il2cpp_mono_initialize_method_metadata(uint32_t index)
{
    assert(g_MetadataUsageListsCount >= 0 && index <= static_cast<uint32_t>(g_MetadataUsageListsCount));

    uint32_t start = g_MetadataUsageLists[index].start;
    uint32_t count = g_MetadataUsageLists[index].count;

    for (uint32_t i = 0; i < count; i++)
    {
        uint32_t offset = start + i;

        assert(g_MetadataUsagePairsCount >= 0 && offset <= static_cast<uint32_t>(g_MetadataUsagePairsCount));
        uint32_t destinationIndex = g_MetadataUsagePairs[offset].destinationIndex;
        uint32_t encodedSourceIndex = g_MetadataUsagePairs[offset].encodedSourceIndex;

        Il2CppMetadataUsage usage = GetEncodedIndexType(encodedSourceIndex);
        uint32_t decodedIndex = GetDecodedMethodIndex(encodedSourceIndex);
        switch (usage)
        {
            case kIl2CppMetadataUsageTypeInfo:
                *g_MetadataUsages[destinationIndex] = ClassFromIndex(decodedIndex);
                break;
            case kIl2CppMetadataUsageFieldInfo:
                *g_MetadataUsages[destinationIndex] = FieldFromIndex(decodedIndex);
                break;
            case kIl2CppMetadataUsageIl2CppType:
                *g_MetadataUsages[destinationIndex] = TypeFromIndex(decodedIndex);
                break;
            case kIl2CppMetadataUsageMethodDef:
                *g_MetadataUsages[destinationIndex] = MethodFromIndex(decodedIndex);
                break;
            case kIl2CppMetadataUsageMethodRef:
                *g_MetadataUsages[destinationIndex] = GenericMethodFromIndex(decodedIndex);
                break;
            case kIl2CppMetadataUsageStringLiteral:
                *g_MetadataUsages[destinationIndex] = StringFromIndex(decodedIndex);
                break;
            default:
                assert(0 && "Unimplemented case for method metadata usage.");
                break;
        }
    }
}

void il2cpp_mono_raise_execution_engine_exception_if_method_is_not_found(MonoMethod* method)
{
    MonoError unused;
    il2cpp_mono_method_initialize_function_pointers(method, &unused);

    if (mono_unity_method_get_method_pointer(method) == NULL)
    {
        MonoException *exc;

        if (mono_unity_method_get_class(method) != NULL)
            exc = mono_get_exception_execution_engine(mono_method_get_name_full(method, true, false, MONO_TYPE_NAME_FORMAT_IL));
        else
            exc = mono_get_exception_execution_engine(mono_unity_method_get_name(method));

        mono_raise_exception(exc);
    }
}

std::vector<uintptr_t> il2cpp_to_mono_array_dimensions(il2cpp_array_size_t* il2cppDimensions, int32_t numberOfElements)
{
    std::vector<uintptr_t> monoDimensions(numberOfElements);
    for (int32_t i = 0; i < numberOfElements; ++i)
        monoDimensions[i] = il2cppDimensions[i];

    return monoDimensions;
}

Il2CppAsyncResult* il2cpp_mono_delegate_begin_invoke(Il2CppDelegate* delegate, void** params, Il2CppDelegate* asyncCallback, Il2CppObject* state)
{
    int numParams = mono_signature_get_param_count(mono_method_signature((MonoMethod*)delegate->method));
    Il2CppObject *nullState = NULL;
    Il2CppDelegate *nullCallback = NULL;

    std::vector<void*> newParams(numParams + 2);
    for (int i = 0; i < numParams; ++i)
        newParams[i] = params[i];

    //If the state and the callback are both null and there are no other params, we will send null for the params array
    newParams[numParams] = &nullCallback;
    newParams[numParams + 1] = &nullState;
    int numNewParams = numParams;

    if (asyncCallback)
    {
        newParams[numParams] = &asyncCallback;
        ++numNewParams;
    }

    if (state)
    {
        newParams[numParams + 1] = &state;
        ++numNewParams;
    }

    MonoError unused;
    return (Il2CppAsyncResult*)mono_threadpool_ms_begin_invoke(mono_domain_get(), (MonoObject*)delegate, (MonoMethod*)delegate->method, numNewParams != 0 ? &newParams[0] : NULL, &unused);
}

static void MonoArrayGetGenericValue(MonoArray* array, int32_t pos, void* value)
{
    MonoObject *obj = mono_array_get(array, MonoObject*, pos);
    MonoClass *klass = mono_unity_object_get_class(obj);

    if (mono_class_is_valuetype(klass))
        memcpy(value, mono_object_unbox(obj), mono_class_instance_size(klass) - sizeof(Il2CppObject));
    else
        memcpy(value, &obj, mono_unity_array_get_element_size(array));
}

Il2CppObject* il2cpp_mono_delegate_end_invoke(Il2CppAsyncResult* asyncResult, void **out_args)
{
    MonoError unused;
    MonoObject *exc = NULL;
    MonoArray *mono_out_args = NULL;
    Il2CppObject *retVal = (Il2CppObject*)mono_threadpool_ms_end_invoke((MonoAsyncResult*)asyncResult, &mono_out_args, &exc, &unused);

    if (exc)
        mono_raise_exception((MonoException*)exc);

    uint32_t numArgs = mono_unity_array_get_max_length(mono_out_args);

    for (uint32_t i = 0; i < numArgs; ++i)
        MonoArrayGetGenericValue(mono_out_args, i, out_args[i]);

    return retVal;
}

MonoArray* MonoArrayNew(MonoClass* elementType, uintptr_t length)
{
    MonoError unused;
    return mono_array_new_specific_checked(mono_class_vtable(mono_domain_get(), mono_array_class_get(elementType, 1)), length, &unused);
}

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
void RegisterAllManagedMethods()
{
    uint32_t numberOfMethods = g_CodeRegistration.methodPointersCount;
    uint32_t numberOfGenericMethods = g_CodeRegistration.genericMethodPointersCount;

    std::vector<MethodDefinitionKey> managedMethods(numberOfMethods + numberOfGenericMethods);

    for (uint32_t i = 0; i < numberOfMethods; ++i)
    {
        MethodDefinitionKey currentMethod;
        currentMethod.methodIndex = g_MethodIndices[i];
        currentMethod.method = g_CodeRegistration.methodPointers[i];
        currentMethod.isGeneric = false;
        managedMethods.push_back(currentMethod);
    }

    for (uint32_t i = 0; i < numberOfGenericMethods; ++i)
    {
        MethodDefinitionKey currentMethod;
        currentMethod.methodIndex = g_GenericMethodIndices[i];
        currentMethod.method = g_CodeRegistration.genericMethodPointers[i];
        currentMethod.isGeneric = true;
        managedMethods.push_back(currentMethod);
    }

    il2cpp::utils::NativeSymbol::RegisterMethods(managedMethods);
}

#endif // IL2CPP_ENABLE_NATIVE_STACKTRACES

void RuntimeInit(MonoClass* klass)
{
    MonoError error;
    mono_runtime_class_init_full(mono_class_vtable(mono_domain_get(), klass), &error);
    if (!mono_error_ok(&error))
        mono_error_raise_exception(&error);
}

extern "C"
{
void il2cpp_set_temp_dir(char *temp_dir)
{
    assert(0 && "This needs to be implemented for Android");
}
}

#endif //RUNTIME_MONO
