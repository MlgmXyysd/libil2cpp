#include "CustomAttributeCreator.h"
#include "gc/WriteBarrier.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/Field.h"
#include "vm/Method.h"
#include "vm/Object.h"
#include "vm/Property.h"
#include "vm/Runtime.h"

namespace il2cpp
{
namespace metadata
{
    static void* ConvertArgumentValue(const Il2CppType* targetType, const CustomAttributeArgument* arg)
    {
        // If the argument target target is a value type, then just pass a pointer to the data
        if (targetType->valuetype)
            return (void*)&arg->data;

        // Our target isn't value type, but our data is, we need to box
        if (il2cpp::vm::Class::IsValuetype(arg->klass))
            return il2cpp::vm::Object::Box(arg->klass, (void*)&arg->data);

        // Storing reference type data in a reference type field, just get the pointer to the object
        return arg->data.obj;
    }

    Il2CppObject* CustomAttributeCreator::Create(const CustomAttributeData& data, Il2CppException** exc)
    {
        il2cpp::gc::WriteBarrier::GenericStoreNull(exc);

        Il2CppObject* attr = il2cpp::vm::Object::New(data.ctor->klass);

        const MethodInfo* ctor = data.ctor;
        IL2CPP_ASSERT(data.arguments.size() == ctor->parameters_count);
        void** ctorArgs = (void**)alloca(data.arguments.size() * sizeof(void*));
        for (size_t i = 0; i < data.arguments.size(); i++)
            ctorArgs[i] = ConvertArgumentValue(ctor->parameters[i], &data.arguments[i]);

        il2cpp::vm::Runtime::Invoke(ctor, attr, ctorArgs, exc);
        if (*exc != NULL)
            return NULL;

        for (std::vector<CustomAttributeFieldArgument>::const_iterator it = data.fields.begin(); it < data.fields.end(); ++it)
            il2cpp::vm::Field::SetValue(attr, it->field, ConvertArgumentValue(it->field->type, &it->arg));

        for (std::vector<CustomAttributePropertyArgument>::const_iterator it = data.properties.begin(); it < data.properties.end(); ++it)
        {
            const MethodInfo* setMethod = il2cpp::vm::Property::GetSetMethod(it->prop);
            IL2CPP_ASSERT(setMethod->parameters_count == 1);
            void* param = ConvertArgumentValue(setMethod->parameters[0], &it->arg);
            il2cpp::vm::Runtime::Invoke(setMethod, attr, &param, exc);
            if (*exc != NULL)
                return NULL;
        }

        return attr;
    }
}     /* namespace vm */
} /* namespace il2cpp */
