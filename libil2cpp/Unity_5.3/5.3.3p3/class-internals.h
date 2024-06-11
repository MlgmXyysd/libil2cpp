#pragma once

#include "il2cpp-config.h"
#include <stdint.h>
#include "metadata.h"
#include "il2cpp-metadata.h"

#define IL2CPP_CLASS_IS_ARRAY(c) ((c)->rank)

struct TypeInfo;
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
	TypeInfo *object_class;
	TypeInfo *byte_class;
	TypeInfo *void_class;
	TypeInfo *boolean_class;
	TypeInfo *sbyte_class;
	TypeInfo *int16_class;
	TypeInfo *uint16_class;
	TypeInfo *int32_class;
	TypeInfo *uint32_class;
	TypeInfo *int_class;
	TypeInfo *uint_class;
	TypeInfo *int64_class;
	TypeInfo *uint64_class;
	TypeInfo *single_class;
	TypeInfo *double_class;
	TypeInfo *char_class;
	TypeInfo *string_class;
	TypeInfo *enum_class;
	TypeInfo *array_class;
	TypeInfo *delegate_class;
	TypeInfo *multicastdelegate_class;
	TypeInfo *asyncresult_class;
	TypeInfo *manualresetevent_class;
	TypeInfo *typehandle_class;
	TypeInfo *fieldhandle_class;
	TypeInfo *methodhandle_class;
	TypeInfo *systemtype_class;
	TypeInfo *monotype_class;
	TypeInfo *exception_class;
	TypeInfo *threadabortexception_class;
	TypeInfo *thread_class;
	/*TypeInfo *transparent_proxy_class;
	TypeInfo *real_proxy_class;
	TypeInfo *mono_method_message_class;*/
	TypeInfo *appdomain_class;
	TypeInfo *appdomain_setup_class;
	TypeInfo *field_info_class;
	TypeInfo *method_info_class;
	TypeInfo *property_info_class;
	TypeInfo *event_info_class;
	TypeInfo *mono_event_info_class;
	TypeInfo *stringbuilder_class;
	/*TypeInfo *math_class;*/
	TypeInfo *stack_frame_class;
	TypeInfo *stack_trace_class;
	TypeInfo *marshal_class;
	/*TypeInfo *iserializeable_class;
	TypeInfo *serializationinfo_class;
	TypeInfo *streamingcontext_class;*/
	TypeInfo *typed_reference_class;
	/*TypeInfo *argumenthandle_class;*/
	TypeInfo *marshalbyrefobject_class;
	/*TypeInfo *monitor_class;
	TypeInfo *iremotingtypeinfo_class;
	TypeInfo *runtimesecurityframe_class;
	TypeInfo *executioncontext_class;
	TypeInfo *internals_visible_class;*/
	TypeInfo *generic_ilist_class;
	TypeInfo *generic_icollection_class;
	TypeInfo *generic_ienumerable_class;
	TypeInfo *generic_nullable_class;
	/*TypeInfo *variant_class;
	TypeInfo *com_object_class;
	TypeInfo *com_interop_proxy_class;
	TypeInfo *iunknown_class;
	TypeInfo *idispatch_class;
	TypeInfo *safehandle_class;
	TypeInfo *handleref_class;
	TypeInfo *attribute_class;*/
	TypeInfo *customattribute_data_class;
	//TypeInfo *critical_finalizer_object;
	TypeInfo *version;
	TypeInfo *culture_info;
	TypeInfo *async_call_class;
	TypeInfo *assembly_class;
	TypeInfo *assembly_name_class;
	TypeInfo *enum_info_class;
	TypeInfo *mono_field_class;
	TypeInfo *mono_method_class;
	TypeInfo *mono_method_info_class;
	TypeInfo *mono_property_info_class;
	TypeInfo *parameter_info_class;
	TypeInfo *module_class;
	TypeInfo *pointer_class;
	TypeInfo *system_exception_class;
	TypeInfo *argument_exception_class;
	TypeInfo *wait_handle_class;
	TypeInfo *safe_handle_class;
	TypeInfo *sort_key_class;
} Il2CppDefaults;

extern Il2CppDefaults il2cpp_defaults;

struct TypeInfo;
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
	TypeInfo** attributeTypes;
};

typedef void (*CustomAttributesCacheGenerator)(CustomAttributesCache*, CustomAttributeTypeCache*);

const int THREAD_STATIC_FIELD_OFFSET = -1;

struct FieldInfo
{
	const char* name;
	const Il2CppType* type;
	TypeInfo *parent;
	int32_t offset;	// If offset is -1, then it's thread static
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct PropertyInfo
{
	TypeInfo *parent;
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
	TypeInfo* parent;
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
	TypeInfo *type;
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
	TypeInfo* klass;
};

struct MethodInfo
{
	methodPointerType method;
	InvokerMethod invoker_method;
	const char* name;
	TypeInfo *declaring_type;
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
	TypeInfo* interfaceType;
	int32_t offset;
};

struct TypeInfo
{
	// The following fields are always valid for a TypeInfo structure
	const Il2CppImage* image;
	void* gc_desc;
	const char* name;
	const char* namespaze;
	const Il2CppType* byval_arg;
	const Il2CppType* this_arg;
	TypeInfo* element_class;
	TypeInfo* castClass;
	TypeInfo* declaringType;
	TypeInfo* parent;
	Il2CppGenericClass *generic_class;
	const Il2CppTypeDefinition* typeDefinition; // non-NULL for TypeInfo's constructed from type defintions
	// End always valid fields
	
	// The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
	FieldInfo* fields; // Initialized in SetupFields
	const EventInfo* events; // Initialized in SetupEvents
	const PropertyInfo* properties; // Initialized in SetupProperties
	const MethodInfo** methods; // Initialized in SetupMethods
	TypeInfo** nestedTypes; // Initialized in SetupNestedTypes
	TypeInfo** implementedInterfaces; // Initialized in SetupInterfaces
	const MethodInfo** vtable; // Initialized in Init
	Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets; // Initialized in Init
	void* static_fields; // Initialized in Init
	const Il2CppRGCTXData* rgctx_data; // Initialized in Init
	// used for fast parent checks
	TypeInfo** typeHierarchy; // Initialized in SetupTypeHierachy
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
	uint32_t genericMethodPointersCount;
	const methodPointerType* genericMethodPointers;
	uint32_t invokerPointersCount;
	const InvokerMethod* invokerPointers;
	CustomAttributeIndex customAttributeCount;
	const CustomAttributesCacheGenerator* customAttributeGenerators;
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
