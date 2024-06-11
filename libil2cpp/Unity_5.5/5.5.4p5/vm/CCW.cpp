#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"
#include "vm/Object.h"
#include "vm/CCW.h"
#include "vm/Class.h"
#include "vm/ComUtils.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"
#include "vm/RCW.h"

namespace il2cpp
{
namespace vm
{
    struct ManagedObject : ComObjectBase<ManagedObject, Il2CppIInspectable>
    {
        inline ManagedObject(Il2CppObject* obj) : ComObjectBase<ManagedObject, Il2CppIInspectable>(obj) {}
    };

    typedef Il2CppIManagedObjectHolder* (*CreateCCWFunc)(Il2CppObject* obj);

    Il2CppIManagedObjectHolder* CCW::CreateCCW(Il2CppObject* obj)
    {
        // check for ccw create function, which is implemented by objects that implement COM or Windows Runtime interfaces
        const int32_t index = obj->klass->typeDefinition->ccwFunctionIndex;
        if (index != kMethodIndexInvalid)
        {
            const CreateCCWFunc createCcw = reinterpret_cast<CreateCCWFunc>(MetadataCache::GetCreateCcwFuncFromIndex(index));
            IL2CPP_ASSERT(createCcw);
            return createCcw(obj);
        }

        // otherwise create generic ccw object that "only" implements IUnknown, IMarshal, IInspectable, IManagedObject and IManagedObjectHolder interfaces
        return ManagedObject::__CreateInstance(obj);
    }
} /* namespace vm */
} /* namespace il2cpp */
