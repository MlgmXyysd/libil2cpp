#pragma once

#include "il2cpp-config.h"
#include <stdint.h>
#include "metadata.h"
#include "il2cpp-metadata.h"

#define IL2CPP_CLASS_IS_ARRAY(c) ((c)->rank)

struct Il2CppClass;
struct Il2CppGuid;
struct Il2CppImage;
struct Il2CppAssembly;
struct Il2CppAppDomain;
struct Il2CppDelegate;
struct Il2CppAppContext;
struct Il2CppNameToTypeDefinitionIndexHashTable;

enum Il2CppTypeNameFormat {
	IL2CPP_TYPE_NAME_FORMAT_IL,
	IL2CPP_TYPE_NAME_FORMAT_REFLECTION,
	IL2CPP_TYPE_NAME_FORMAT_FULL_NAME,
	IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED
};

extern bool g_il2cpp_is_fully_initialized;

typedef struct {
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
	Il2CppClass *assembly_name_class;
	Il2CppClass *enum_info_class;
	Il2CppClass *mono_field_class;
	Il2CppClass *mono_method_class;
	Il2CppClass *mono_method_info_class;
	Il2CppClass *mono_property_info_class;
	Il2CppClass *parameter_info_class;
	Il2CppClass *module_class;
	Il2CppClass *pointer_class;
	Il2CppClass *system_exception_class;
	Il2CppClass *argument_exception_class;
	Il2CppClass *wait_handle_class;
	Il2CppClass *safe_handle_class;
	Il2CppClass *sort_key_class;
} Il2CppDefaults;

extern Il2CppDefaults il2cpp_defaults;

struct Il2CppClass;
struct MethodInfo;
struct FieldInfo;
struct Il2CppObject;
struct MemberInfo;

struct CustomAttributesCache
{
	int count;
	Il2CppObject** attributes;
};

struct CustomAttributeTypeCache
{
	int count;
	Il2CppClass** attributeTypes;
};

typedef void (*CustomAttributesCacheGenerator)(CustomAttributesCache*);

const int THREAD_STATIC_FIELD_OFFSET = -1;

