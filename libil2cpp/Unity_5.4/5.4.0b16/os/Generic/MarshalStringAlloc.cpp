#include "il2cpp-config.h"

#if IL2CPP_USE_GENERIC_COM
#include "os/MarshalStringAlloc.h"

namespace il2cpp
{

namespace os
{

il2cpp_hresult_t MarshalStringAlloc::AllocateBStringLength(const uint16_t* text, int32_t length, uint16_t** bstr)
{
	NO_UNUSED_WARNING(text);
	NO_UNUSED_WARNING(length);
	assert(bstr);
	*bstr = NULL;
	return IL2CPP_E_NOTIMPL;
}

il2cpp_hresult_t MarshalStringAlloc::GetBStringLength(uint16_t* bstr, int32_t* length)
{
	NO_UNUSED_WARNING(bstr);
	assert(length);
	*length = 0;
	return IL2CPP_E_NOTIMPL;
}

il2cpp_hresult_t MarshalStringAlloc::FreeBString(uint16_t* bstr)
{
	NO_UNUSED_WARNING(bstr);
	return IL2CPP_E_NOTIMPL;
}

}
}
#endif
