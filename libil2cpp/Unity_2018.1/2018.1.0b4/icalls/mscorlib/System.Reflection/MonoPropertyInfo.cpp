#include "il2cpp-config.h"
#include <stddef.h>
#include "icalls/mscorlib/System.Reflection/MonoPropertyInfo.h"
#include "il2cpp-class-internals.h"
#include "il2cpp-object-internals.h"
#include "vm/Reflection.h"
#include "vm/String.h"
#include "vm/Exception.h"

using namespace il2cpp::vm;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{
    void MonoPropertyInfo::get_property_info(Il2CppReflectionProperty *property, Il2CppPropertyInfo *info, PInfo req_info)
    {
        if ((req_info & PInfo_ReflectedType) != 0)
            IL2CPP_STRUCT_SETREF(info, parent, vm::Reflection::GetTypeObject(property->klass->byval_arg));
        else if ((req_info & PInfo_DeclaringType) != 0)
        {
#if !NET_4_0
            IL2CPP_STRUCT_SETREF(info, parent, vm::Reflection::GetTypeObject(property->property->parent->byval_arg));
#else
            IL2CPP_STRUCT_SETREF(info, declaringType, vm::Reflection::GetTypeObject(property->property->parent->byval_arg));
#endif
        }

        if ((req_info & PInfo_Name) != 0)
            IL2CPP_STRUCT_SETREF(info, name, vm::String::New(property->property->name));

        if ((req_info & PInfo_Attributes) != 0)
            info->attrs = property->property->attrs;

        if ((req_info & PInfo_GetMethod) != 0)
            IL2CPP_STRUCT_SETREF(info, get, property->property->get ?
                vm::Reflection::GetMethodObject(property->property->get, property->klass) : NULL);

        if ((req_info & PInfo_SetMethod) != 0)
            IL2CPP_STRUCT_SETREF(info, set, property->property->set ?
                vm::Reflection::GetMethodObject(property->property->set, property->klass) : NULL);
        /*
         * There may be other methods defined for properties, though, it seems they are not exposed
         * in the reflection API
         */
    }

    Il2CppArray* MonoPropertyInfo::GetTypeModifiers(void* /* System.Reflection.MonoProperty */ prop, bool optional)
    {
        NOT_SUPPORTED_IL2CPP(MonoPropertyInfo::GetTypeModifiers, "This icall is not supported by il2cpp.");

        return 0;
    }

#if NET_4_0
    Il2CppObject* MonoPropertyInfo::get_default_value(Il2CppReflectionProperty* prop)
    {
        IL2CPP_NOT_IMPLEMENTED_ICALL(MonoPropertyInfo::get_default_value);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

#endif
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
