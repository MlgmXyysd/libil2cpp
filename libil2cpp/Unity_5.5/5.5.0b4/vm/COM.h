#pragma once

#include "class-internals.h"
#include "os/COM.h"
#include "vm/Exception.h"

struct Il2CppGuid;
struct Il2CppSafeArrayBound;
struct Il2CppSafeArray;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API COM
{
public:
	static inline void CreateInstance(const Il2CppGuid& clsid, Il2CppIUnknown** object)
	{
		const il2cpp_hresult_t hr = os::COM::CreateInstance(clsid, object);
		Exception::RaiseIfFailed(hr);
	}

	static inline il2cpp_hresult_t CreateFreeThreadedMarshaler(Il2CppIUnknown* outer, Il2CppIUnknown** marshal)
	{
		return os::COM::CreateFreeThreadedMarshaler(outer, marshal);
	}

	static void MarshalVariant(Il2CppObject* obj, Il2CppVariant* variant);
	static Il2CppObject* MarshalVariantResult(Il2CppVariant* variant);
	static void DestroyVariant(Il2CppVariant* variant);
	static Il2CppSafeArray* MarshalSafeArray(uint16_t variantType, Il2CppArray* managedArray);
	static Il2CppArray* MarshalSafeArrayResult(uint16_t variantType, Il2CppClass* type, Il2CppSafeArray* safeArray);
	static Il2CppSafeArray* MarshalSafeArrayBString(Il2CppArray* managedArray);
	static Il2CppArray* MarshalSafeArrayBStringResult(Il2CppClass* type, Il2CppSafeArray* safeArray);

	static inline void DestroySafeArray(Il2CppSafeArray* safeArray)
	{
		const il2cpp_hresult_t hr = os::COM::SafeArrayDestroy(safeArray);
		Exception::RaiseIfFailed(hr);
	}

	static inline bool IsCOMObject(Il2CppObject* obj)
	{
		return obj->klass == il2cpp_defaults.il2cpp_com_object_class;
	}
};

} /* namespace vm */
} /* namespace il2cpp */
