#pragma once

#include "il2cpp-config.h"
#include <stdint.h>
#include <stddef.h>

struct TypeInfo;
struct MethodInfo;
struct PropertyInfo;
struct FieldInfo;
struct EventInfo;
struct Il2CppType;
struct Il2CppAssembly;
struct Il2CppException;
struct Il2CppImage;
struct Il2CppDomain;
struct Il2CppString;
struct Il2CppReflectionMethod;
struct Il2CppAsyncCall;
struct MonitorData;

namespace il2cpp
{
	namespace os
	{
		class Mutex;
		class Thread;
	}
}

struct Il2CppReflectionAssembly;

struct Il2CppObject{
	TypeInfo *klass;
	MonitorData *monitor;
};

typedef int32_t il2cpp_array_lower_bound_t;
#define IL2CPP_ARRAY_MAX_INDEX ((int32_t) 0x7fffffff)
#define IL2CPP_ARRAY_MAX_SIZE  ((uint32_t) 0xffffffff)

struct Il2CppArrayBounds
{
	il2cpp_array_size_t length;
	il2cpp_array_lower_bound_t lower_bound;
};

struct Il2CppArray
{
	Il2CppObject obj;
	/* bounds is NULL for szarrays */
	Il2CppArrayBounds *bounds;
	/* total number of elements of the array */
	il2cpp_array_size_t max_length;
	/* we use double to ensure proper alignment on platforms that need it */
	double vector [IL2CPP_ZERO_LEN_ARRAY];
};

// This is a part of a string literal - it is needed for debugging visualization
// Note: store just Il2CppObject here, so this struct does not change memory layout!
struct Il2CppDataSegmentString
{
	Il2CppObject object;
};

// System.String
struct Il2CppString
{
	Il2CppObject object;
	int32_t length;								///< Length of string *excluding* the trailing null (which is included in 'chars').
	uint16_t chars [IL2CPP_ZERO_LEN_ARRAY];
};

#define IL2CPP_OBJECT_SETREF(obj,fieldname,value) do {	\
		/* mono_gc_wbarrier_set_field ((MonoObject*)(obj), &((obj)->fieldname), (MonoObject*)value); */	\
		(obj)->fieldname = (value);	\
	} while (0)

/* This should be used if 's' can reside on the heap */
#define IL2CPP_STRUCT_SETREF(s,field,value) do { \
		/* mono_gc_wbarrier_generic_store (&((s)->field), (MonoObject*)(value)); */ \
		(s)->field = (value); \
	} while (0)

struct Il2CppReflectionType {
	Il2CppObject object;
	const Il2CppType *type;
};

// IMPORTANT: All managed types corresponding to the objects below must be blacklisted in mscorlib.xml

// System.MonoType
struct Il2CppReflectionMonoType{
	Il2CppReflectionType type;
	Il2CppObject *type_info;
};

// System.EventInfo
struct Il2CppReflectionEvent {
	Il2CppObject object;
	Il2CppObject *cached_add_event;
};

// System.Reflection.MonoEvent
struct Il2CppReflectionMonoEvent
{
	Il2CppReflectionEvent event;
	Il2CppReflectionType* reflectedType;
	EventInfo* eventInfo;
};

// System.Reflection.MonoEventInfo
struct Il2CppReflectionMonoEventInfo
{
	Il2CppReflectionType* declaringType;
	Il2CppReflectionType* reflectedType;
	Il2CppString* name;
	Il2CppReflectionMethod* addMethod;
	Il2CppReflectionMethod* removeMethod;
	Il2CppReflectionMethod* raiseMethod;
	uint32_t eventAttributes;
	Il2CppArray* otherMethods;
};

// System.MonoEnumInfo
struct Il2CppEnumInfo {
	Il2CppReflectionType *utype;
	Il2CppArray *values;
	Il2CppArray *names;
	void* name_hash;
};

