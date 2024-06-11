#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppGenericMethodCompare.h"
#include "Il2CppGenericContextCompare.h"

namespace il2cpp
{
namespace metadata
{
    bool Il2CppGenericMethodCompare::operator()(const KeyWrapper<const Il2CppGenericMethod*>& m1, const KeyWrapper<const Il2CppGenericMethod*>& m2) const
    {
        return Equals(m1, m2);
    }

    bool Il2CppGenericMethodCompare::Equals(const KeyWrapper<const Il2CppGenericMethod*>& m1, const KeyWrapper<const Il2CppGenericMethod*>& m2)
    {
        if (m1.type != m2.type)
            return false;
        else if (!m1.isNormal())
            return true;

        if (m1.key->methodDefinition != m2.key->methodDefinition)
            return false;

        return Il2CppGenericContextCompare::Compare(&m1.key->context, &m2.key->context);
    }
} /* namespace vm */
} /* namespace il2cpp */
