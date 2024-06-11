#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "gc/gc-internal.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/RCW.h"
#include "os/COM.h"
#include "os/Mutex.h"
#include "utils/StdUnorderedMap.h"

const Il2CppGuid Il2CppIUnknown::IID = { 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46 };

namespace il2cpp
{

using namespace os;

namespace vm
{

typedef unordered_map<Il2CppIUnknown*, Il2CppRCW*> RCWCache;

static FastMutex g_CacheMutex;
static RCWCache g_Cache;

void RCW::Initialize(Il2CppRCW* rcw, const Il2CppGuid& clsid)
{
	const il2cpp_hresult_t hr = COM::CreateInstance(clsid, &rcw->identity);
	Exception::RaiseIfFailed(hr);

	{
		FastAutoLock lock(&g_CacheMutex);
		const bool inserted = g_Cache.insert(std::make_pair(rcw->identity, rcw)).second;
		NO_UNUSED_WARNING(inserted);
		assert(inserted);
	}

	il2cpp_gc_register_finalizer_with_callback(rcw, &RCW::Cleanup);
}

void RCW::Cleanup(void* obj, void* data)
{
	Il2CppRCW* rcw = static_cast<Il2CppRCW*>(obj);

	// RCW is removed from the cache before finalizer is run.
	// In case finalizer somehow resurrects RCW, duplicate RCW object will be created.
	// That's not desired but it's better than corrupted cache.
	{
		FastAutoLock lock(&g_CacheMutex);
		const size_t erased = g_Cache.erase(rcw->identity);
		NO_UNUSED_WARNING(erased);
		assert(1 == erased);
	}

	if (rcw->klass->has_finalize)
		il2cpp_gc_run_finalize(obj, data);

	if (rcw->identity)
	{
		rcw->identity->Release();
		rcw->identity = NULL;
	}
}

Il2CppIUnknown* RCW::QueryInterface(Il2CppRCW* rcw, const Il2CppGuid& iid)
{
	assert(rcw);
	assert(rcw->identity);
	Il2CppIUnknown* result;
	const il2cpp_hresult_t hr = rcw->identity->QueryInterface(iid, reinterpret_cast<void**>(&result));
	Exception::RaiseIfFailed(hr);
	assert(result);
	return result;
}

Il2CppRCW* RCW::Create(Il2CppIUnknown* unknown)
{
	if (!unknown)
		return NULL;

	Il2CppIUnknown* identity;
	const il2cpp_hresult_t hr = unknown->QueryInterface(Il2CppIUnknown::IID, reinterpret_cast<void**>(&identity));
	Exception::RaiseIfFailed(hr);
	assert(identity);

	Il2CppRCW* rcw;

	{
		FastAutoLock lock(&g_CacheMutex);
		RCWCache::iterator iter = g_Cache.find(identity);
		if (iter != g_Cache.end())
		{
			identity->Release();
			return iter->second;
		}

		rcw = static_cast<Il2CppRCW*>(Object::AllocatePtrFree(sizeof(Il2CppRCW), il2cpp_defaults.object_class));
#if NEED_TO_ZERO_PTRFREE
		memset((char*)rcw + sizeof(Il2CppObject), 0, sizeof(Il2CppRCW) - sizeof(Il2CppObject));
#endif

		rcw->identity = identity;

		const bool inserted = g_Cache.insert(std::make_pair(rcw->identity, rcw)).second;
		NO_UNUSED_WARNING(inserted);
		assert(inserted);
	}

	il2cpp_gc_register_finalizer_with_callback(rcw, &RCW::Cleanup);

	return rcw;
}

} /* namespace vm */
} /* namespace il2cpp */
