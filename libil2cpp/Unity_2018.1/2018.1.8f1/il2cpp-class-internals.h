#pragma once

#include "il2cpp-config.h"
#include <stdint.h>
#include "il2cpp-runtime-metadata.h"
#include "il2cpp-metadata.h"


#define IL2CPP_CLASS_IS_ARRAY(c) ((c)->rank)

typedef struct Il2CppClass Il2CppClass;
typedef struct Il2CppGuid Il2CppGuid;
typedef struct Il2CppImage Il2CppImage;
typedef struct Il2CppAppDomain Il2CppAppDomain;
typedef struct Il2CppAppDomainSetup Il2CppAppDomainSetup;
typedef struct Il2CppDelegate Il2CppDelegate;
typedef struct Il2CppAppContext Il2CppAppContext;
typedef struct Il2CppNameToTypeDefinitionIndexHashTable Il2CppNameToTypeDefinitionIndexHashTable;

#if RUNTIME_MONO
extern "C"
{
#include <mono/metadata/metadata.h>
}
#endif

typedef struct VirtualInvokeData
{
    Il2CppMethodPointer methodPtr;
#if RUNTIME_MONO
    const MonoMethod* method;
#else
    const MethodInfo* method;
#endif
} VirtualInvokeData;

typedef enum Il2CppTypeNameFormat
{
    IL2CPP_TYPE_NAME_FORMAT_IL,
    IL2CPP_TYPE_NAME_FORMAT_REFLECTION,
    IL2CPP_TYPE_NAME_FORMAT_FULL_NAME,
    IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED
} Il2CppTypeNameFormat;


