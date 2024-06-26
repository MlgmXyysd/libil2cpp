#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "gc/GCHandle.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"
#include "vm/Object.h"
#include "vm/PlatformInvoke.h"
#include "vm/RCW.h"
#include "vm/Runtime.h"
#include "os/COM.h"
#include "os/Mutex.h"
#include "os/WindowsRuntime.h"
#include "utils/Il2CppHashMap.h"
#include "utils/HashUtils.h"
#include "utils/StringUtils.h"

const Il2CppGuid Il2CppIUnknown::IID = { 0x00000000, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppISequentialStream::IID = { 0x0c733a30, 0x2a1c, 0x11ce, 0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d };
const Il2CppGuid Il2CppIStream::IID = { 0x0000000c, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppIMarshal::IID = { 0x00000003, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppIManagedObject::IID = { 0xc3fcc19e, 0xa970, 0x11d2, 0x8b, 0x5a, 0x00, 0xa0, 0xc9, 0xb7, 0xc9, 0xc4 };
const Il2CppGuid Il2CppIManagedObjectHolder::IID = { 0xd4bbc1c8, 0xf5bf, 0x4647, 0x94, 0x95, 0x2e, 0x5c, 0xf, 0x20, 0xf7, 0x5d };
const Il2CppGuid Il2CppIInspectable::IID = { 0xaf86e2e0, 0xb12d, 0x4c6a, 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1E, 0x90 };
const Il2CppGuid Il2CppIActivationFactory::IID = { 0x00000035, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };
const Il2CppGuid Il2CppIRestrictedErrorInfo::IID = { 0x82ba7092, 0x4c88, 0x427d, 0xa7, 0xbc, 0x16, 0xdd, 0x93, 0xfe, 0xb6, 0x7e };
const Il2CppGuid Il2CppILanguageExceptionErrorInfo::IID = { 0x04a2dbf3, 0xdf83, 0x116c, 0x09, 0x46, 0x08, 0x12, 0xab, 0xf6, 0xe0, 0x7d };

using il2cpp::utils::PointerHash;

namespace il2cpp
{

using namespace os;

namespace vm
{

typedef Il2CppHashMap<Il2CppIUnknown*, /* Weak GC Handle */uint32_t, PointerHash<Il2CppIUnknown> > RCWCache;

static FastMutex s_RCWCacheMutex;
static RCWCache s_RCWCache;

void RCW::Register(Il2CppComObject* rcw)
{
	FastAutoLock lock(&s_RCWCacheMutex);
	const bool inserted = s_RCWCache.insert(std::make_pair(rcw->identity, gc::GCHandle::NewWeakref(rcw, false))).second;
	Assert(inserted);
}

static inline Il2CppIUnknown* GetIdentity(Il2CppIUnknown* unknown)
{
	Il2CppIUnknown* identity;
	il2cpp_hresult_t hr = unknown->QueryInterface(Il2CppIUnknown::IID, reinterpret_cast<void**>(&identity));
	Exception::RaiseIfFailed(hr, true);
	IL2CPP_ASSERT(identity);

	return identity;
}

// Shameless comment copycat from .NET Native (https://github.com/dotnet/corert/blob/374c3d47992a7c444ec7d1dfe94b1780de942a55/src/System.Private.Interop/src/Shared/McgComHelpers.cs#L557):
// 1. Prefer using the class returned from GetRuntimeClassName
// 2. Otherwise use the class (if there) in the signature
// 3. Out of options - create Il2CppComObject
static inline Il2CppClass* GetClassForRCW(Il2CppIInspectable* inspectable, Il2CppClass* fallbackClass)
{
	Il2CppHString className;
	il2cpp_hresult_t hr = inspectable->GetRuntimeClassName(&className);
	if (IL2CPP_HR_FAILED(hr) || className == NULL)
		return fallbackClass;
	
	uint32_t classNameLength;
	const Il2CppChar* classNamePtr = os::WindowsRuntime::GetHStringBuffer(className, &classNameLength);
	std::string classNameUtf8 = utils::StringUtils::Utf16ToUtf8(classNamePtr, classNameLength);
	os::WindowsRuntime::DeleteHString(className);

	Il2CppClass* rcwClass = MetadataCache::GetWindowsRuntimeClass(classNameUtf8);
	return rcwClass != NULL ? rcwClass : fallbackClass;
}

static inline Il2CppClass* GetClassForRCW(Il2CppIUnknown* unknown, Il2CppClass* fallbackClass)
{
	Il2CppIInspectable* inspectable;
	il2cpp_hresult_t hr = unknown->QueryInterface(Il2CppIInspectable::IID, reinterpret_cast<void**>(&inspectable));

	if (IL2CPP_HR_FAILED(hr))
		return fallbackClass;

	Il2CppClass* result = GetClassForRCW(inspectable, fallbackClass);
	inspectable->Release();
	return result;
}

Il2CppObject* ReboxIfBoxed(Il2CppIUnknown* comObject, Il2CppClass* objectClass)
{
	const char kWindowsFoundationNamespace[] = "Windows.Foundation";
	const char kIReferenceClassName[] = "IReference`1";

	if (strcmp(objectClass->namespaze, kWindowsFoundationNamespace) != 0)
		return NULL;

	if (strcmp(objectClass->name, kIReferenceClassName) != 0)
		return NULL;

	Class::Init(objectClass);
	
	// Sanity checks
	IL2CPP_ASSERT(Class::IsInflated(objectClass));
	IL2CPP_ASSERT(objectClass->vtable_count == 1); // IReference`1<T> only has get_Value method

	const MethodInfo* getValueMethod = objectClass->vtable[0].method;
	IL2CPP_ASSERT(strcmp(getValueMethod->name, "get_Value") == 0);

	// We don't really want to allocate it on the GC heap for this little invocation
	Il2CppComObject fakeRcw;
	fakeRcw.klass = objectClass;
	fakeRcw.monitor = NULL;
	fakeRcw.identity = comObject;

	Il2CppException* exception = NULL;
	Il2CppObject* reboxed = Runtime::Invoke(getValueMethod, &fakeRcw, NULL, &exception);

	if (exception != NULL)
		Exception::Raise(exception);

	return reboxed;
}

template <typename T, bool isSealedClassInstance>
static inline Il2CppObject* GetOrCreateRCW(T* comObject, Il2CppClass* objectClass)
{
	IL2CPP_ASSERT(comObject != NULL);

	if (!isSealedClassInstance)
	{
		// 1. Check if comObject is actually our COM Callable Wrapper
		Il2CppIManagedObjectHolder* managedHolder;
		il2cpp_hresult_t hr = comObject->QueryInterface(Il2CppIManagedObjectHolder::IID, reinterpret_cast<void**>(&managedHolder));
		if (IL2CPP_HR_SUCCEEDED(hr))
		{
			Il2CppObject* instance = managedHolder->GetManagedObject();
			managedHolder->Release();

			IL2CPP_ASSERT(instance);
			return instance;
		}
	}

	Il2CppIUnknown* identity = GetIdentity(comObject);

	// 2. Try to find it in RCW cache
	FastAutoLock lock(&s_RCWCacheMutex);
	RCWCache::iterator iter = s_RCWCache.find(identity);
	if (iter != s_RCWCache.end())
	{
		Il2CppObject* obj = gc::GCHandle::GetTarget(iter->second);
		if (obj != NULL)
		{
			identity->Release();
			identity = NULL;
			return obj;
		}
		else
		{
			// The RCW was already queued for finalization.
			// Erase it from the cache and let us create a new one.
			s_RCWCache.erase(iter);
		}
	}

	// 3. Figure out the concrete RCW class
	if (!isSealedClassInstance)
	{
		objectClass = GetClassForRCW(comObject, objectClass);

		// If object class is Windows.Foundation.IReference`1<T>,
		// unbox the object from its windows runtime representation,
		// unmarshal it, box it to Il2CppObject and return it
		Il2CppObject* reboxed = ReboxIfBoxed(comObject, objectClass);
		if (reboxed != NULL)
			return reboxed;
	}

	// 4. Create RCW object
	Il2CppComObject* rcw = static_cast<Il2CppComObject*>(Object::New(objectClass));
	rcw->identity = identity;

	// 5. Insert it into the cache
	const bool inserted = s_RCWCache.insert(std::make_pair(identity, gc::GCHandle::NewWeakref(rcw, false))).second;
	Assert(inserted);

	return rcw;
}

Il2CppObject* RCW::GetOrCreateFromIUnknown(Il2CppIUnknown* unknown, Il2CppClass* fallbackClass)
{
	return GetOrCreateRCW<Il2CppIUnknown, false>(unknown, fallbackClass);
}

Il2CppObject* RCW::GetOrCreateFromIInspectable(Il2CppIInspectable* inspectable, Il2CppClass* fallbackClass)
{
	return GetOrCreateRCW<Il2CppIInspectable, false>(inspectable, fallbackClass);
}

Il2CppObject* RCW::GetOrCreateForSealedClass(Il2CppIUnknown* unknown, Il2CppClass* objectClass)
{
	return GetOrCreateRCW<Il2CppIUnknown, true>(unknown, objectClass);
}

void RCW::Cleanup(Il2CppComObject* rcw)
{
	FastAutoLock lock(&s_RCWCacheMutex);

	RCWCache::iterator iter = s_RCWCache.find(rcw->identity);

	// It is possible for us to not find object in the cache if two RCWs for the same IUnknown get
	// finalized in a row: then, the first finalizer will remove the NULL object, and the second one
	// will not find it.
	if (iter != s_RCWCache.end())
	{
		Il2CppObject* obj = gc::GCHandle::GetTarget(iter->second);

		// If it's not NULL, it means that we have already created a new RCW in place 
		// of this one during the time it had been queued for finalization
		if (obj == NULL)
			s_RCWCache.erase(iter);
	}
}

} /* namespace vm */
} /* namespace il2cpp */
