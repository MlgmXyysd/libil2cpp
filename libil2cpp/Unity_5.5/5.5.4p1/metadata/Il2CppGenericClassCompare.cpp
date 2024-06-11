#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericClassCompare.h"
#include "Il2CppGenericInstCompare.h"

namespace il2cpp
{
namespace metadata
{
    bool Il2CppGenericClassCompare::operator()(const KeyWrapper<Il2CppGenericClass*>& gc1, const KeyWrapper<Il2CppGenericClass*>& gc2) const
    {
        return Compare(gc1, gc2);
    }

    bool Il2CppGenericClassCompare::Compare(const KeyWrapper<Il2CppGenericClass*>& gc1, const KeyWrapper<Il2CppGenericClass*>& gc2)
    {
        if (gc1.type != gc2.type)
            return false;
        else if (!gc1.isNormal())
            return true;

        const Il2CppGenericInst *i1 = gc1.key->context.class_inst;
        const Il2CppGenericInst *i2 = gc2.key->context.class_inst;

        if (gc1.key->typeDefinitionIndex != gc2.key->typeDefinitionIndex)
            return false;

        return Il2CppGenericInstCompare::Compare(i1, i2);
    }
} /* namespace vm */
} /* namespace il2cpp */