typedef struct Il2CppDefaults
{
    Il2CppImage *corlib;
    Il2CppClass *object_class;
    Il2CppClass *byte_class;
    Il2CppClass *void_class;
    Il2CppClass *boolean_class;
    Il2CppClass *sbyte_class;
    Il2CppClass *int16_class;
    Il2CppClass *uint16_class;
    Il2CppClass *int32_class;
    Il2CppClass *uint32_class;
    Il2CppClass *int_class;
    Il2CppClass *uint_class;
    Il2CppClass *int64_class;
    Il2CppClass *uint64_class;
    Il2CppClass *single_class;
    Il2CppClass *double_class;
    Il2CppClass *char_class;
    Il2CppClass *string_class;
    Il2CppClass *enum_class;
    Il2CppClass *array_class;
    Il2CppClass *delegate_class;
    Il2CppClass *multicastdelegate_class;
    Il2CppClass *asyncresult_class;
    Il2CppClass *manualresetevent_class;
    Il2CppClass *typehandle_class;
    Il2CppClass *fieldhandle_class;
    Il2CppClass *methodhandle_class;
    Il2CppClass *systemtype_class;
    Il2CppClass *monotype_class;
    Il2CppClass *exception_class;
    Il2CppClass *threadabortexception_class;
    Il2CppClass *thread_class;
#if NET_4_0
    Il2CppClass *internal_thread_class;
#endif
    /*Il2CppClass *transparent_proxy_class;
    Il2CppClass *real_proxy_class;
    Il2CppClass *mono_method_message_class;*/
    Il2CppClass *appdomain_class;
    Il2CppClass *appdomain_setup_class;
    Il2CppClass *field_info_class;
    Il2CppClass *method_info_class;
    Il2CppClass *property_info_class;
    Il2CppClass *event_info_class;
    Il2CppClass *mono_event_info_class;
    Il2CppClass *stringbuilder_class;
    /*Il2CppClass *math_class;*/
    Il2CppClass *stack_frame_class;
    Il2CppClass *stack_trace_class;
    Il2CppClass *marshal_class;
    /*Il2CppClass *iserializeable_class;
    Il2CppClass *serializationinfo_class;
    Il2CppClass *streamingcontext_class;*/
    Il2CppClass *typed_reference_class;
    /*Il2CppClass *argumenthandle_class;*/
    Il2CppClass *marshalbyrefobject_class;
    /*Il2CppClass *monitor_class;
    Il2CppClass *iremotingtypeinfo_class;
    Il2CppClass *runtimesecurityframe_class;
    Il2CppClass *executioncontext_class;
    Il2CppClass *internals_visible_class;*/
    Il2CppClass *generic_ilist_class;
    Il2CppClass *generic_icollection_class;
    Il2CppClass *generic_ienumerable_class;
#if NET_4_0
    Il2CppClass *generic_ireadonlylist_class;
    Il2CppClass *generic_ireadonlycollection_class;
    Il2CppClass *runtimetype_class;
#endif
    Il2CppClass *generic_nullable_class;
    /*Il2CppClass *variant_class;
    Il2CppClass *com_object_class;*/
    Il2CppClass *il2cpp_com_object_class;
    /*Il2CppClass *com_interop_proxy_class;
    Il2CppClass *iunknown_class;
    Il2CppClass *idispatch_class;
    Il2CppClass *safehandle_class;
    Il2CppClass *handleref_class;
    Il2CppClass *attribute_class;*/
    Il2CppClass *customattribute_data_class;
    //Il2CppClass *critical_finalizer_object;
    Il2CppClass *version;
    Il2CppClass *culture_info;
    Il2CppClass *async_call_class;
    Il2CppClass *assembly_class;
#if NET_4_0
    Il2CppClass *mono_assembly_class;
#endif
    Il2CppClass *assembly_name_class;
#if !NET_4_0
    Il2CppClass *enum_info_class;
#endif
    Il2CppClass *mono_field_class;
    Il2CppClass *mono_method_class;
    Il2CppClass *mono_method_info_class;
    Il2CppClass *mono_property_info_class;
    Il2CppClass *parameter_info_class;
#if NET_4_0
    Il2CppClass *mono_parameter_info_class;
#endif
    Il2CppClass *module_class;
    Il2CppClass *pointer_class;
    Il2CppClass *system_exception_class;
    Il2CppClass *argument_exception_class;
    Il2CppClass *wait_handle_class;
    Il2CppClass *safe_handle_class;
    Il2CppClass *sort_key_class;
    Il2CppClass *dbnull_class;
    Il2CppClass *error_wrapper_class;
    Il2CppClass *missing_class;
    Il2CppClass *value_type_class;

#if NET_4_0
    // Stuff used by the mono code
    Il2CppClass *threadpool_wait_callback_class;
    MethodInfo *threadpool_perform_wait_callback_method;
    Il2CppClass *mono_method_message_class;
#endif

    // Windows.Foundation.IReference`1<T>
    Il2CppClass* ireference_class;
    // Windows.Foundation.Collections.IKeyValuePair`2<K, V>
    Il2CppClass* ikey_value_pair_class;
    // System.Collections.Generic.KeyValuePair`2<K, V>
    Il2CppClass* key_value_pair_class;
    // Windows.Foundation.Uri
    Il2CppClass* windows_foundation_uri_class;
    // Windows.Foundation.IUriRuntimeClass
    Il2CppClass* windows_foundation_iuri_runtime_class_class;
    // System.Uri
    Il2CppClass* system_uri_class;
} Il2CppDefaults;

extern LIBIL2CPP_CODEGEN_API Il2CppDefaults il2cpp_defaults;

struct Il2CppClass;
struct MethodInfo;
struct FieldInfo;
struct Il2CppObject;
struct MemberInfo;

typedef struct CustomAttributesCache
{
    int count;
    Il2CppObject** attributes;
} CustomAttributesCache;

typedef struct CustomAttributeTypeCache
{
    int count;
    Il2CppClass** attributeTypes;
} CustomAttributeTypeCache;

typedef void (*CustomAttributesCacheGenerator)(CustomAttributesCache*);

const int THREAD_STATIC_FIELD_OFFSET = -1;

