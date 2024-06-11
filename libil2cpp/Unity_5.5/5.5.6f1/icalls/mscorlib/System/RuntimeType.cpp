#include "il2cpp-config.h"

#if NET_4_0

#include "Activator.h"
#include "metadata.h"
#include "MonoType.h"
#include "RuntimeType.h"
#include "Type.h"
#include "vm/Type.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    bool RuntimeType::IsTypeExportedToWindowsRuntime(Il2CppObject* type)
    {
        NOT_IMPLEMENTED_ICALL(RuntimeType::IsTypeExportedToWindowsRuntime);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

    bool RuntimeType::IsWindowsRuntimeObjectType(Il2CppObject* type)
    {
        NOT_IMPLEMENTED_ICALL(RuntimeType::IsWindowsRuntimeObjectType);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

    int32_t RuntimeType::get_core_clr_security_level(Il2CppObject* _this)
    {
        NOT_IMPLEMENTED_ICALL(RuntimeType::get_core_clr_security_level);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

    int32_t RuntimeType::GetGenericParameterPosition(Il2CppReflectionRuntimeType* _this)
    {
        return Type::GetGenericParameterPosition(&_this->type);
    }

    Il2CppObject* RuntimeType::CreateInstanceInternal(Il2CppReflectionType* type)
    {
        return Activator::CreateInstanceInternal(type);
    }

    int32_t RuntimeType::GetGenericParameterAttributes(Il2CppReflectionRuntimeType* _this)
    {
        return Type::GetGenericParameterAttributes(&_this->type);
    }

    Il2CppObject* RuntimeType::get_DeclaringMethod(Il2CppObject* _this)
    {
        NOT_IMPLEMENTED_ICALL(RuntimeType::get_DeclaringMethod);
        IL2CPP_UNREACHABLE;
        return NULL;
    }

    Il2CppArray* RuntimeType::GetConstructors_internal(Il2CppReflectionRuntimeType* _this, int32_t bindingAttr, Il2CppReflectionType* reflected_type)
    {
        return MonoType::GetConstructors_internal(&_this->type, bindingAttr, reflected_type);
    }

    Il2CppArray* RuntimeType::GetEvents_internal(Il2CppReflectionRuntimeType* _this, Il2CppString* name, int32_t bindingAttr, Il2CppReflectionType* reflected_type)
    {
        return MonoType::GetEventsByName(&_this->type, name, bindingAttr, reflected_type);
    }

    Il2CppArray* RuntimeType::GetFields_internal(Il2CppReflectionRuntimeType* _this, Il2CppString* name, int32_t bindingAttr, Il2CppReflectionType* reflected_type)
    {
        return MonoType::GetFieldsByName(&_this->type, name, bindingAttr, reflected_type);
    }

    Il2CppArray* RuntimeType::GetMethodsByName(Il2CppReflectionRuntimeType* _this, Il2CppString* name, int32_t bindingAttr, bool ignoreCase, Il2CppReflectionType* reflected_type)
    {
        return MonoType::GetMethodsByName(&_this->type, name, bindingAttr, ignoreCase, reflected_type);
    }

    Il2CppArray* RuntimeType::GetPropertiesByName(Il2CppReflectionRuntimeType* _this, Il2CppString* name, int32_t bindingAttr, bool icase, Il2CppReflectionType* reflected_type)
    {
        return MonoType::GetPropertiesByName(&_this->type, name, bindingAttr, icase, reflected_type);
    }

    Il2CppArray* RuntimeType::GetNestedTypes_internal(Il2CppReflectionRuntimeType* _this, Il2CppString* name, int32_t bindingFlags)
    {
        return MonoType::GetNestedTypesByName(&_this->type, name, bindingFlags);
    }

    Il2CppString* RuntimeType::get_Name(Il2CppReflectionRuntimeType* _this)
    {
        return MonoType::get_Name(&_this->type);
    }

    Il2CppString* RuntimeType::get_Namespace(Il2CppReflectionRuntimeType* _this)
    {
        return MonoType::get_Namespace(&_this->type);
    }

    Il2CppString* RuntimeType::getFullName(Il2CppReflectionRuntimeType* _this, bool full_name, bool assembly_qualified)
    {
        return MonoType::getFullName(&_this->type, full_name, assembly_qualified);
    }

    Il2CppReflectionType* RuntimeType::get_DeclaringType(Il2CppReflectionRuntimeType* _this)
    {
        return vm::Type::GetDeclaringType(_this->type.type);
    }

    Il2CppReflectionType* RuntimeType::make_array_type(Il2CppReflectionRuntimeType* _this, int32_t rank)
    {
        return Type::make_array_type(&_this->type, rank);
    }

    Il2CppReflectionType* RuntimeType::make_byref_type(Il2CppReflectionRuntimeType* _this)
    {
        return Type::make_byref_type(&_this->type);
    }

    Il2CppReflectionType* RuntimeType::MakeGenericType(Il2CppReflectionType* gt, Il2CppArray* types)
    {
        return Type::MakeGenericType(gt, types);
    }

    Il2CppReflectionType* RuntimeType::MakePointerType(Il2CppReflectionType* type)
    {
        return Type::MakePointerType(type);
    }

    Il2CppArray* RuntimeType::GetGenericArgumentsInternal(Il2CppReflectionRuntimeType* _this, bool runtimeArray)
    {
        return vm::Type::GetGenericArgumentsInternal(&_this->type, runtimeArray);
    }

    Il2CppArray* RuntimeType::GetGenericParameterConstraints_impl(Il2CppReflectionRuntimeType* _this)
    {
        return Type::GetGenericParameterConstraints_impl(&_this->type);
    }

    Il2CppArray* RuntimeType::GetInterfaces(Il2CppReflectionRuntimeType* _this)
    {
        return MonoType::GetInterfaces(&_this->type);
    }

    int32_t RuntimeType::GetTypeCodeImplInternal(Il2CppReflectionType* type)
    {
        return Type::GetTypeCodeInternal(type);
    }

    void RuntimeType::GetInterfaceMapData(Il2CppReflectionType* t, Il2CppReflectionType* iface, Il2CppArray** targets, Il2CppArray** methods)
    {
        Type::GetInterfaceMapData(t, iface, targets, methods);
    }

    void RuntimeType::GetPacking(Il2CppReflectionRuntimeType* _this, int32_t* packing, int32_t* size)
    {
        Type::GetPacking(&_this->type, packing, size);
    }
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp

#endif
