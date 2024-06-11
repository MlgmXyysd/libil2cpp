#pragma once

#include "gc/GCHandle.h"
#include "il2cpp-string-types.h"
#include "os/WindowsRuntime.h"
#include "vm/Atomic.h"
#include "vm/COM.h"
#include "vm/Exception.h"
#include "vm/MarshalAlloc.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"

struct Il2CppObject;
struct Il2CppIUnknown;

namespace il2cpp
{
namespace vm
{

namespace details
{

struct ComNil;

template <typename I0 = ComNil, typename I1 = ComNil, typename I2 = ComNil, typename I3 = ComNil, typename I4 = ComNil, typename I5 = ComNil, typename I6 = ComNil, typename I7 = ComNil, typename I8 = ComNil, typename I9 = ComNil>
struct ComChain
{
	typedef I0 Current;
	typedef ComChain<I1, I2, I3, I4, I5, I6, I7, I8, I9, ComNil> Next;
};

template <>
struct ComChain<ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil, ComNil>
{
	typedef Il2CppIUnknown Current;
};

typedef ComChain<> ComChainLast;

template <typename Chain>
struct ComInterface : Chain::Current, ComInterface<typename Chain::Next>
{
};

template <>
struct ComInterface<ComChainLast> : ComChainLast::Current
{
};

template <typename Interface>
struct HiddenInterface : Interface
{
};

template <typename T, typename Chain>
struct QueryInterfaceHelper
{
	inline static il2cpp_hresult_t QueryInterface(T* self, const Il2CppGuid& iid, void** object)
	{
		if (!::memcmp(&iid, &Chain::Current::IID, sizeof(Il2CppGuid)))
		{
			*object = static_cast<typename Chain::Current*>(self);
			self->AddRef();
			return IL2CPP_S_OK;
		}
		return QueryInterfaceHelper<T, typename Chain::Next>::QueryInterface(self, iid, object);
	}
};

template <typename T>
struct QueryInterfaceHelper<T, ComChainLast>
{
	inline static il2cpp_hresult_t QueryInterface(T* self, const Il2CppGuid& iid, void** object)
	{
		NO_UNUSED_WARNING(self);
		NO_UNUSED_WARNING(iid);
		*object = NULL;
		return IL2CPP_E_NOINTERFACE;
	}
};

template<typename T, typename U>
struct IsSame
{
	static const bool value = false;
};

template<typename T>
struct IsSame<T, T>
{
	static const bool value = true;
};

template <typename B, typename D>
struct IsBaseOf
{
private:
	template <typename BT, typename DT>
	struct Converter
	{
		operator BT*() const;
		operator DT*();
	};

	typedef int16_t Derived;
	typedef int8_t NotDerived;