typedef struct FieldInfo
{
    const char* name;
    const Il2CppType* type;
    Il2CppClass *parent;
    int32_t offset; // If offset is -1, then it's thread static
    CustomAttributeIndex customAttributeIndex;
    uint32_t token;
} FieldInfo;

typedef struct PropertyInfo
{
    Il2CppClass *parent;
    const char *name;
    const MethodInfo *get;
    const MethodInfo *set;
    uint32_t attrs;
    CustomAttributeIndex customAttributeIndex;
    uint32_t token;
} PropertyInfo;

typedef struct EventInfo
{
    const char* name;
    const Il2CppType* eventType;
    Il2CppClass* parent;
    const MethodInfo* add;
    const MethodInfo* remove;
    const MethodInfo* raise;
    CustomAttributeIndex customAttributeIndex;
    uint32_t token;
} EventInfo;

typedef struct ParameterInfo
{
    const char* name;
    int32_t position;
    uint32_t token;
    CustomAttributeIndex customAttributeIndex;
    const Il2CppType* parameter_type;
} ParameterInfo;

#if RUNTIME_MONO
typedef void* (*InvokerMethod)(Il2CppMethodPointer, const MonoMethod*, void*, void**);
#else
typedef void* (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo*, void*, void**);
#endif

typedef enum MethodVariableKind
{
    kMethodVariableKind_This,
    kMethodVariableKind_Parameter,
    kMethodVariableKind_LocalVariable
} MethodVariableKind;

typedef enum SequencePointKind
{
    kSequencePointKind_Normal,
    kSequencePointKind_StepOut
} SequencePointKind;

typedef struct Il2CppMethodExecutionContextInfo
{
#ifdef __cplusplus
private:
#endif
#if RUNTIME_MONO
    const MonoType** m_type;
#else
    const Il2CppType** m_type;
#endif
    const char* m_name;
    MethodVariableKind m_variableKind;
    int m_start;
    int m_end;
#ifdef __cplusplus
public:
    Il2CppMethodExecutionContextInfo() :
        m_type(NULL), m_name(NULL), m_variableKind(kMethodVariableKind_This), m_start(0), m_end(0)
    {
    }

    Il2CppMethodExecutionContextInfo(const Il2CppMethodExecutionContextInfo& other) :
        m_type(other.m_type), m_name(other.m_name), m_variableKind(other.m_variableKind), m_start(other.m_start), m_end(other.m_end)
    {
    }

#if RUNTIME_MONO
    Il2CppMethodExecutionContextInfo(const MonoType** type, const char* name, MethodVariableKind variableKind) :
        m_type(type), m_name(name), m_variableKind(variableKind)
    {
    }

    Il2CppMethodExecutionContextInfo(const MonoType** type, const char* name, MethodVariableKind variableKind, int start, int end) :
        m_type(type), m_name(name), m_variableKind(variableKind), m_start(start), m_end(end)
    {
    }

#else
    Il2CppMethodExecutionContextInfo(const Il2CppType** type, const char* name, MethodVariableKind variableKind) :
        m_type(type), m_name(name), m_variableKind(variableKind), m_start(0), m_end(0)
    {
    }

    Il2CppMethodExecutionContextInfo(const Il2CppType** type, const char* name, MethodVariableKind variableKind, int start, int end) :
        m_type(type), m_name(name), m_variableKind(variableKind), m_start(start), m_end(end)
    {
    }

#endif

    const Il2CppMethodExecutionContextInfo& operator=(const Il2CppMethodExecutionContextInfo& other)
    {
        m_type = other.m_type;
        m_name = other.m_name;
        m_variableKind = other.m_variableKind;
        m_start = other.m_start;
        m_end = other.m_end;
        return *this;
    }

    const char* name() const { return m_name; }
    MethodVariableKind variableKind() const { return m_variableKind; }
    int start() const { return m_start; }
    int end() const { return m_end; }

#if RUNTIME_MONO
    const MonoType** type() const { return m_type; }
#else
    const Il2CppType** type() const { return m_type; }
#endif
#endif //__cplusplus
} Il2CppMethodExecutionContextInfo;

