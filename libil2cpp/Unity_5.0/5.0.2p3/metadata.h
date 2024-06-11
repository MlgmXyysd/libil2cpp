#pragma once

#include <stdint.h>
#include "blob.h"

struct TypeInfo;
struct MethodInfo;
struct Il2CppType;

struct Il2CppGenericContainer;

struct Il2CppArrayType {
	const Il2CppType* etype;
	uint8_t rank;
	uint8_t numsizes;
	uint8_t numlobounds;
	int *sizes;
	int *lobounds;
};

struct Il2CppGenericInst {
	uint32_t type_argc;
	const Il2CppType **type_argv;
};

struct Il2CppGenericContext {
	/* The instantiation corresponding to the class generic parameters */
	Il2CppGenericInst *class_inst;
	/* The instantiation corresponding to the method generic parameters */
	Il2CppGenericInst *method_inst;
};

/*
 * A type parameter.
 */
struct Il2CppGenericParam {
	Il2CppGenericContainer *owner;	/* Type or method this parameter was defined in. */
	uint16_t num;
	/*
	 * If owner is NULL, or owner is 'owned' by this gparam,
	 * then this is the image whose mempool this struct was allocated from.
	 * The second case happens for gparams created in
	 * mono_reflection_initialize_generic_parameter ().
	 */
	/*MonoImage *image;*/
};

/* Additional details about a Il2CppGenericParam */
struct Il2CppGenericParamInfo {
	TypeInfo *pklass;		/* The corresponding `TypeInfo'. */
	const char *name;
	uint16_t flags;
	uint32_t token;
	const Il2CppType** constraints; /* NULL means end of list */
};

struct Il2CppGenericParamFull {
	Il2CppGenericParam param;
	Il2CppGenericParamInfo info;
};

struct Il2CppGenericContainer {
	Il2CppGenericContext context;
	/* If we're a generic method definition in a generic type definition,
	   the generic container of the containing class. */
	Il2CppGenericContainer *parent;
	/* the generic type definition or the generic method definition corresponding to this container */
	union {
		void* dummy; // We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union.
		TypeInfo *klass;
		MethodInfo *method;
	} owner;
	uint32_t type_argc    : 31;
	/* If true, we're a generic method, otherwise a generic type definition. */
	/* Invariant: parent != NULL => is_method */
	uint32_t is_method : 1;
	/* Our type parameters. */
	Il2CppGenericParamFull **type_params;
};

#define il2cpp_generic_container_get_param(gc, i) ((Il2CppGenericParam *) *((gc)->type_params + (i)))

struct Il2CppGenericClass
{
	TypeInfo *container_class;	/* the generic type definition */
	Il2CppGenericContext context;	/* a context that contains the type instantiation doesn't contain any method instantiation */
	TypeInfo *cached_class;	/* if present, the TypeInfo corresponding to the instantiation.  */
};

struct Il2CppGenericMethod
{
	MethodInfo* methodDefinition;
	Il2CppGenericContext context;
	MethodInfo* cachedMethod;
};

struct Il2CppType {
	union {
		// We have this dummy field first because pre C99 compilers (MSVC) can only initializer the first value in a union.
		void* dummy;
		TypeInfo *klass; /* for VALUETYPE and CLASS */
		const Il2CppType *type;   /* for PTR and SZARRAY */
		Il2CppArrayType *array; /* for ARRAY */
		//MonoMethodSignature *method;
		Il2CppGenericParam *generic_param; /* for VAR and MVAR */
		Il2CppGenericClass *generic_class; /* for GENERICINST */
	} data;
	unsigned int attrs    : 16; /* param attributes or field flags */
	Il2CppTypeEnum type     : 8;
	unsigned int num_mods : 6;  /* max 64 modifiers follow at the end */
	unsigned int byref    : 1;
	unsigned int pinned   : 1;  /* valid when included in a local var signature */
	//MonoCustomMod modifiers [MONO_ZERO_LEN_ARRAY]; /* this may grow */
};

typedef enum {
	IL2CPP_CALL_DEFAULT,
	IL2CPP_CALL_C,
	IL2CPP_CALL_STDCALL,
	IL2CPP_CALL_THISCALL,
	IL2CPP_CALL_FASTCALL,
	IL2CPP_CALL_VARARG
} Il2CppCallConvention;

enum Il2CppCharSet
{
	CHARSET_ANSI,
	CHARSET_UNICODE
};

struct PInvokeArguments
{
	const char* moduleName;
	const char* entryPoint;
	Il2CppCallConvention callingConvention;
	Il2CppCharSet charSet;
	int parameterSize;
	bool isNoMangle;	// Says whether P/Invoke should append to function name 'A'/'W' according to charSet.
};
