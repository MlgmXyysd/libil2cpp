#include "il2cpp-config.h"
#include "object-internals.h"
#include "vm/COM.h"
#include "os/COM.h"

namespace il2cpp
{
namespace vm
{

il2cpp_hresult_t COM::CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object)
{
	return os::COM::CreateInstance(clsid, object);
}

il2cpp_hresult_t COM::CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal)
{
	return os::COM::CreateFreeThreadedMarshaler(outer, marshal);
}

} /* namespace vm */
} /* namespace il2cpp */
