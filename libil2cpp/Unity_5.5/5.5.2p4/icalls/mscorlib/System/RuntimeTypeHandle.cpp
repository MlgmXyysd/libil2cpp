#include "il2cpp-config.h"
#include "object-internals.h"

#include "MonoType.h"
#include "RuntimeTypeHandle.h"
#include "Type.h"

#include "vm/Class.h"
#include "vm/Image.h"
#include "vm/Reflection.h"

#if NET_4_0

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool RuntimeTypeHandle::HasInstantiation(Il2CppReflectionRuntimeType* type)
    {
        return Type::get_IsGenericType(&type->type);
    }

    bool RuntimeTypeHandle::IsArray(Il2CppReflectionRuntimeType* type)
    {
        return Type::IsArrayImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsByRef(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsByRefImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsComObject(Il2CppReflectionRuntimeType* type)
    {
        return false; // il2cpp does not support COM objects, so this is always false
    }

    bool RuntimeTypeHandle::IsGenericTypeDefinition(Il2CppReflectionRuntimeType* type)
    {
        return Type::get_IsGenericTypeDefinition(&type->type);
    }

    bool RuntimeTypeHandle::IsGenericVariable(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::get_IsGenericParameter(&type->type);
    }

    bool RuntimeTypeHandle::IsInstanceOfType(Il2CppReflectionRuntimeType* type, Il2CppObject* o)
    {
        return Type::IsInstanceOfType(&type->type, o);
    }

    bool RuntimeTypeHandle::IsPointer(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsPointerImpl(&type->type);
    }

    bool RuntimeTypeHandle::IsPrimitive(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::IsPrimitiveImpl(&type->type);
    }

    bool RuntimeTypeHandle::type_is_assignable_from(Il2CppReflectionType* a, Il2CppReflectionType* b)
    {
        return Type::type_is_assignable_from(a, b);
    }

    int32_t RuntimeTypeHandle::GetArrayRank(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::GetArrayRank(&type->type);
    }

    int32_t RuntimeTypeHandle::GetMetadataToken(Il2CppReflectionRuntimeType* type)
    {
        return vm::Class::FromSystemType(&type->type)->token;
    }

    Il2CppReflectionAssembly* RuntimeTypeHandle::GetAssembly(Il2CppReflectionRuntimeType* type)
    {
        return vm::Reflection::GetAssemblyObject(vm::Image::GetAssembly(vm::Class::GetImage(vm::Class::FromIl2CppType(type->type.type))));
    }

    Il2CppReflectionModule* RuntimeTypeHandle::GetModule(Il2CppReflectionRuntimeType* type)
    {
        return vm::Reflection::GetModuleObject(vm::Class::GetImage(vm::Class::FromIl2CppType(type->type.type)));
    }

    int32_t RuntimeTypeHandle::GetAttributes(Il2CppReflectionRuntimeType* type)
    {
        return MonoType::get_attributes(&type->type);
    }

    Il2CppReflectionRuntimeType* RuntimeTypeHandle::GetBaseType(Il2CppReflectionRuntimeType* type)
    {
        return reinterpret_cast<Il2CppReflectionRuntimeType*>(MonoType::get_BaseType(&type->type));
    }

    Il2CppReflectionRuntimeType* RuntimeTypeHandle::GetElementType(Il2CppReflectionRuntimeType* type)
    {
        return reinterpret_cast<Il2CppReflectionRuntimeType*>(MonoType::GetElementType(&type->type));
    }

    Il2CppReflectionType* RuntimeTypeHandle::GetGenericTypeDefinition_impl(Il2CppReflectionRuntimeType* type)
    {
        return Type::GetGenericTypeDefinition_impl(&type->type);
    }
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
