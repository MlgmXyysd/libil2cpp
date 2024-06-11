#pragma once

#if defined(SN_TARGET_PSP2)
#define IL2CPP_TARGET_PSP2 1
#define _UNICODE 1
#define UNICODE 1
#include "il2cpp-config-psp2.h"
#elif defined(SN_TARGET_ORBIS)
#define IL2CPP_TARGET_PS4 1
#define _UNICODE 1
#define UNICODE 1
#elif defined(_MSC_VER)
#define IL2CPP_TARGET_WINDOWS 1
#if defined(_XBOX_ONE)
#define IL2CPP_TARGET_XBOXONE 1
#elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define IL2CPP_TARGET_WINRT 1
#else
#define IL2CPP_TARGET_WINDOWS_DESKTOP 1
#endif
#define _UNICODE 1
#define UNICODE 1
#define STRICT 1
#elif defined(__APPLE__)
#define IL2CPP_TARGET_DARWIN 1
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_TVOS_SIMULATOR
#define IL2CPP_TARGET_IOS 1
#endif
#elif defined(__ANDROID__)
#define IL2CPP_TARGET_ANDROID 1
#elif defined(EMSCRIPTEN)
#define IL2CPP_TARGET_JAVASCRIPT 1
#elif defined(TIZEN)
#define IL2CPP_TARGET_TIZEN 1
#elif defined(__linux__)
#define IL2CPP_TARGET_LINUX 1
#elif defined(NN_PLATFORM_CTR)
#define IL2CPP_TARGET_N3DS 1
#elif defined(NN_BUILD_TARGET_PLATFORM_NX)
#define IL2CPP_TARGET_SWITCH 1
#else
#error please define your target platform
#endif

#ifndef IL2CPP_TARGET_WINDOWS
#define IL2CPP_TARGET_WINDOWS 0
#endif

#ifndef IL2CPP_TARGET_WINDOWS_DESKTOP
#define IL2CPP_TARGET_WINDOWS_DESKTOP 0
#endif

#ifndef IL2CPP_TARGET_WINRT
#define IL2CPP_TARGET_WINRT 0
#endif

#ifndef IL2CPP_TARGET_XBOXONE
#define IL2CPP_TARGET_XBOXONE 0
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

#ifndef IL2CPP_TARGET_TIZEN
#define IL2CPP_TARGET_TIZEN 0
#endif

#ifndef IL2CPP_TARGET_LINUX
#define IL2CPP_TARGET_LINUX 0
#endif

#ifndef IL2CPP_TARGET_N3DS
#define IL2CPP_TARGET_N3DS 0
#endif

#ifndef IL2CPP_TARGET_PS4
#define IL2CPP_TARGET_PS4 0
#endif

#ifndef IL2CPP_TARGET_PSP2
#define IL2CPP_TARGET_PSP2 0
#endif

#ifndef IL2CPP_TARGET_SWITCH
#define IL2CPP_TARGET_SWITCH 0
#endif

#define IL2CPP_TARGET_POSIX (IL2CPP_TARGET_DARWIN || IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_PS4 || IL2CPP_TARGET_PSP2 || IL2CPP_TARGET_TIZEN)

#define IL2CPP_SUPPORT_THREADS !IL2CPP_TARGET_JAVASCRIPT

#if IL2CPP_TARGET_WINDOWS || IL2CPP_TARGET_XBOXONE || IL2CPP_TARGET_WINRT
#include <crtdbg.h>
#define IL2CPP_ASSERT(expr) \
    _ASSERTE(expr)
#else
#define IL2CPP_ASSERT(expr) \
    assert(expr)
#endif

/* Trigger assert if 'ptr' is not aligned to 'alignment'. */
#define ASSERT_ALIGNMENT(ptr, alignment) \
    IL2CPP_ASSERT((((ptrdiff_t) ptr) & (alignment - 1)) == 0 && "Unaligned pointer!")

    #if defined(_MSC_VER)
    #if defined(_M_X64)
        #define IL2CPP_SIZEOF_VOID_P 8
    #elif defined(_M_IX86) || defined(_M_ARM)
        #define IL2CPP_SIZEOF_VOID_P 4
    #else
        #error invalid windows architecture
    #endif
#elif defined(__GNUC__) || defined(__SNC__)
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
