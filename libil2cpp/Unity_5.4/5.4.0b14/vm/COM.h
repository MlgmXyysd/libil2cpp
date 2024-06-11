#pragma once

struct Il2CppGuid;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API COM
{
public:
	static il2cpp_hresult_t CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object);
	static il2cpp_hresult_t CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal);
};

} /* namespace vm */
} /* namespace il2cpp */