	template <typename T>
	static Derived IsDerived(D*, T);
	static NotDerived IsDerived(B*, int);

public:
	static const bool value = IsSame<B, D>::value || sizeof(IsDerived(Converter<B, D>(), int())) == sizeof(Derived);
};

template <typename Base, typename Chain>
struct IsBaseOfChained
{
	static const bool value = IsBaseOf<Base, typename Chain::Current>::value || IsBaseOfChained<Base, typename Chain::Next>::value;
};

template <typename Base>
struct IsBaseOfChained<Base, ComChainLast>
{
	static const bool value = false;
};

template <typename TargetType, typename Chain, bool canCastCurrent = IsBaseOf<TargetType, typename Chain::Current>::value>
struct CastHelper
{
	template <typename T>
	inline static TargetType* Cast(T* value)
	{
		return static_cast<typename Chain::Current*>(value);
	}
};

template <typename TargetType, typename Chain>
struct CastHelper<TargetType, Chain, false>
{
	template <typename T>
	inline static TargetType* Cast(T* value)
	{
		return CastHelper<TargetType, typename Chain::Next>::Cast(value);
	}
};

template <typename TargetType>
struct CastHelper<TargetType, ComChainLast, false>
{
	template <typename T>
	inline static TargetType* Cast(T* value)
	{
		return NULL;
	}
};

template <typename Interface>
struct IsHidden
{
	static const bool value = false;
};

template <typename Interface>
struct IsHidden<HiddenInterface<Interface> >
{
	static const bool value = true;
};

template <typename Interface>
struct ShouldCountInterface
{
	// From GetIids docs: The IUnknown and IInspectable interfaces are excluded.
	// From CoreCLR source code: only IInspectable interfaces are included.
	static const bool value = !IsHidden<Interface>::value && !IsSame<Interface, Il2CppIUnknown>::value && !IsSame<Interface, Il2CppIInspectable>::value && IsBaseOf<Il2CppIInspectable, Interface>::value;
};

template <typename Chain>
struct InterfaceCounter
{
	static const uint32_t value = ShouldCountInterface<typename Chain::Current>::value + InterfaceCounter<typename Chain::Next>::value;
};

template <>
struct InterfaceCounter<ComChainLast>
{
	static const uint32_t value = 0;
};

template <typename Chain>
struct InterfaceFiller
{
	inline static void Fill(Il2CppGuid* iids)
	{
		if (ShouldCountInterface<typename Chain::Current>::value)
		{
			*iids = Chain::Current::IID;
			iids++;
		}

		InterfaceFiller<typename Chain::Next>::Fill(iids);
	}
};

template <>
struct InterfaceFiller<ComChainLast>
{
	inline static void Fill(Il2CppGuid* iids)
	{
	}
};

} /* namespace details */

// Alright, so the lifetime of this guy is pretty weird
// For a single managed object, the IUnknown of its COM Callable Wrapper must always be the same
// That means that we have to keep the same COM Callable Wrapper alive for an object once we create it
// They are cached in il2cpp::vm::g_CCWCache, which is managed by il2cpp::vm::CCW class
//
// Here comes the tricky part: when a native object has a reference to the COM Callable Wrapper,
// the managed object is not supposed to be garbage collected. However, when no native objects are referencing
// it, it should not prevent the GC from collecting the managed object. We implement this by keeping a GC handle
// on the managed object if our reference count is 1 or more. We acquire it when it gets increased from 0 (this
// is safe because such AddRef can only come when this object is retrieved from CCW Cache) and release the GC
// handle when our reference count gets decreased to 0. Here's a kicker: we don't destroy the COM Callable Wrapper
// when the reference count reaches 0; we instead rely on GC finalizer of the managed object to both remove it from
// CCW cache and also destroy it.
template <typename T, typename I0 = details::ComNil, typename I1 = details::ComNil, typename I2 = details::ComNil, typename I3 = details::ComNil, typename I4 = details::ComNil, typename I5 = details::ComNil, typename I6 = details::ComNil, typename I7 = details::ComNil>
struct NOVTABLE ComObjectBase : details::ComInterface<details::ComChain<details::HiddenInterface<Il2CppIManagedObject>, details::HiddenInterface<Il2CppIManagedObjectHolder>, I0, I1, I2, I3, I4, I5, I6, I7> >
{
private:
	volatile uint32_t m_RefCount;
	Il2CppIUnknown* m_Marshal;
	Il2CppObject* m_ManagedObject;
	uint32_t m_GCHandle;

	typedef details::ComChain<details::HiddenInterface<Il2CppIManagedObject>, details::HiddenInterface<Il2CppIManagedObjectHolder>, I0, I1, I2, I3, I4, I5, I6, I7> MyChain;

public:
	inline ComObjectBase(Il2CppObject* obj) : 
		m_RefCount(0),
		m_ManagedObject(obj),
		m_GCHandle(0)
	{
		IL2CPP_ASSERT(obj != NULL);

		il2cpp_hresult_t hr = COM::CreateFreeThreadedMarshaler(details::CastHelper<Il2CppIUnknown, MyChain>::Cast(this), &m_Marshal);
		Exception::RaiseIfFailed(hr);
	}

	inline ~ComObjectBase()
	{
		if (m_Marshal)
			m_Marshal->Release();
	}

	inline static Il2CppIManagedObjectHolder* __CreateInstance(Il2CppObject* obj)
	{
		void* memory = utils::Memory::Malloc(sizeof(T));
		if (!memory)
			Exception::Raise(IL2CPP_E_OUTOFMEMORY);
		return new(memory) T(obj);
	}

	virtual void STDCALL Destroy() IL2CPP_OVERRIDE
	{
		IL2CPP_ASSERT(m_RefCount == 0);

		T* instance = static_cast<T*>(this);
		instance->~T();
		utils::Memory::Free(instance);
	}

	inline Il2CppIUnknown* GetIdentity()
	{
		return details::CastHelper<Il2CppIUnknown, MyChain>::Cast(this);
	}

