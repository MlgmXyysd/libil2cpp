#pragma once

namespace il2cpp
{
namespace os
{

class COM
{
public:
	static il2cpp_hresult_t CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object);
	static il2cpp_hresult_t CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal);
};

} /* namespace os */
} /* namespace il2cpp*/
#pragma once
