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

enum Il2CppTypeNameFormat {
	IL2CPP_TYPE_NAME_FORMAT_IL,
	IL2CPP_TYPE_NAME_FORMAT_REFLECTION,
	IL2CPP_TYPE_NAME_FORMAT_FULL_NAME,
	IL2CPP_TYPE_NAME_FORMAT_ASSEMBLY_QUALIFIED
};

extern bool g_il2cpp_is_fully_initialized;

typedef struct {
	const Il2CppImage *corlib;
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
	TypeInfo *attribute_class;
	TypeInfo *customattribute_data_class;
	TypeInfo *critical_finalizer_object;*/
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


struct CustomAttributesCache
{
	int count;
	Il2CppObject** attributes;
};

typedef void (*CustomAttributesCacheGenerator)(CustomAttributesCache*);

const int THREAD_STATIC_FIELD_OFFSET = -1;

struct FieldInfo
{
	const char* name;
	const Il2CppType* type;
	TypeInfo *parent;
	int32_t offset;	// If offset is -1, then it's thread static
	CustomAttributeIndex customAttributeIndex;
};

struct PropertyInfo {
	TypeInfo *parent;
	const char *name;
	const MethodInfo *get;
	const MethodInfo *set;
	uint32_t attrs;
	CustomAttributeIndex customAttributeIndex;
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

struct Il2CppGenericMethodFunctions
{
	const Il2CppGenericMethod* genericMethod;
	uint32_t methodPointerIndex;
	uint32_t invokerMethodPointerIndex;
};

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

union Il2CppRGCTXDefinitionData
{
	const void* rgctxDataDummy;
	const Il2CppGenericMethod* method;
	const Il2CppType* type;
};

enum Il2CppRGCTXDataType
{
	IL2CPP_RGCTX_DATA_INVALID,
	IL2CPP_RGCTX_DATA_TYPE,
	IL2CPP_RGCTX_DATA_CLASS,
	IL2CPP_RGCTX_DATA_METHOD
};

struct Il2CppRGCTXDefinition
{
	Il2CppRGCTXDataType type;
	Il2CppRGCTXDefinitionData data;
};

union Il2CppRGCTX
{
	void* dummy; // We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union.
	Il2CppRGCTXData* data;
	Il2CppRGCTXDefinition* definition;
};

struct MethodInfo
{
	const char* name;
	methodPointerType method;
	TypeInfo *declaring_type;
	const Il2CppType *return_type;
	InvokerMethod invoker_method;
	const ParameterInfo* parameters;
	CustomAttributeIndex customAttributeIndex;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint8_t parameters_count;
	bool is_generic; /* true if method is a generic method definition */
	bool is_inflated; /* true if declaring_type is a generic instance or if method is a generic instance*/
	/* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
	uint32_t token;
	Il2CppRGCTX rgctx_data;
	methodPointerType native_delegate_wrapper;

	union
	{
		const void* genericDummy; /* We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union. */
		const Il2CppGenericMethod* genericMethod; /* is_inflated is true */
		const Il2CppGenericContainer* genericContainer; /* is_inflated is false and is_generic is true */
	};

#if IL2CPP_DEBUGGER_ENABLED
	const Il2CppDebugMethodInfo *debug_info;
#endif
};

struct Il2CppInterfaceOffsetPair
{
	const Il2CppType* interfaceType;
	int32_t offset;
};

struct Il2CppRuntimeInterfaceOffsetPair
{
	TypeInfo* interfaceType;
	int32_t offset;
};

union Il2CppMethodReference
{
	const void* dummy;
	const MethodInfo* method;
	const Il2CppGenericMethod* genericMethod;
};

struct Il2CppTypeDefinitionMetadata
{
	const Il2CppType* declaringType;
	const Il2CppType** nestedTypes;
	const Il2CppType** implementedInterfaces;
	const Il2CppInterfaceOffsetPair* interfaceOffsets;
	const Il2CppType* parent;
	const Il2CppMethodReference* vtableMethods;
	const bool* vtableEntryIsGenericMethod;
	const Il2CppRGCTXDefinition* rgctxDefinition;
	FieldIndex fieldStart;
};

struct Il2CppRuntimeMetadata
{
	TypeInfo* declaringType;
	TypeInfo** nestedTypes;
	TypeInfo** implementedInterfaces;
	Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets;
	TypeInfo* parent;
	TypeInfo* castClass;
	FieldInfo* fields;
};

struct TypeInfo
{
	Il2CppImage* image;
	void* gc_desc;
	const char* name;
	const char* namespaze;
	const MethodInfo** methods;
	const PropertyInfo** properties;
	const EventInfo** events;
	TypeInfo* element_class;
	const MethodInfo** vtable;
	CustomAttributeIndex customAttributeIndex;
	const Il2CppType* byval_arg;
	const Il2CppType* this_arg;

	const Il2CppTypeDefinitionMetadata* definitionMetadata;
	Il2CppRuntimeMetadata* runtimeMetadata;

	Il2CppGenericClass *generic_class;
	const Il2CppGenericContainer *generic_container;

	void* static_fields;

	Il2CppRGCTX rgctx_data;

	methodPointerType pinvoke_delegate_wrapper;
	methodPointerType marshal_to_native_func;
	methodPointerType marshal_from_native_func;
	methodPointerType marshal_cleanup_func;

	uint32_t instance_size;
	uint32_t actualSize;
	uint32_t element_size;
	int32_t native_size;
	uint32_t static_fields_size;
	uint32_t thread_static_fields_size;
	int32_t thread_static_fields_offset;
	uint32_t flags;
	uint8_t rank;
	uint8_t minimumAlignment;
	bool valuetype;
	bool initialized;
	bool enumtype;
	bool is_generic;
	bool has_references;
	bool init_pending;
	bool size_inited;
	bool has_finalize;
	bool has_cctor;
	bool is_blittable;

	uint16_t method_count;
	uint16_t property_count;
	uint16_t field_count;
	uint16_t event_count;
	uint16_t nested_type_count;
	uint16_t vtable_count;
	uint16_t interfaces_count;
	uint16_t interface_offsets_count;

#if IL2CPP_DEBUGGER_ENABLED
	const Il2CppDebugTypeInfo *debug_info;
#endif

	uint32_t cctor_started;
	uint32_t cctor_finished;
	ALIGN_TYPE (8) uint64_t cctor_thread;
};


struct Il2CppAssemblyName
{
	const char *name;
	const char *culture;
	const char *hash_value;
	const char* public_key;
	uint8_t public_key_token [IL2CPP_PUBLIC_KEY_TOKEN_LENGTH];
	uint32_t hash_alg;
	uint32_t hash_len;
	uint32_t flags;
	uint16_t major;
	uint16_t minor;
	uint16_t build;
	uint16_t revision;
};

struct Il2CppImage
{
	const char* name;
	Il2CppAssembly* assembly;

	TypeInfo** types;
	size_t typeCount;

	const MethodInfo* entryPoint;

	const char** strings;
	StringIndex stringsCount;

	const Il2CppFieldDefinition* fields;
	FieldIndex fieldsCount;

	const Il2CppFieldDefaultValue* fieldDefaultValues;
	DefaultValueIndex fieldDefaultValuesCount;

	const uint8_t* fieldDefaultValueData;
	DefaultValueDataIndex fieldDefaultValueDataCount;

	// number of custom attributes referenced by the image
	CustomAttributeIndex customAttributeCount;
	// per image cache of custom attributes - populated at runtime from customAttributeGenerators
	CustomAttributesCache** customAttributeCaches;
	// per image table of custom attribute generator functions
	const CustomAttributesCacheGenerator* customAttributeGenerators;
};

struct Il2CppAssembly
{
	Il2CppAssemblyName aname;
	Il2CppImage *image;
	CustomAttributeIndex customAttributeIndex;
};

struct Il2CppDomain
{
	Il2CppAppDomain* domain;
	Il2CppObject* setup;	// We don't define setup class in native code because it depends on mscorlib profile and we never seen to access its internals anyway
	const char* friendly_name;
	uint32_t domain_id;
};

struct Il2CppCodeRegistration
{
	uint32_t methodPointersCount;
	const methodPointerType* methodPointers;
	uint32_t invokerPointersCount;
	const InvokerMethod* invokerPointers;
};

struct Il2CppMetadataRegistration
{
	uint32_t assembliesCount;
	Il2CppAssembly** assemblies;
	uint32_t genericClassesCount;
	Il2CppGenericClass** genericClasses;
	uint32_t genericMethodsCount;
	const Il2CppGenericMethod** genericMethods;
	uint32_t genericInstsCount;
	const Il2CppGenericInst** genericInsts;
	uint32_t genericMethodTableCount;
	Il2CppGenericMethodFunctions* genericMethodTable;
	uint32_t typesCount;
	const Il2CppType* const * types;
	uint32_t methodReferencesCount;
	const Il2CppMethodReference* methodReferences;
};
