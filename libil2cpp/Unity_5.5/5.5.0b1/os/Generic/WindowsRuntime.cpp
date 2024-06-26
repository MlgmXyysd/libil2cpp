#include "il2cpp-config.h"
#include "os/WindowsRuntime.h"
#include "vm/Exception.h"

#if IL2CPP_USE_GENERIC_WINDOWSRUNTIME

namespace il2cpp
{

namespace os
{

il2cpp_hresult_t WindowsRuntime::GetActivationFactory(Il2CppHString className, Il2CppIActivationFactory** activationFactory)
{
	NO_UNUSED_WARNING(className);
	NO_UNUSED_WARNING(activationFactory);
	assert(className != NULL);
	assert(activationFactory != NULL);
	return IL2CPP_REGDB_E_CLASSNOTREG;
}

il2cpp_hresult_t WindowsRuntime::CreateHStringReference(const utils::StringView<Il2CppNativeChar>& str, Il2CppHStringHeader* header, Il2CppHString* hstring)
{
	NO_UNUSED_WARNING(str);
	NO_UNUSED_WARNING(header);
	NO_UNUSED_WARNING(hstring);
	assert(header != NULL);
	assert(hstring != NULL);
	return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
}

il2cpp_hresult_t WindowsRuntime::CreateHString(const utils::StringView<Il2CppChar>& str, Il2CppHString* hstring)
{
	NO_UNUSED_WARNING(str);
	NO_UNUSED_WARNING(hstring);
	return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
}

il2cpp_hresult_t WindowsRuntime::DeleteHString(Il2CppHString hstring)
{
	NO_UNUSED_WARNING(hstring);
	return IL2CPP_COR_E_PLATFORMNOTSUPPORTED;
}

const Il2CppChar* WindowsRuntime::GetHStringBuffer(Il2CppHString hstring, uint32_t* length)
{
	NO_UNUSED_WARNING(hstring);
	NO_UNUSED_WARNING(length);
	vm::Exception::Raise(IL2CPP_COR_E_PLATFORMNOTSUPPORTED);
	return NULL;
}

Il2CppString* WindowsRuntime::HStringToManagedString(Il2CppHString hstring)
{
	NO_UNUSED_WARNING(hstring);
	vm::Exception::Raise(IL2CPP_COR_E_PLATFORMNOTSUPPORTED);
	return NULL;
}

}
}

#endif