typedef struct Il2CppMethodScope
{
    int startOffset;
    int endOffset;
} Il2CppMethodScope;

typedef struct Il2CppMethodHeaderInfo
{
#ifdef __cplusplus
private:
#endif //__cplusplus
    int m_codeSize;
    int m_numScopes;
    Il2CppMethodScope *m_scopes;

#ifdef __cplusplus
public:
    Il2CppMethodHeaderInfo() :
        m_codeSize(0), m_numScopes(0), m_scopes(NULL)
    {
    }

    ~Il2CppMethodHeaderInfo()
    {
        if (m_scopes)
            delete[] m_scopes;
    }

    void setScope(int index, int startOffset, int endOffset)
    {
        m_scopes[index].startOffset = startOffset;
        m_scopes[index].endOffset = endOffset;
    }

    int codeSize() const { return m_codeSize; }
    void setCodeSize(int codeSize) { m_codeSize = codeSize; }

    int numScopes() const { return m_numScopes; }
    void setNumScopes(int numScopes)
    {
        m_numScopes = numScopes;

        if (m_scopes)
            delete[] m_scopes;

        m_scopes = new Il2CppMethodScope[numScopes];
    }

    const Il2CppMethodScope* scopes() const { return m_scopes; }
#endif //__cplusplus
} Il2CppMethodHeaderInfo;

typedef struct Hash16
{
#ifdef __cplusplus
    Hash16()
    {
        for (int i = 0; i < 16; ++i)
            m_hash[i] = 0;
    }

    Hash16(const Hash16& hash)
    {
        *this = hash;
    }

    Hash16(uint8_t h1, uint8_t h2, uint8_t h3, uint8_t h4, uint8_t h5, uint8_t h6, uint8_t h7, uint8_t h8, uint8_t h9, uint8_t h10, uint8_t h11, uint8_t h12, uint8_t h13, uint8_t h14, uint8_t h15, uint8_t h16)
    {
        m_hash[0] = h1;
        m_hash[1] = h2;
        m_hash[2] = h3;
        m_hash[3] = h4;
        m_hash[4] = h5;
        m_hash[5] = h6;
        m_hash[6] = h7;
        m_hash[7] = h8;
        m_hash[8] = h9;
        m_hash[9] = h10;
        m_hash[10] = h11;
        m_hash[11] = h12;
        m_hash[12] = h13;
        m_hash[13] = h14;
        m_hash[14] = h15;
        m_hash[15] = h16;
    }

    uint8_t operator[](int i) const
    {
        return m_hash[i];
    }

    const Hash16& operator=(const Hash16& hash)
    {
        for (int i = 0; i < 16; ++i)
            m_hash[i] = hash.m_hash[i];

        return *this;
    }

private:
#endif
    uint8_t m_hash[16];
} Hash16;


typedef struct Il2CppSequencePoint
{
    const Il2CppMethodExecutionContextInfo* executionContextInfos;
    uint32_t executionContextInfoCount;
    const Il2CppMethodHeaderInfo* header;
#if RUNTIME_MONO
    const MonoMethod* method;
    const MonoClass* catchType;
#else
    const MethodInfo* method;
    const Il2CppClass* catchType;
#endif
    const char* sourceFile;
    Hash16 sourceFileHash;
    int32_t lineStart, lineEnd;
    int32_t columnStart, columnEnd;
    int32_t ilOffset;
    SequencePointKind kind;
    uint8_t isActive;
    int id;
    uint8_t tryDepth;

#ifdef __cplusplus
    Il2CppSequencePoint() : executionContextInfos(NULL), executionContextInfoCount(0), header(NULL), method(NULL), sourceFile(NULL), lineStart(0), lineEnd(0), columnStart(0), columnEnd(0),
        ilOffset(0), kind(kSequencePointKind_Normal), isActive(0), id(0), tryDepth(0), catchType(NULL)
    {
    }

#endif //__cplusplus
} Il2CppSequencePoint;