// System.Reflection.MonoField
struct Il2CppReflectionField {
	Il2CppObject object;
	TypeInfo *klass;
	FieldInfo *field;
	Il2CppString *name;
	Il2CppReflectionType *type;
	uint32_t attrs;
};

// System.Reflection.MonoProperty
struct Il2CppReflectionProperty {
	Il2CppObject object;
	TypeInfo *klass;
	PropertyInfo *property;
};

// System.Reflection.MonoMethod
struct Il2CppReflectionMethod {
	Il2CppObject object;
	MethodInfo *method;
	Il2CppString *name;
	Il2CppReflectionType *reftype;
};

// System.Reflection.MonoGenericMethod
struct Il2CppReflectionGenericMethod
{
	Il2CppReflectionMethod base;
};

// System.Reflection.MonoMethodInfo
struct Il2CppMethodInfo {
	Il2CppReflectionType *parent;
	Il2CppReflectionType *ret;
	uint32_t attrs;
	uint32_t implattrs;
	uint32_t callconv;
};

// System.Reflection.MonoPropertyInfo
struct Il2CppPropertyInfo {
	Il2CppReflectionType *parent;
	Il2CppString *name;
	Il2CppReflectionMethod *get;
	Il2CppReflectionMethod *set;
	uint32_t attrs;
};

// System.Reflection.ParameterInfo
struct Il2CppReflectionParameter{
	Il2CppObject object;
	Il2CppReflectionType *ClassImpl;
	Il2CppObject *DefaultValueImpl;
	Il2CppObject *MemberImpl;
	Il2CppString *NameImpl;
	int32_t PositionImpl;
	uint32_t AttrsImpl;
	Il2CppObject *MarshalAsImpl;
};

// System.Reflection.Module
struct Il2CppReflectionModule {
	Il2CppObject obj;
	Il2CppImage* image;
	Il2CppReflectionAssembly* assembly;
	Il2CppString* fqname;
	Il2CppString* name;
	Il2CppString* scopename;
	bool is_resource;
	uint32_t token;
};

// System.Reflection.AssemblyName
struct Il2CppReflectionAssemblyName
{
	Il2CppObject  obj;
	Il2CppString *name;
	Il2CppString *codebase;
	int32_t major, minor, build, revision;
	Il2CppObject  *cultureInfo;
	uint32_t     flags;
	uint32_t     hashalg;
	Il2CppObject  *keypair;
	Il2CppArray   *publicKey;
	Il2CppArray   *keyToken;
	uint32_t     versioncompat;
	Il2CppObject *version;
	uint32_t     processor_architecture;
};

// System.Reflection.Assembly
struct Il2CppReflectionAssembly {
	Il2CppObject object;
	Il2CppAssembly *assembly;
	Il2CppObject *resolve_event_holder;
	/* CAS related */
	Il2CppObject *evidence;	/* Evidence */
	Il2CppObject *minimum;	/* PermissionSet - for SecurityAction.RequestMinimum */
	Il2CppObject *optional;	/* PermissionSet - for SecurityAction.RequestOptional */
	Il2CppObject *refuse;	/* PermissionSet - for SecurityAction.RequestRefuse */
	Il2CppObject *granted;	/* PermissionSet - for the resolved assembly granted permissions */
	Il2CppObject *denied;	/* PermissionSet - for the resolved assembly denied permissions */
	/* */
	bool from_byte_array;
	Il2CppString *name;
};

// System.Reflection.Emit.UnmanagedMarshal
struct Il2CppReflectionMarshal {
	Il2CppObject object;
	int32_t count;
	int32_t type;
	int32_t eltype;
	Il2CppString* guid;
	Il2CppString* mcookie;
	Il2CppString* marshaltype;
	Il2CppObject* marshaltyperef;
	int32_t param_num;
	bool has_size;
};

// System.Reflection.Pointer
struct Il2CppReflectionPointer
{
	Il2CppObject object;
	void* data;
	Il2CppReflectionType* type;
};

