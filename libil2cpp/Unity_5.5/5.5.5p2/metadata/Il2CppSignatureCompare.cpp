#include "il2cpp-config.h"
#include "class-internals.h"
#include "Il2CppTypeCompare.h"
#include "Il2CppSignatureCompare.h"
#include "utils/KeyWrapper.h"

namespace il2cpp
{
namespace metadata
{
    bool Il2CppSignatureCompare::operator()(const KeyWrapper<dynamic_array<const Il2CppType*> >& s1, const KeyWrapper<dynamic_array<const Il2CppType*> >& s2) const
    {
        return Equals(s1, s2);
    }

    bool Il2CppSignatureCompare::Equals(const KeyWrapper<dynamic_array<const Il2CppType*> >& s1, const KeyWrapper<dynamic_array<const Il2CppType*> >& s2)
    {
        if (s1.type != s2.type)
            return false;
        else if (!s1.isNormal())
            return true;

        if (s1.key.size() != s2.key.size())
            return false;

        dynamic_array<const Il2CppType*>::const_iterator s1Iter, s1End = s1.key.end(), s2Iter;

        for (s1Iter = s1.key.begin(), s2Iter = s2.key.begin(); s1Iter != s1End; ++s1Iter, ++s2Iter)
        {
            if (!Il2CppTypeCompare::Compare(*s1Iter, *s2Iter))
                return false;
        }

        return true;
    }
} /* namespace vm */
} /* namespace il2cpp */