struct FieldInfo
{
	const char* name;
	const Il2CppType* type;
	Il2CppClass *parent;
	int32_t offset;	// If offset is -1, then it's thread static
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct PropertyInfo
{
	Il2CppClass *parent;
	const char *name;
	const MethodInfo *get;
	const MethodInfo *set;
	uint32_t attrs;
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct EventInfo
{
	const char* name;
	const Il2CppType* eventType;
	Il2CppClass* parent;
	const MethodInfo* add;
	const MethodInfo* remove;
	const MethodInfo* raise;
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct ParameterInfo
{
	const char* name;
	int32_t position;
	uint32_t token;
	CustomAttributeIndex customAttributeIndex;
	const Il2CppType* parameter_type;
};

typedef void* (*InvokerMethod)(const MethodInfo*, void*, void**);

#if IL2CPP_DEBUGGER_ENABLED
struct Il2CppDebugDocument
{
	const char *directory;
	const char *filename;
};

struct Il2CppDebugTypeInfo
{
	const Il2CppDebugDocument *document;
};

struct Il2CppDebugLocalsInfo
{
	Il2CppClass *type;
	const char *name;
	uint32_t start_offset;
	uint32_t end_offset;
};

struct SequencePointRecord
{
	int32_t start_offset;
	int32_t end_offset;
	void *user_data;
};

struct Il2CppDebugMethodInfo
{
	const Il2CppDebugDocument *document;
	// Table of [offset -> lineno] mapping.
	// Ends with {-1,-1}
	const int32_t *offsets_table;
	const size_t code_size;
	const Il2CppDebugLocalsInfo **locals;

	int32_t sequence_points_count;
	int32_t breakpoints_count;// incremented atomically
	SequencePointRecord *sequence_points;
};
#endif

union Il2CppRGCTXData
{
	void* rgctxDataDummy;
	const MethodInfo* method;
	const Il2CppType* type;
	Il2CppClass* klass;
};

struct MethodInfo
{
	methodPointerType method;
	InvokerMethod invoker_method;
	const char* name;
	Il2CppClass *declaring_type;
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

#if IL2CPP_DEBUGGER_ENABLED
	const Il2CppDebugMethodInfo *debug_info;
#endif
};

struct Il2CppRuntimeInterfaceOffsetPair
{
	Il2CppClass* interfaceType;
	int32_t offset;
};

struct Il2CppClass
{
	// The following fields are always valid for a Il2CppClass structure
	const Il2CppImage* image;
	void* gc_desc;
	const char* name;
	const char* namespaze;
	const Il2CppType* byval_arg;
	const Il2CppType* this_arg;
	Il2CppClass* element_class;
	Il2CppClass* castClass;
	Il2CppClass* declaringType;
	Il2CppClass* parent;
	Il2CppGenericClass *generic_class;
	const Il2CppTypeDefinition* typeDefinition; // non-NULL for Il2CppClass's constructed from type defintions
	// End always valid fields
	
	// The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
	FieldInfo* fields; // Initialized in SetupFields
	const EventInfo* events; // Initialized in SetupEvents
	const PropertyInfo* properties; // Initialized in SetupProperties
	const MethodInfo** methods; // Initialized in SetupMethods
	Il2CppClass** nestedTypes; // Initialized in SetupNestedTypes
	Il2CppClass** implementedInterfaces; // Initialized in SetupInterfaces
	const MethodInfo** vtable; // Initialized in Init
	Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // Initialized in Init
	void* static_fields; // Initialized in Init
	const Il2CppRGCTXData* rgctx_data; // Initialized in Init
	// used for fast parent checks
	Il2CppClass** typeHierarchy; // Initialized in SetupTypeHierachy
	// End initialization required fields

#if IL2CPP_DEBUGGER_ENABLED
	const Il2CppDebugTypeInfo *debug_info;
#endif

	uint32_t cctor_started;
	uint32_t cctor_finished;
	ALIGN_TYPE (8) uint64_t cctor_thread;

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
	uint8_t is_import : 1;
};

// compiler calcualted values
struct Il2CppTypeDefinitionSizes
{
	uint32_t instance_size;
	int32_t native_size;
	uint32_t static_fields_size;
	uint32_t thread_static_fields_size;
};

struct Il2CppDomain
{
	Il2CppAppDomain* domain;
	Il2CppObject* setup;	// We don't define setup class in native code because it depends on mscorlib profile and we never seen to access its internals anyway
	Il2CppAppContext* default_context;
	const char* friendly_name;
	uint32_t domain_id;
};

struct Il2CppImage
{
	const char* name;
	AssemblyIndex assemblyIndex;

	TypeDefinitionIndex typeStart;
	uint32_t typeCount;

	MethodIndex entryPointIndex;

	Il2CppNameToTypeDefinitionIndexHashTable* nameToClassHashTable;

	uint32_t token;
};

struct Il2CppMarshalingFunctions
{
	methodPointerType marshal_to_native_func;
	methodPointerType marshal_from_native_func;
	methodPointerType marshal_cleanup_func;
};

struct Il2CppCodeGenOptions
{
	bool enablePrimitiveValueTypeGenericSharing;
};

struct Il2CppCodeRegistration
{
	uint32_t methodPointersCount;
	const methodPointerType* methodPointers;
	uint32_t delegateWrappersFromNativeToManagedCount;
	const methodPointerType** delegateWrappersFromNativeToManaged; // note the double indirection to handle different calling conventions
	uint32_t delegateWrappersFromManagedToNativeCount;
	const methodPointerType* delegateWrappersFromManagedToNative;
	uint32_t marshalingFunctionsCount;
	const Il2CppMarshalingFunctions* marshalingFunctions;
	uint32_t ccwMarshalingFunctionsCount;
	const methodPointerType* ccwMarshalingFunctions;
	uint32_t genericMethodPointersCount;
	const methodPointerType* genericMethodPointers;
	uint32_t invokerPointersCount;
	const InvokerMethod* invokerPointers;
	CustomAttributeIndex customAttributeCount;
	const CustomAttributesCacheGenerator* customAttributeGenerators;
	GuidIndex guidCount;
	const Il2CppGuid** guids;
};

struct Il2CppMetadataRegistration
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
	const int32_t* fieldOffsets;

	TypeDefinitionIndex typeDefinitionsSizesCount;
	const Il2CppTypeDefinitionSizes* typeDefinitionsSizes;
	const size_t metadataUsagesCount;
	void** const* metadataUsages;
};

struct Il2CppRuntimeStats
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
};

extern Il2CppRuntimeStats il2cpp_runtime_stats;
