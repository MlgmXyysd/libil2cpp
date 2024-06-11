#pragma once

namespace il2cpp
{
namespace os
{

class MarshalStringAlloc
{
public:
	static il2cpp_hresult_t AllocateBStringLength(const uint16_t* text, int32_t length, uint16_t** bstr);
	static il2cpp_hresult_t GetBStringLength(uint16_t* bstr, int32_t* length);
	static il2cpp_hresult_t FreeBString(uint16_t* bstr);
};

} /* namespace os */
} /* namespace il2cpp*/
