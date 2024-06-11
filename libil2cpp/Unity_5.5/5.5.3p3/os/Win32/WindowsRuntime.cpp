#include "il2cpp-config.h"
#include "os/WindowsRuntime.h"
#include "vm/Exception.h"
#include "vm/String.h"
#include "WindowsHeaders.h"

#if IL2CPP_TARGET_WINDOWS

namespace il2cpp
{
namespace os
{
#if !LINK_TO_WINDOWSRUNTIME_LIBS
    template<typename FunctionType>
    FunctionType ResolveAPI(const wchar_t* moduleName, const char* functionName)
    {
        HMODULE module = GetModuleHandleW(moduleName);

        if (module == NULL)
            return NULL;

        return reinterpret_cast<FunctionType>(GetProcAddress(module, functionName));
    }

#endif

    il2cpp_hresult_t WindowsRuntime::GetActivationFactory(Il2CppHString className, Il2CppIActivationFactory** activationFactory)
    {
        IL2CPP_ASSERT(className != NULL);
        IL2CPP_ASSERT(activationFactory != NULL);

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return RoGetActivationFactory(reinterpret_cast<HSTRING>(className), reinterpret_cast<const IID&>(Il2CppIActivationFactory::IID), reinterpret_cast<void**>(activationFactory));
#else
        typedef HRESULT(WINAPI* RoGetActivationFactoryFunc)(void* activatableClassId, const Il2CppGuid& iid, Il2CppIActivationFactory** factory);
        static RoGetActivationFactoryFunc RoGetActivationFactory = NULL;

        if (RoGetActivationFactory == NULL)
        {
            RoGetActivationFactory = ResolveAPI<RoGetActivationFactoryFunc>(L"api-ms-win-core-winrt-l1-1-0.dll", "RoGetActivationFactory");

            if (RoGetActivationFactory == NULL)
                return IL2CPP_REGDB_E_CLASSNOTREG;
        }

        return RoGetActivationFactory(className, Il2CppIActivationFactory::IID, activationFactory);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::CreateHStringReference(const utils::StringView<Il2CppNativeChar>& str, Il2CppHStringHeader* header, Il2CppHString* hstring)
    {
        IL2CPP_ASSERT(header != NULL);
        IL2CPP_ASSERT(hstring != NULL);

        if (str.Length() == 0)
        {
            *hstring = NULL;
            return S_OK;
        }

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), reinterpret_cast<HSTRING_HEADER*>(header), reinterpret_cast<HSTRING*>(hstring));
#else
        typedef HRESULT(STDAPICALLTYPE * WindowsCreateStringReferenceFunc)(const wchar_t* sourceString, uint32_t length, Il2CppHStringHeader* hstringHeader, Il2CppHString* hstring);
        static WindowsCreateStringReferenceFunc WindowsCreateStringReference = NULL;

        if (WindowsCreateStringReference == NULL)
        {
            WindowsCreateStringReference = ResolveAPI<WindowsCreateStringReferenceFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsCreateStringReference");

            if (WindowsCreateStringReference == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsCreateStringReference(str.Str(), static_cast<uint32_t>(str.Length()), header, hstring);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::CreateHString(const utils::StringView<Il2CppChar>& str, Il2CppHString* hstring)
    {
        IL2CPP_ASSERT(str.Str() != NULL || str.Length() == 0);

        if (str.Length() == 0)
        {
            *hstring = NULL;
            return S_OK;
        }

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsCreateString(str.Str(), static_cast<uint32_t>(str.Length()), reinterpret_cast<HSTRING*>(hstring));
#else
        typedef il2cpp_hresult_t (STDAPICALLTYPE * WindowsCreateStringFunc)(const wchar_t* sourceString, uint32_t length, Il2CppHString* hstring);
        static WindowsCreateStringFunc WindowsCreateString = NULL;

        if (WindowsCreateString == NULL)
        {
            WindowsCreateString = ResolveAPI<WindowsCreateStringFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsCreateString");

            if (WindowsCreateString == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsCreateString(str.Str(), static_cast<uint32_t>(str.Length()), hstring);
#endif
    }

    il2cpp_hresult_t WindowsRuntime::DeleteHString(Il2CppHString hstring)
    {
        if (hstring == NULL)
            return IL2CPP_S_OK;

#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsDeleteString(reinterpret_cast<HSTRING>(hstring));
#else
        typedef il2cpp_hresult_t (STDAPICALLTYPE * WindowsDeleteStringFunc)(Il2CppHString hstring);
        static WindowsDeleteStringFunc WindowsDeleteString = NULL;

        if (WindowsDeleteString == NULL)
        {
            WindowsDeleteString = ResolveAPI<WindowsDeleteStringFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsDeleteString");

            if (WindowsDeleteString == NULL)
                return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
        }

        return WindowsDeleteString(hstring);
#endif
    }

    const Il2CppChar* WindowsRuntime::GetHStringBuffer(Il2CppHString hstring, uint32_t* length)
    {
#if LINK_TO_WINDOWSRUNTIME_LIBS
        return WindowsGetStringRawBuffer(reinterpret_cast<HSTRING>(hstring), length);
#else
        typedef const wchar_t* (STDAPICALLTYPE * WindowsGetStringRawBufferFunc)(Il2CppHString hstring, uint32_t* length);
        static WindowsGetStringRawBufferFunc WindowsGetStringRawBuffer = NULL;

        if (WindowsGetStringRawBuffer == NULL)
        {
            WindowsGetStringRawBuffer = ResolveAPI<WindowsGetStringRawBufferFunc>(L"api-ms-win-core-winrt-string-l1-1-0.dll", "WindowsGetStringRawBuffer");

            if (WindowsGetStringRawBuffer == NULL)
                vm::Exception::Raise(vm::Exception::GetNotSupportedException("Marshaling HSTRINGs is not supported on current platform."));
        }

        return WindowsGetStringRawBuffer(hstring, length);
#endif
    }

    Il2CppString* WindowsRuntime::HStringToManagedString(Il2CppHString hstring)
    {
        if (hstring == NULL)
            return vm::String::Empty();

        uint32_t length;
        const wchar_t* ptr = GetHStringBuffer(hstring, &length);
        return vm::String::NewUtf16(ptr, length);
    }
}
}

#endif