	virtual il2cpp_hresult_t STDCALL QueryInterface(const Il2CppGuid& iid, void** object) IL2CPP_OVERRIDE
	{
		if (!object)
			return IL2CPP_E_POINTER;
		
		if (!::memcmp(&iid, &Il2CppIUnknown::IID, sizeof(Il2CppGuid)))
		{
			*object = GetIdentity();
			AddRef();
			return IL2CPP_S_OK;
		}

		if (details::IsBaseOfChained<Il2CppIInspectable, MyChain>::value && !::memcmp(&iid, &Il2CppIInspectable::IID, sizeof(Il2CppGuid)))
		{
			*object = details::CastHelper<Il2CppIInspectable, MyChain>::Cast(this);
			AddRef();
			return IL2CPP_S_OK;
		}

		if (!::memcmp(&iid, &Il2CppIMarshal::IID, sizeof(Il2CppGuid)))
			return m_Marshal->QueryInterface(iid, object);

		return details::QueryInterfaceHelper<ComObjectBase, MyChain>::QueryInterface(this, iid, object);
	}

	virtual uint32_t STDCALL AddRef() IL2CPP_OVERRIDE
	{
		const uint32_t refCount = Atomic::Increment(&m_RefCount);

		if (refCount == 1)
		{
			IL2CPP_ASSERT(m_GCHandle == 0);
			m_GCHandle = gc::GCHandle::New(m_ManagedObject, false);
		}

		return refCount;
	}

	virtual uint32_t STDCALL Release() IL2CPP_OVERRIDE
	{
		const uint32_t count = Atomic::Decrement(&m_RefCount);
		if (count == 0)
		{
			IL2CPP_ASSERT(m_GCHandle != 0);
			gc::GCHandle::Free(m_GCHandle);
			m_GCHandle = 0;
		}

		return count;
	}

	il2cpp_hresult_t STDCALL GetIids(uint32_t* iidCount, Il2CppGuid** iids) IL2CPP_OVERRIDE
	{
		Il2CppStaticAssert(details::IsBaseOfChained<Il2CppIInspectable, MyChain>::value);

		uint32_t interfaceCount = details::InterfaceCounter<MyChain>::value;
		Il2CppGuid* interfaceIds = static_cast<Il2CppGuid*>(vm::MarshalAlloc::Allocate(interfaceCount * sizeof(Il2CppGuid)));

		details::InterfaceFiller<MyChain>::Fill(interfaceIds);

		*iidCount = interfaceCount;
		*iids = interfaceIds;
		return IL2CPP_S_OK;
	}

	il2cpp_hresult_t STDCALL GetRuntimeClassName(Il2CppHString* className) IL2CPP_OVERRIDE
	{
		Il2CppStaticAssert(details::IsBaseOfChained<Il2CppIInspectable, MyChain>::value);

		Il2CppClass* objectClass = GetManagedObjectInline()->klass;
		UTF16String name = utils::StringUtils::Utf8ToUtf16(Class::GetNamespace(objectClass)) + static_cast<Il2CppChar>('.') + utils::StringUtils::Utf8ToUtf16(Class::GetName(objectClass));
		return os::WindowsRuntime::CreateHString(utils::StringView<Il2CppChar>(name.c_str(), name.length()), className);
	}

	il2cpp_hresult_t STDCALL GetTrustLevel(int32_t* trustLevel) IL2CPP_OVERRIDE
	{
		Il2CppStaticAssert(details::IsBaseOfChained<Il2CppIInspectable, MyChain>::value);

		*trustLevel = 0;
		return IL2CPP_S_OK;
	}

	virtual il2cpp_hresult_t STDCALL GetSerializedBuffer(Il2CppChar** bstr) IL2CPP_OVERRIDE
	{
		if (!bstr)
			return IL2CPP_E_POINTER;
		*bstr = NULL;
		return IL2CPP_E_NOTIMPL;
	}

	virtual il2cpp_hresult_t STDCALL GetObjectIdentity(Il2CppChar** bstr_guid, int32_t* app_domain_id, intptr_t* ccw) IL2CPP_OVERRIDE
	{
		if (!bstr_guid || !app_domain_id || !ccw)
			return IL2CPP_E_POINTER;
		*bstr_guid = NULL;
		*app_domain_id = 0;
		*ccw = reinterpret_cast<intptr_t>(gc::GCHandle::GetTarget(this->m_GCHandle));
		return IL2CPP_S_OK;
	}

	inline Il2CppObject* GetManagedObjectInline() const
	{
		return m_ManagedObject;
	}

	virtual Il2CppObject* STDCALL GetManagedObject() IL2CPP_OVERRIDE
	{
		return GetManagedObjectInline();
	}

private:
	ComObjectBase(const ComObjectBase&);
	ComObjectBase& operator=(const ComObjectBase&);
};

} /* namespace vm */
} /* namespace il2cpp */
