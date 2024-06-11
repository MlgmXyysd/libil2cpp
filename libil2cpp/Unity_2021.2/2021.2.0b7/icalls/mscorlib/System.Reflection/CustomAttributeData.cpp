#include "il2cpp-config.h"
#include "CustomAttributeData.h"
#include "gc/GarbageCollector.h"
#include "metadata/CustomAttributeDataReader.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Object.h"
#include "vm/Reflection.h"
#include "vm/Runtime.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"

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
    static void SetCustomAttributeTypeArgument(const MethodInfo* ctor, void* argBuffer, const il2cpp::metadata::CustomAttributeArgument * inArg)
    {
        void* params[] = {
            il2cpp::vm::Reflection::GetTypeObject(&inArg->klass->byval_arg),
            il2cpp::vm::Class::IsValuetype(inArg->klass) ? vm::Object::Box(inArg->klass, (void*)&inArg->data) : reinterpret_cast<Il2CppObject*>(inArg->data.obj)
        };
        il2cpp::vm::Runtime::Invoke(ctor, argBuffer, params, NULL);
    }

    static void CreateCustomAttributeData(const il2cpp::metadata::CustomAttributeData& data, Il2CppArray** argArray, Il2CppArray** namedValueArray)
    {
        static const MethodInfo* customAttributeTypedArgumentConstructor;
        static const MethodInfo* customAttributeNamedArgumentConstructor;

        if (!customAttributeTypedArgumentConstructor)
        {
            const Il2CppType* typedArgumentCtorArgTypes[] = { &il2cpp_defaults.systemtype_class->byval_arg, &il2cpp_defaults.object_class->byval_arg };
            customAttributeTypedArgumentConstructor = vm::Class::GetMethodFromNameFlagsAndSig(il2cpp_defaults.customattribute_typed_argument_class, ".ctor", 2, 0, typedArgumentCtorArgTypes);

            const Il2CppType* namedArgumentCtorArgTypes[] = { &il2cpp_defaults.member_info_class->byval_arg, &il2cpp_defaults.customattribute_typed_argument_class->byval_arg };
            customAttributeNamedArgumentConstructor = vm::Class::GetMethodFromNameFlagsAndSig(il2cpp_defaults.customattribute_named_argument_class, ".ctor", 2, 0, namedArgumentCtorArgTypes);

            if (customAttributeTypedArgumentConstructor == NULL || customAttributeNamedArgumentConstructor == NULL)
            {
                customAttributeTypedArgumentConstructor = NULL;
                IL2CPP_NOT_IMPLEMENTED_ICALL(MonoCustomAttrs::GetCustomAttributesDataInternal);
            }

            IL2CPP_ASSERT(il2cpp::vm::Class::IsValuetype(il2cpp_defaults.customattribute_typed_argument_class));
            IL2CPP_ASSERT(il2cpp::vm::Class::IsValuetype(il2cpp_defaults.customattribute_named_argument_class));
        }

        int32_t typedArgSize = il2cpp::vm::Class::GetInstanceSize(il2cpp_defaults.customattribute_typed_argument_class) - sizeof(Il2CppObject);
        int32_t namedArgSize = il2cpp::vm::Class::GetInstanceSize(il2cpp_defaults.customattribute_named_argument_class) - sizeof(Il2CppObject);

        void* typedArgBuffer = alloca(typedArgSize);
        void* namedArgBuffer = alloca(namedArgSize);

        *argArray = il2cpp::vm::Array::New(il2cpp_defaults.object_class, data.arguments.size());
        il2cpp::gc::GarbageCollector::SetWriteBarrier((void**)*argArray);
        for (std::vector<il2cpp::metadata::CustomAttributeArgument>::const_iterator it = data.arguments.begin(); it < data.arguments.end(); ++it)
        {
            memset(typedArgBuffer, 0, typedArgSize);
            SetCustomAttributeTypeArgument(customAttributeTypedArgumentConstructor, typedArgBuffer, &(*it));
            il2cpp_array_setref(*argArray, (it - data.arguments.begin()), il2cpp::vm::Object::Box(il2cpp_defaults.customattribute_typed_argument_class, typedArgBuffer));
        }

        *namedValueArray = il2cpp::vm::Array::New(il2cpp_defaults.customattribute_named_argument_class, data.fields.size() + data.properties.size());
        il2cpp::gc::GarbageCollector::SetWriteBarrier((void**)*argArray);
        for (std::vector<il2cpp::metadata::CustomAttributeFieldArgument>::const_iterator it = data.fields.begin(); it < data.fields.end(); ++it)
        {
            memset(typedArgBuffer, 0, typedArgSize);
            memset(namedArgBuffer, 0, namedArgSize);

            SetCustomAttributeTypeArgument(customAttributeTypedArgumentConstructor, typedArgBuffer, &it->arg);
            void* params[] = {
                vm::Reflection::GetFieldObject(data.ctor->klass, const_cast<FieldInfo*>(it->field)),
                typedArgBuffer
            };

            il2cpp::vm::Runtime::Invoke(customAttributeNamedArgumentConstructor, namedArgBuffer, params, NULL);
            il2cpp_array_setref(*namedValueArray, (it - data.fields.begin()), il2cpp::vm::Object::Box(il2cpp_defaults.customattribute_named_argument_class, namedArgBuffer));
        }

        size_t fieldCount = data.fields.size();
        for (std::vector<il2cpp::metadata::CustomAttributePropertyArgument>::const_iterator it = data.properties.begin(); it < data.properties.end(); ++it)
        {
            memset(typedArgBuffer, 0, typedArgSize);
            memset(namedArgBuffer, 0, namedArgSize);

            SetCustomAttributeTypeArgument(customAttributeTypedArgumentConstructor, typedArgBuffer, &it->arg);
            void* params[] = {
                vm::Reflection::GetPropertyObject(data.ctor->klass, const_cast<PropertyInfo*>(it->prop)),
                typedArgBuffer
            };

            il2cpp::vm::Runtime::Invoke(customAttributeNamedArgumentConstructor, namedArgBuffer, params, NULL);
            il2cpp_array_setref(*namedValueArray, (it - data.properties.begin() + fieldCount), il2cpp::vm::Object::Box(il2cpp_defaults.customattribute_named_argument_class, namedArgBuffer));
        }
    }

    void CustomAttributeData::ResolveArgumentsInternal(Il2CppObject* ctor, Il2CppObject* assembly, intptr_t data, uint32_t data_length, Il2CppArray** ctorArgs, Il2CppArray** namedArgs)
    {
        const MethodInfo* ctorMethod = ((Il2CppReflectionMethod*)ctor)->method;
        const Il2CppImage* image = ((Il2CppReflectionAssembly*)assembly)->assembly->image;

        il2cpp::metadata::CustomAttributeData attributeData;
        Il2CppException* exc;
        if (il2cpp::metadata::CustomAttributeDataReader::ReadCustomAttributeData(image, ctorMethod, (const void*)data, data_length, &attributeData, &exc))
            CreateCustomAttributeData(attributeData, ctorArgs, namedArgs);

        if (exc != NULL)
            vm::Exception::Raise(exc);
    }
} // namespace Reflection
} // namespace System
} // namespace mscorlib
} // namespace icalls
} // namespace il2cpp
