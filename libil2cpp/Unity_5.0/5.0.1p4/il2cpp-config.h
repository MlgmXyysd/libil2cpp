#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* first setup platform defines*/
#if defined(_XBOX)
	#define IL2CPP_TARGET_XBOX360 1
	#define _UNICODE 1
	#define UNICODE 1
#elif defined(_XBOX_ONE)
	#define IL2CPP_TARGET_XBOXONE 1
	#define _UNICODE 1
	#define UNICODE 1
#elif defined(_MSC_VER)
	#define IL2CPP_TARGET_WINDOWS 1
	#define _UNICODE 1
	#define UNICODE 1
	#define NOMINMAX 1
	#define STRICT 1
#elif defined(__APPLE__)
	#define IL2CPP_TARGET_DARWIN 1
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define IL2CPP_TARGET_IOS 1
	#endif
#elif defined(__ANDROID__)
	#define IL2CPP_TARGET_ANDROID 1
#elif defined(EMSCRIPTEN)
	#define IL2CPP_TARGET_JAVASCRIPT 1
#elif defined(__linux__)
	#define IL2CPP_TARGET_LINUX 1
#elif defined(NN_PLATFORM_CTR)
	#define IL2CPP_TARGET_N3DS 1
#else
	#error please define your target platform
#endif

#ifndef IL2CPP_TARGET_WINDOWS
#define IL2CPP_TARGET_WINDOWS 0
#endif

#ifndef IL2CPP_TARGET_DARWIN
#define IL2CPP_TARGET_DARWIN 0
#endif

#ifndef IL2CPP_TARGET_IOS
#define IL2CPP_TARGET_IOS 0
#endif

#ifndef IL2CPP_TARGET_ANDROID
#define IL2CPP_TARGET_ANDROID 0
#endif

#ifndef IL2CPP_TARGET_JAVASCRIPT
#define IL2CPP_TARGET_JAVASCRIPT 0
#endif

#ifndef IL2CPP_TARGET_LINUX
#define IL2CPP_TARGET_LINUX 0
#endif

#ifndef IL2CPP_TARGET_XBOX360
#define IL2CPP_TARGET_XBOX360 0
#endif

#ifndef IL2CPP_TARGET_XBOXONE
#define IL2CPP_TARGET_XBOXONE 0
#endif

#define IL2CPP_TARGET_POSIX (IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID)

#ifndef IL2CPP_EXCEPTION_DISABLED
#define IL2CPP_EXCEPTION_DISABLED 0
#endif

#ifdef _MSC_VER
# include <malloc.h>
# define IL2CPP_EXPORT __declspec(dllexport)
#else
# define IL2CPP_EXPORT __attribute__ ((visibility ("default")))
#endif

#if defined(__ARMCC_VERSION)
	#include <assert.h>
	#include <wchar.h>
	#include <ctype.h>
	#define INTPTR_MAX 2147483647
#endif

#if IL2CPP_TARGET_DARWIN
	#define IL2CPP_METHOD_ATTR
	// the following gives more accurate managed stack traces, but may cause linker errors on ARMv7 builds
	// #define IL2CPP_METHOD_ATTR __attribute__((section ("__TEXT,__managed,regular,pure_instructions")))
#else
	#define IL2CPP_METHOD_ATTR
#endif

#if defined(_MSC_VER)
	#if defined(_M_X64)
		#define IL2CPP_SIZEOF_VOID_P 8
	#elif defined(_M_IX86) || defined(_XBOX)
		#define IL2CPP_SIZEOF_VOID_P 4
	#else
		#error invalid windows architecture
	#endif
#elif defined(__GNUC__)
	#if defined(__x86_64__)
		#define IL2CPP_SIZEOF_VOID_P 8
	#elif defined(__i386__)
		#define IL2CPP_SIZEOF_VOID_P 4
	#elif defined(EMSCRIPTEN)
		#define IL2CPP_SIZEOF_VOID_P 4
	#elif defined(__arm__)
		#define IL2CPP_SIZEOF_VOID_P 4
	#elif defined(__arm64__)
		#define IL2CPP_SIZEOF_VOID_P 8
	#else
		#error invalid windows architecture
	#endif
#else
	#error please define your target architecture size
#endif

#if defined(_MSC_VER)
#define IL2CPP_CXX_ABI_MSVC 1
#else
#define IL2CPP_CXX_ABI_MSVC 0
#endif

