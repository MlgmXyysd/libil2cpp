#pragma once

#include "utils/StringView.h"

typedef enum Il2CppCallConvention
{
    IL2CPP_CALL_DEFAULT,
    IL2CPP_CALL_C,
    IL2CPP_CALL_STDCALL,
    IL2CPP_CALL_THISCALL,
    IL2CPP_CALL_FASTCALL,
    IL2CPP_CALL_VARARG
} Il2CppCallConvention;

typedef enum Il2CppCharSet
{
    CHARSET_ANSI,
    CHARSET_UNICODE,
    CHARSET_NOT_SPECIFIED
} Il2CppCharSet;

struct PInvokeArguments
{
    const il2cpp::utils::StringView<Il2CppNativeChar> moduleName;
    const il2cpp::utils::StringView<char> entryPoint;
    Il2CppCallConvention callingConvention;
    Il2CppCharSet charSet;
    int parameterSize;
    bool isNoMangle;    // Says whether P/Invoke should append to function name 'A'/'W' according to charSet.
};