typedef union Il2CppRGCTXData
{
    void* rgctxDataDummy;
    const MethodInfo* method;
    const Il2CppType* type;
    Il2CppClass* klass;
} Il2CppRGCTXData;

typedef struct MethodInfo
{
    Il2CppMethodPointer methodPointer;
    InvokerMethod invoker_method;
    const char* name;
    Il2CppClass *klass;
    const Il2CppType *return_type;
    const ParameterInfo* parameters;

    union
    {
        const Il2CppRGCTXData* rgctx_data; /* is_inflated is true and is_generic is false, i.e. a generic instance method */
        const Il2CppMethodDefinition* methodDefinition;
    };

    /* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
    union
    {
        const Il2CppGenericMethod* genericMethod; /* is_inflated is true */
        const Il2CppGenericContainer* genericContainer; /* is_inflated is false and is_generic is true */
    };

    CustomAttributeIndex customAttributeIndex;
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1; /* true if method is a generic method definition */
    uint8_t is_inflated : 1; /* true if declaring_type is a generic instance or if method is a generic instance*/
    uint8_t wrapper_type : 1; /* always zero (MONO_WRAPPER_NONE) needed for the debugger */
    uint8_t is_marshaled_from_native : 1; /* a fake MethodInfo wrapping a native function pointer */
} MethodInfo;

typedef struct Il2CppRuntimeInterfaceOffsetPair
{
    Il2CppClass* interfaceType;
    int32_t offset;
} Il2CppRuntimeInterfaceOffsetPair;

typedef void (*PInvokeMarshalToNativeFunc)(void* managedStructure, void* marshaledStructure);
typedef void (*PInvokeMarshalFromNativeFunc)(void* marshaledStructure, void* managedStructure);
typedef void (*PInvokeMarshalCleanupFunc)(void* marshaledStructure);
typedef struct Il2CppIUnknown* (*CreateCCWFunc)(Il2CppObject* obj);

#if RUNTIME_MONO
#include "il2cpp-mapping.h"
#endif

typedef struct Il2CppInteropData
{
    Il2CppMethodPointer delegatePInvokeWrapperFunction;
    PInvokeMarshalToNativeFunc pinvokeMarshalToNativeFunction;
    PInvokeMarshalFromNativeFunc pinvokeMarshalFromNativeFunction;
    PInvokeMarshalCleanupFunc pinvokeMarshalCleanupFunction;
    CreateCCWFunc createCCWFunction;
    const Il2CppGuid* guid;
#if RUNTIME_MONO
    MonoMetadataToken typeToken;
    uint64_t hash;
#else
    const Il2CppType* type;
#endif
} Il2CppInteropData;

#if IL2CPP_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4200 )
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#endif

typedef struct Il2CppClass
{
    // The following fields are always valid for a Il2CppClass structure
    const Il2CppImage* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    Il2CppGenericClass *generic_class;
    const Il2CppTypeDefinition* typeDefinition; // non-NULL for Il2CppClass's constructed from type defintions
    const Il2CppInteropData* interopData;
    Il2CppClass* klass; // hack to pretend we are a MonoVTable. Points to ourself
    // End always valid fields

    // The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
    FieldInfo* fields; // Initialized in SetupFields
    const EventInfo* events; // Initialized in SetupEvents
    const PropertyInfo* properties; // Initialized in SetupProperties
    const MethodInfo** methods; // Initialized in SetupMethods
    Il2CppClass** nestedTypes; // Initialized in SetupNestedTypes
    Il2CppClass** implementedInterfaces; // Initialized in SetupInterfaces
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // Initialized in Init
    void* static_fields; // Initialized in Init
    const Il2CppRGCTXData* rgctx_data; // Initialized in Init
    // used for fast parent checks
    Il2CppClass** typeHierarchy; // Initialized in SetupTypeHierachy
    // End initialization required fields

    uint32_t cctor_started;
    uint32_t cctor_finished;
    ALIGN_TYPE(8) uint64_t cctor_thread;

    // Remaining fields are always valid except where noted
    GenericContainerIndex genericContainerIndex;
    CustomAttributeIndex customAttributeIndex;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;

    uint16_t method_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count; // lazily calculated for arrays, i.e. when rank > 0
    uint16_t interfaces_count;
    uint16_t interface_offsets_count; // lazily calculated for arrays, i.e. when rank > 0

    uint8_t typeHierarchyDepth; // Initialized in SetupTypeHierachy
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t packingSize;

    uint8_t valuetype : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    VirtualInvokeData vtable[IL2CPP_ZERO_LEN_ARRAY];
} Il2CppClass;

