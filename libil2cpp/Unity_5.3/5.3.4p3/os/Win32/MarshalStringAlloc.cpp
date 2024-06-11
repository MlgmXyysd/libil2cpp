#include "il2cpp-config.h"

#if !IL2CPP_USE_GENERIC_COM
#include "os/MarshalStringAlloc.h"
#include "WindowsHeaders.h"

namespace il2cpp
{

namespace os
{

il2cpp_hresult_t MarshalStringAlloc::AllocateBStringLength(const uint16_t* text, int32_t length, uint16_t** bstr)
{
	assert(bstr);
	*bstr = (uint16_t*)::SysAllocStringLen((const OLECHAR*)text, length);
	return *bstr ? IL2CPP_S_OK : IL2CPP_E_OUTOFMEMORY;
}

il2cpp_hresult_t MarshalStringAlloc::GetBStringLength(uint16_t* bstr, int32_t* length)
{
	assert(length);
	*length = ::SysStringLen((BSTR)bstr);
	return IL2CPP_S_OK;
}

il2cpp_hresult_t MarshalStringAlloc::FreeBString(uint16_t* bstr)
{
	::SysFreeString((BSTR)bstr);
	return IL2CPP_S_OK;
}

}
}
#endif
