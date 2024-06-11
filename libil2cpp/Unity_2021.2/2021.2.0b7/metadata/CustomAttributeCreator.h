#pragma once

#include "metadata/CustomAttributeDataReader.h"
#include "il2cpp-object-internals.h"

namespace il2cpp
{
namespace metadata
{
    class CustomAttributeCreator
    {
    public:
        static Il2CppObject* Create(const CustomAttributeData& data, Il2CppException** exc);
    };
}         /* namespace vm */
} /* namespace il2cpp */