#if IL2CPP_COMPILER_MSVC
#pragma warning( pop )
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

// compiler calcualted values
typedef struct Il2CppTypeDefinitionSizes
{
    uint32_t instance_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
} Il2CppTypeDefinitionSizes;

typedef struct Il2CppDomain
{
    Il2CppAppDomain* domain;
    Il2CppAppDomainSetup* setup;
    Il2CppAppContext* default_context;
    const char* friendly_name;
    uint32_t domain_id;

#if NET_4_0
    volatile int threadpool_jobs;
#endif
    void* agent_info;
} Il2CppDomain;

typedef struct Il2CppAssemblyName
{
    const char* name;
    const char* culture;
    const char* hash_value;
    const char* public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[PUBLIC_KEY_BYTE_LENGTH];
} Il2CppAssemblyName;

typedef struct Il2CppImage
{
    const char* name;
    const char *nameNoExt;
    Il2CppAssembly* assembly;

    TypeDefinitionIndex typeStart;
    uint32_t typeCount;

    TypeDefinitionIndex exportedTypeStart;
    uint32_t exportedTypeCount;

    MethodIndex entryPointIndex;

#ifdef __cplusplus
    mutable
#endif
    Il2CppNameToTypeDefinitionIndexHashTable * nameToClassHashTable;

    uint32_t token;
    uint8_t dynamic;
} Il2CppImage;

typedef struct Il2CppAssembly
{
    Il2CppImage* image;
    CustomAttributeIndex customAttributeIndex;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
} Il2CppAssembly;

typedef struct Il2CppCodeGenOptions
{
    bool enablePrimitiveValueTypeGenericSharing;
} Il2CppCodeGenOptions;

typedef struct Il2CppCodeRegistration
{
    uint32_t methodPointersCount;
    const Il2CppMethodPointer* methodPointers;
    uint32_t reversePInvokeWrapperCount;
    const Il2CppMethodPointer* reversePInvokeWrappers;
    uint32_t genericMethodPointersCount;
    const Il2CppMethodPointer* genericMethodPointers;
    uint32_t invokerPointersCount;
    const InvokerMethod* invokerPointers;
    CustomAttributeIndex customAttributeCount;
    const CustomAttributesCacheGenerator* customAttributeGenerators;
    uint32_t unresolvedVirtualCallCount;
    const Il2CppMethodPointer* unresolvedVirtualCallPointers;
    uint32_t interopDataCount;
    Il2CppInteropData* interopData;
} Il2CppCodeRegistration;

typedef struct Il2CppMetadataRegistration
{
    int32_t genericClassesCount;
    Il2CppGenericClass* const * genericClasses;
    int32_t genericInstsCount;
    const Il2CppGenericInst* const * genericInsts;
    int32_t genericMethodTableCount;
    const Il2CppGenericMethodFunctionsDefinitions* genericMethodTable;
    int32_t typesCount;
    const Il2CppType* const * types;
    int32_t methodSpecsCount;
    const Il2CppMethodSpec* methodSpecs;

    FieldIndex fieldOffsetsCount;
    const int32_t** fieldOffsets;

    TypeDefinitionIndex typeDefinitionsSizesCount;
    const Il2CppTypeDefinitionSizes** typeDefinitionsSizes;
    const size_t metadataUsagesCount;
    void** const* metadataUsages;
} Il2CppMetadataRegistration;