// System.IntPtr
// Just in case there is a platform where we cannot replace struct {void* val;} with void*
struct Il2CppIntPtr
{
	void* m_value;
	static Il2CppIntPtr Zero;
};

// System.Threading.Thread
struct Il2CppThread {
	Il2CppObject  obj;
	int         lock_thread_id; /* to be used as the pre-shifted thread id in thin locks */
	il2cpp::os::Thread* handle;
	Il2CppArray  *cached_culture_info;
	void*    unused1;
	bool threadpool_thread;//bool threadpool_thread;
	uint16_t* name;
	uint32_t name_len;
	uint32_t	    state;
	Il2CppObject* abort_exc;//MonoException *abort_exc;
	int abort_state_handle;
	uint64_t tid;	/* This is accessed as a gsize in the code (so it can hold a 64bit pointer on systems that need it), but needs to reserve 64 bits of space on all machines as it corresponds to a field in managed code */
	void*	    start_notify;
	void* stack_ptr;
	void* *static_data;
	void* jit_data;
	void* lock_data;
	Il2CppObject* current_appcontext;//MonoAppContext *current_appcontext;
	int stack_size;
	Il2CppObject* start_obj;//MonoObject *start_obj;
	void* appdomain_refs;//GSList *appdomain_refs;
	///* This is modified using atomic ops, so keep it a int32_t */
	int32_t interruption_requested;
	void* suspend_event;
	void* suspended_event;
	void* resume_event;
	il2cpp::os::Mutex* synch_cs;
	uint8_t* serialized_culture_info;
	uint32_t serialized_culture_info_len;
	uint8_t* serialized_ui_culture_info;
	uint32_t serialized_ui_culture_info_len;
	bool thread_dump_requested;//bool thread_dump_requested;
	void* end_stack; /* This is only used when running in the debugger. */
	bool thread_interrupt_requested;//bool thread_interrupt_requested;
	uint8_t	apartment_state;
	int32_t critical_region_level;
	uint32_t small_id; /* A small, unique id, used for the hazard pointer table. */
	void* manage_callback;
	Il2CppException *pending_exception;
	Il2CppObject *ec_to_set;
	///*
	// * These fields are used to avoid having to increment corlib versions
	// * when a new field is added to the unmanaged MonoThread structure.
	// */
	void* interrupt_on_stop;
	void* unused3;
	void* unused4;
	void* unused5;
	void* stackFrames;
};

// System.Exception
struct Il2CppException {
	Il2CppObject object;
	/* Stores the IPs and the generic sharing infos
	   (vtable/MRGCTX) of the frames. */
	Il2CppArray *trace_ips;
	Il2CppObject *inner_ex;
	Il2CppString *message;
	Il2CppString *help_link;
	Il2CppString *class_name;
	Il2CppString *stack_trace;
	Il2CppString *remote_stack_trace;
	int32_t    remote_stack_index;
	int32_t    hresult;
	Il2CppString *source;
	Il2CppObject *_data;
};

// System.SystemException
struct Il2CppSystemException {
	Il2CppException base;
};

// System.ArgumentException
struct Il2CppArgumentException {
	Il2CppException base;
	Il2CppString *argName;
};

// System.TypedReference
struct Il2CppTypedRef {
	Il2CppType *type;
	void*  value;
	TypeInfo *klass;
};

// System.Delegate
struct Il2CppDelegate {
	Il2CppObject object;
	/* The compiled code of the target method */
	methodPointerType method_ptr;
	/* The invoke code */
	void* (*invoke_impl)(MethodInfo*, void*, void**);
	Il2CppObject *target;
	MethodInfo *method;
	void* delegate_trampoline;
	/*
	 * If non-NULL, this points to a memory location which stores the address of
	 * the compiled code of the method, or NULL if it is not yet compiled.
	 */
	uint8_t **method_code;
	Il2CppReflectionMethod *method_info;
	Il2CppReflectionMethod *original_method_info;
	Il2CppObject *data;
};