#if IL2CPP_TARGET_WINDOWS
#define STDCALL __stdcall
#define CDECL __cdecl
#else
#define STDCALL
#define CDECL
#endif

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_XBOXONE || defined(__ARMCC_VERSION)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

#define IL2CPP_ENABLE_MONO_BUG_EMULATION 1

#if defined(__GNUC__) || defined(__SNC__) || defined(__clang__)
	#define ALIGN_OF(T) __alignof__(T)
	#define ALIGN_TYPE(val) __attribute__((aligned(val)))
	#define FORCE_INLINE inline __attribute__ ((always_inline))
#elif defined(_MSC_VER)
	#define ALIGN_OF(T) __alignof(T)
	#define ALIGN_TYPE(val) __declspec(align(val))
	#define FORCE_INLINE __forceinline
#else
	#define ALIGN_TYPE(size)
	#define FORCE_INLINE inline
#endif

/* Trigger assert if 'ptr' is not aligned to 'alignment'. */
#define ASSERT_ALIGNMENT(ptr, alignment) \
	assert ((((ptrdiff_t) ptr) & (alignment - 1)) == 0 && "Unaligned pointer!")

/* Debugging */
#ifndef IL2CPP_DEBUG
#define IL2CPP_DEBUG 0
#endif

/* Threading */
#define IL2CPP_SUPPORT_THREADS !IL2CPP_TARGET_JAVASCRIPT

#define IL2CPP_USE_STD_THREAD 0

#define IL2CPP_THREADS_STD IL2CPP_USE_STD_THREAD
#define IL2CPP_THREADS_PTHREAD (!IL2CPP_THREADS_STD && IL2CPP_TARGET_POSIX)
#define IL2CPP_THREADS_WIN32 (!IL2CPP_THREADS_STD && IL2CPP_TARGET_WINDOWS)
#define IL2CPP_THREADS_XBOXONE (!IL2CPP_THREADS_STD && IL2CPP_TARGET_XBOXONE)
#define IL2CPP_THREADS_N3DS (!IL2CPP_THREADS_STD && IL2CPP_TARGET_N3DS)

#if (IL2CPP_SUPPORT_THREADS && (!IL2CPP_THREADS_STD && !IL2CPP_THREADS_PTHREAD && !IL2CPP_THREADS_WIN32 && !IL2CPP_THREADS_XBOXONE && !IL2CPP_THREADS_N3DS))
#error "No thread implementation defined"
#endif

/* Stacktraces supported by calling into native code instead of using stacktrace sentries*/
#define IL2CPP_SUPPORT_NATIVE_STACKTRACES (IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_POSIX) && !IL2CPP_TARGET_JAVASCRIPT && !IL2CPP_TARGET_ANDROID

/* Profiler */
#define IL2CPP_ENABLE_PROFILER 1

/* GC defines*/
#define IL2CPP_GC_BOEHM !IL2CPP_TARGET_LINUX
#define IL2CPP_GC_NULL !IL2CPP_GC_BOEHM

/* we always need to NULL pointer free memory with our current allocators */
#define NEED_TO_ZERO_PTRFREE 1
#define IL2CPP_HAS_GC_DESCRIPTORS 1

/* compiler specific macros*/
typedef void (*methodPointerType)();

#if defined(_MSC_VER)
	#define IL2CPP_ZERO_LEN_ARRAY 0
#else
	#define IL2CPP_ZERO_LEN_ARRAY 0
#endif

/* clang specific __has_feature check */
#ifndef __has_feature
  #define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

#define IL2CPP_HAS_CXX_CONSTEXPR (__has_feature (cxx_constexpr))
#define IL2CPP_HAS_UNORDERED_CONTAINER (!defined(__ARMCC_VERSION))	// could be made platform specific rather than toolchain specific

/* clang specific __has_builtin check */
#ifndef __has_builtin
	#define __has_builtin(x) 0 // Compatibility with non-clang compilers.
#endif

/* need to figure out where this goes */
typedef int32_t il2cpp_array_size_t;