typedef struct Il2CppRuntimeStats
{
    uint64_t new_object_count;
    uint64_t initialized_class_count;
    // uint64_t generic_vtable_count;
    // uint64_t used_class_count;
    uint64_t method_count;
    // uint64_t class_vtable_size;
    uint64_t class_static_data_size;
    uint64_t generic_instance_count;
    uint64_t generic_class_count;
    uint64_t inflated_method_count;
    uint64_t inflated_type_count;
    // uint64_t delegate_creations;
    // uint64_t minor_gc_count;
    // uint64_t major_gc_count;
    // uint64_t minor_gc_time_usecs;
    // uint64_t major_gc_time_usecs;
    bool enabled;
} Il2CppRuntimeStats;

extern Il2CppRuntimeStats il2cpp_runtime_stats;

/*
* new structure to hold performance counters values that are exported
* to managed code.
* Note: never remove fields from this structure and only add them to the end.
* Size of fields and type should not be changed as well.
*/
typedef struct Il2CppPerfCounters
{
    /* JIT category */
    uint32_t jit_methods;
    uint32_t jit_bytes;
    uint32_t jit_time;
    uint32_t jit_failures;
    /* Exceptions category */
    uint32_t exceptions_thrown;
    uint32_t exceptions_filters;
    uint32_t exceptions_finallys;
    uint32_t exceptions_depth;
    uint32_t aspnet_requests_queued;
    uint32_t aspnet_requests;
    /* Memory category */
    uint32_t gc_collections0;
    uint32_t gc_collections1;
    uint32_t gc_collections2;
    uint32_t gc_promotions0;
    uint32_t gc_promotions1;
    uint32_t gc_promotion_finalizers;
    uint32_t gc_gen0size;
    uint32_t gc_gen1size;
    uint32_t gc_gen2size;
    uint32_t gc_lossize;
    uint32_t gc_fin_survivors;
    uint32_t gc_num_handles;
    uint32_t gc_allocated;
    uint32_t gc_induced;
    uint32_t gc_time;
    uint32_t gc_total_bytes;
    uint32_t gc_committed_bytes;
    uint32_t gc_reserved_bytes;
    uint32_t gc_num_pinned;
    uint32_t gc_sync_blocks;
    /* Remoting category */
    uint32_t remoting_calls;
    uint32_t remoting_channels;
    uint32_t remoting_proxies;
    uint32_t remoting_classes;
    uint32_t remoting_objects;
    uint32_t remoting_contexts;
    /* Loader category */
    uint32_t loader_classes;
    uint32_t loader_total_classes;
    uint32_t loader_appdomains;
    uint32_t loader_total_appdomains;
    uint32_t loader_assemblies;
    uint32_t loader_total_assemblies;
    uint32_t loader_failures;
    uint32_t loader_bytes;
    uint32_t loader_appdomains_uloaded;
    /* Threads and Locks category  */
    uint32_t thread_contentions;
    uint32_t thread_queue_len;
    uint32_t thread_queue_max;
    uint32_t thread_num_logical;
    uint32_t thread_num_physical;
    uint32_t thread_cur_recognized;
    uint32_t thread_num_recognized;
    /* Interop category */
    uint32_t interop_num_ccw;
    uint32_t interop_num_stubs;
    uint32_t interop_num_marshals;
    /* Security category */
    uint32_t security_num_checks;
    uint32_t security_num_link_checks;
    uint32_t security_time;
    uint32_t security_depth;
    uint32_t unused;
    /* Threadpool */
    uint64_t threadpool_workitems;
    uint64_t threadpool_ioworkitems;
    unsigned int threadpool_threads;
    unsigned int threadpool_iothreads;
} Il2CppPerfCounters;