// System.MarshalByRefObject
struct Il2CppMarshalByRefObject {
	Il2CppObject obj;
	Il2CppObject *identity;
};

// System.AppDomain
struct Il2CppAppDomain {
	Il2CppMarshalByRefObject mbr;
	Il2CppDomain *data;
};

// System.Diagnostics.StackFrame
struct Il2CppStackFrame {
	Il2CppObject obj;
	int32_t il_offset;
	int32_t native_offset;
	Il2CppReflectionMethod *method;
	Il2CppString *filename;
	int32_t line;
	int32_t column;
	Il2CppString *internal_method_name;
};

// System.Globalization.DateTimeFormatInfo
struct Il2CppDateTimeFormatInfo {
	Il2CppObject obj;
	bool readOnly;
	Il2CppString* AMDesignator;
	Il2CppString* PMDesignator;
	Il2CppString* DateSeparator;
	Il2CppString* TimeSeparator;
	Il2CppString* ShortDatePattern;
	Il2CppString* LongDatePattern;
	Il2CppString* ShortTimePattern;
	Il2CppString* LongTimePattern;
	Il2CppString* MonthDayPattern;
	Il2CppString* YearMonthPattern;
	Il2CppString* FullDateTimePattern;
	Il2CppString* RFC1123Pattern;
	Il2CppString* SortableDateTimePattern;
	Il2CppString* UniversalSortableDateTimePattern;
	uint32_t FirstDayOfWeek;
	Il2CppObject* Calendar;
	uint32_t CalendarWeekRule;
	Il2CppArray* AbbreviatedDayNames;
	Il2CppArray* DayNames;
	Il2CppArray* MonthNames;
	Il2CppArray* AbbreviatedMonthNames;
	Il2CppArray* ShortDatePatterns;
	Il2CppArray* LongDatePatterns;
	Il2CppArray* ShortTimePatterns;
	Il2CppArray* LongTimePatterns;
	Il2CppArray* MonthDayPatterns;
	Il2CppArray* YearMonthPatterns;
	Il2CppArray* shortDayNames;
};

// System.Globalization.NumberFormatInfo
struct Il2CppNumberFormatInfo {
	Il2CppObject obj;
	bool readOnly;
	Il2CppString* decimalFormats;
	Il2CppString* currencyFormats;
	Il2CppString* percentFormats;
	Il2CppString* digitPattern;
	Il2CppString* zeroPattern;
	int32_t currencyDecimalDigits;
	Il2CppString* currencyDecimalSeparator;
	Il2CppString* currencyGroupSeparator;
	Il2CppArray* currencyGroupSizes;
	int32_t currencyNegativePattern;
	int32_t currencyPositivePattern;
	Il2CppString* currencySymbol;
	Il2CppString* naNSymbol;
	Il2CppString* negativeInfinitySymbol;
	Il2CppString* negativeSign;
	uint32_t numberDecimalDigits;
	Il2CppString* numberDecimalSeparator;
	Il2CppString* numberGroupSeparator;
	Il2CppArray* numberGroupSizes;
	int32_t numberNegativePattern;
	int32_t percentDecimalDigits;
	Il2CppString* percentDecimalSeparator;
	Il2CppString* percentGroupSeparator;
	Il2CppArray* percentGroupSizes;
	int32_t percentNegativePattern;
	int32_t percentPositivePattern;
	Il2CppString* percentSymbol;
	Il2CppString* perMilleSymbol;
	Il2CppString* positiveInfinitySymbol;
	Il2CppString* positiveSign;
};

