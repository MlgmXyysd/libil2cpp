#include "il2cpp-config.h"

#if IL2CPP_USE_GENERIC_COM
#include "object-internals.h"
#include "os/COM.h"

namespace il2cpp
{

namespace os
{

il2cpp_hresult_t COM::CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object)
{
	NO_UNUSED_WARNING(clsid);
	assert(object);
	*object = NULL;
	return IL2CPP_E_NOTIMPL;
}

}
}
#endif
