#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_COM
#include "object-internals.h"
#include "os/COM.h"
#include "WindowsHeaders.h"

namespace il2cpp
{

namespace os
{

il2cpp_hresult_t COM::CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object)
{
	assert(object);
	return ::CoCreateInstance(reinterpret_cast<REFCLSID>(clsid), NULL, CLSCTX_ALL, IID_IUnknown, reinterpret_cast<LPVOID*>(object));
}

}
}
#endif