/* Debug macros */
#define STRINGIZE( L )			#L
#define MAKE_STRING( M, L )		M(L)
#define $Line					MAKE_STRING( STRINGIZE, __LINE__ )
#define FIXME					__FILE__ "(" $Line ") : FIXME: "
#define ICALLMESSAGE(name)		__FILE__ "(" $Line ") : FIXME: Missing internal call implementation: " name
#define RUNTIMEMESSAGE(name)	__FILE__ "(" $Line ") : FIXME: Missing runtime implementation: " name
#define NOTSUPPORTEDICALLMESSAGE(target, name, reason)	__FILE__ "(" $Line ") : Unsupported internal call for " target ":" name " - " reason

// Keeping this for future usage if needed.
//#if defined(_MSC_VER)
//	#define PRAGMA_MESSAGE(value) __pragma(message(value))
//#else
//	#define PRAGMA_MESSAGE(value) _Pragma(STRINGIZE(value))
//#endif
#define PRAGMA_MESSAGE(value)

#if !defined(EMSCRIPTEN)

#define NOT_IMPLEMENTED_ICALL(func) \
	PRAGMA_MESSAGE(ICALLMESSAGE(#func)) \
	assert(0 && #func)
#define NOT_IMPLEMENTED_ICALL_NO_ASSERT(func,reason) \
	PRAGMA_MESSAGE(ICALLMESSAGE(#func))

#define NOT_IMPLEMENTED(func) \
	PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func)) \
	assert(0 && #func)
#define NOT_IMPLEMENTED_NO_ASSERT(func,reason) \
	PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func))

#else

// emscripten's assert will throw an exception in js.
// For now, we don't want that, so just printf and move on.
	#define NOT_IMPLEMENTED_ICALL(func) \
	PRAGMA_MESSAGE(message(ICALLMESSAGE(#func))) \
	printf("Not implemented icall: %s\n", #func);
#define NOT_IMPLEMENTED_ICALL_NO_ASSERT(func,reason) \
	PRAGMA_MESSAGE(message(ICALLMESSAGE(#func)))

#define NOT_IMPLEMENTED(func) \
	PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func))) \
	printf("Not implemented: %s\n", #func);
#define NOT_IMPLEMENTED_NO_ASSERT(func,reason) \
	PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func)))

#endif

#define NOT_SUPPORTED_IL2CPP(func, reason) \
	il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, #reason) ))

#define NOT_SUPPORTED_SRE(func) \
	il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, "System.Reflection.Emit is not supported.") ))

#define NOT_SUPPORTED_REMOTING(func) \
	il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("IL2CPP", #func, "System.Runtime.Remoting is not supported.") ))

#if IL2CPP_TARGET_JAVASCRIPT
#define NOT_SUPPORTED_WEBGL(func, reason) \
	il2cpp::vm::Exception::Raise (il2cpp::vm::Exception::GetNotSupportedException ( NOTSUPPORTEDICALLMESSAGE ("WebGL", #func, #reason) ))
#else
#define NOT_SUPPORTED_WEBGL(func, reason)
#endif

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE
	#define IL2CPP_DIR_SEPARATOR '\\'	/* backslash */
#else
	#define IL2CPP_DIR_SEPARATOR '/'	/* forward slash */
#endif

#ifndef IL2CPP_DEBUGGER_ENABLED
	#define	IL2CPP_DEBUGGER_ENABLED	0
#endif

#ifndef IL2CPP_DEBUGGER_LOG
	#define	IL2CPP_DEBUGGER_LOG	0
#endif
	
#ifndef IL2CPP_DISABLE_FULL_MESSAGES
	#define IL2CPP_DISABLE_FULL_MESSAGES	1
#endif

#if IL2CPP_TARGET_WINDOWS
	#define IL2CPP_USE_GENERIC_SOCKET_IMPL	0
#else
	#define IL2CPP_USE_GENERIC_SOCKET_IMPL	(!IL2CPP_TARGET_POSIX || IL2CPP_TARGET_JAVASCRIPT)
#endif

#define IL2CPP_USE_GENERIC_ENVIRONMENT	(!IL2CPP_TARGET_WINDOWS && !IL2CPP_TARGET_POSIX)

#define IL2CPP_SIZEOF_STRUCT_WITH_NO_INSTANCE_FIELDS 1
#define IL2CPP_VALIDATE_FIELD_LAYOUT 0

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE	// Use stub "return false" implementation where it's not implemented
#define IL2CPP_ISDEBUGGERPRESENT_IMPLEMENTED 1 
#else
#define IL2CPP_ISDEBUGGERPRESENT_IMPLEMENTED 0
#endif