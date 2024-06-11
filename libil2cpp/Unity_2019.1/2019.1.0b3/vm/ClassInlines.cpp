#include "ClassInlines.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Method.h"
#include "gc/GCHandle.h"

namespace il2cpp
{
namespace vm
{
    bool ClassInlines::InitFromCodegenSlow(Il2CppClass *klass)
    {
        bool result = Class::Init(klass);

        if (klass->has_initialization_error)
            il2cpp::vm::Exception::Raise((Il2CppException*)gc::GCHandle::GetTarget(klass->initializationExceptionGCHandle));

        return result;
    }

    NORETURN static void RaiseExceptionForNotFoundInterface(const Il2CppClass* klass, const Il2CppClass* itf, Il2CppMethodSlot slot)
    {
        std::string message;
        message = "Attempt to access method '" + Type::GetName(&itf->byval_arg, IL2CPP_TYPE_NAME_FORMAT_IL) + "." + Method::GetName(itf->methods[slot])
            + "' on type '" + Type::GetName(&klass->byval_arg, IL2CPP_TYPE_NAME_FORMAT_IL) + "' failed.";
        Exception::Raise(il2cpp::vm::Exception::GetMethodAccessException(message.c_str()));
    }

    const VirtualInvokeData* ClassInlines::GetInterfaceInvokeDataFromVTableSlowPath(const Il2CppClass* klass, const Il2CppClass* itf, Il2CppMethodSlot slot)
    {
#if NET_4_0
        if (itf->generic_class != NULL)
        {
            const Il2CppTypeDefinition* genericInterface = MetadataCache::GetTypeDefinitionFromIndex(itf->generic_class->typeDefinitionIndex);
            const Il2CppGenericContainer* genericContainer = MetadataCache::GetGenericContainerFromIndex(genericInterface->genericContainerIndex);

            for (uint16_t i = 0; i < klass->interface_offsets_count; ++i)
            {
                const Il2CppRuntimeInterfaceOffsetPair* pair = klass->interfaceOffsets + i;
                if (Class::IsGenericClassAssignableFrom(itf, pair->interfaceType, genericContainer))
                {
                    IL2CPP_ASSERT(pair->offset + slot < klass->vtable_count);
                    return &klass->vtable[pair->offset + slot];
                }
            }
        }
#endif

        return NULL;
    }

    const VirtualInvokeData& ClassInlines::GetInterfaceInvokeDataFromVTableSlowPath(const Il2CppObject* obj, const Il2CppClass* itf, Il2CppMethodSlot slot)
    {
        const Il2CppClass* klass = obj->klass;
        const VirtualInvokeData* data;

        data = GetInterfaceInvokeDataFromVTableSlowPath(klass, itf, slot);
        if (data)
            return *data;

        if (klass->is_import_or_windows_runtime)
        {
            Il2CppIUnknown* iunknown = static_cast<const Il2CppComObject*>(obj)->identity;

            // It might be null if it's called on a dead (already released) or fake object
            if (iunknown != NULL)
            {
                if (itf->vtable_count > 0)
                {
                    IL2CPP_ASSERT(slot < itf->vtable_count);

                    // Nothing will be referencing these types directly, so we need to initialize them here
                    const VirtualInvokeData& invokeData = itf->vtable[slot];
                    Class::Init(invokeData.method->klass);
                    return invokeData;
                }

                // TO DO: add support for covariance/contravariance for projected interfaces like
                // System.Collections.Generic.IEnumerable`1<T>
            }
        }

        RaiseExceptionForNotFoundInterface(klass, itf, slot);
        IL2CPP_UNREACHABLE;
    }
}
}