// System.Globalization.CultureInfo
struct Il2CppCultureInfo {
	Il2CppObject obj;
	bool is_read_only;
	int32_t lcid;
	int32_t parent_lcid;
	int32_t specific_lcid;
	int32_t datetime_index;
	int32_t number_index;
	bool use_user_override;
	Il2CppNumberFormatInfo* number_format;
	Il2CppDateTimeFormatInfo* datetime_format;
	Il2CppObject* textinfo;
	Il2CppString* name;
	Il2CppString* displayname;
	Il2CppString* englishname;
	Il2CppString* nativename;
	Il2CppString* iso3lang;
	Il2CppString* iso2lang;
	Il2CppString* icu_name;
	Il2CppString* win3lang;
	Il2CppString* territory;
	Il2CppString* compareinfo;
	const int32_t* calendar_data;
	const void* text_info_data;
};

// System.Threading.WaitHandle
struct Il2CppWaitHandle {
	Il2CppMarshalByRefObject object;
	void* handle;
	bool disposed;
};

// System.Runtime.InteropServices.SafeHandle
// Inherited by Microsoft.Win32.SafeHandles.SafeWaitHandle
struct Il2CppSafeHandle {
	Il2CppObject base;
	void* handle;
	void* invalid_handle_value;
	int refcount;
	bool owns_handle;
};

// System.Text.StringBuilder
struct Il2CppStringBuilder {
	Il2CppObject object;
	int32_t length;
	Il2CppString *str;
	Il2CppString *cached_str;
	int32_t max_capacity;
};

// System.Net.SocketAddress
struct Il2CppSocketAddress
{
	Il2CppObject base;
	Il2CppArray* data;
};

// System.Globalization.SortKey
struct Il2CppSortKey
{
	Il2CppObject base;
	Il2CppString *str;
	int32_t options;
	Il2CppArray *key;
	int32_t lcid;
};

struct Il2CppAsyncResult
{
	Il2CppObject base;
	Il2CppObject *async_state;
	Il2CppWaitHandle *handle;
	Il2CppDelegate *async_delegate;
	void* data; // We pass delegate arguments here. This is repurposed. Depends on Mono C# code not using the field.
	Il2CppAsyncCall *object_data;
	bool  sync_completed;
	bool  completed;
	bool  endinvoke_called;
	Il2CppObject *async_callback;
	Il2CppObject *execution_context;
	Il2CppObject *original_context;
};

struct Il2CppAsyncCall
{
	Il2CppObject base;
	void *msg; // We pass exceptions through here for now.
	MethodInfo *cb_method; // We don't set this.
	Il2CppDelegate *cb_target; // We pass the actual delegate here.
	Il2CppObject *state;
	Il2CppObject *res;
	Il2CppArray *out_args;
	/* This is a HANDLE, we use guint64 so the managed object layout remains constant */
	uint64_t wait_event;
};

struct Il2CppExceptionWrapper
{
	Il2CppException* ex;

	Il2CppExceptionWrapper (Il2CppException* ex) : ex (ex) {}
};

/// Corresponds to Mono's internal System.Net.Sockets.Socket.SocketAsyncResult
/// class. Has no relation to Il2CppAsyncResult.
struct Il2CppSocketAsyncResult
{
	Il2CppObject base;
	Il2CppObject *socket;
	Il2CppIntPtr handle;
	Il2CppObject *state;
	Il2CppDelegate *callback;
	Il2CppWaitHandle *wait_handle;
	Il2CppException *delayed_exc;
	Il2CppObject *ep;
	Il2CppArray *buffer;
	int32_t offset;
	int32_t size;
	int32_t socket_flags;
	Il2CppObject *accept_reuse_socket;
	Il2CppArray *addresses;
	int32_t port;
	Il2CppObject *buffers;
	bool reusesocket;
	Il2CppObject *acc_socket;
	int32_t total;
	bool completed_synch;
	bool completed;
	bool blocking;
	int32_t error;
	int32_t operation;
	Il2CppAsyncResult *ares;
};

#define IL2CPP_CHECK_ARG_NULL(arg)	do {	\
	if (arg == NULL)	\
	{	\
		il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetArgumentNullException (#arg));	\
	};	} while (0)
