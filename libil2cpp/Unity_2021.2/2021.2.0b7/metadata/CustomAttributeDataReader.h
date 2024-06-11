#pragma once

#include <stdint.h>
#include <vector>
#include "il2cpp-object-internals.h"

namespace il2cpp
{
namespace metadata
{
    // This union is large enough to store any type
    // that can be serialized by an attribute argument
    // bool, byte, char, double, float, int, long, sbyte, short, string, uint, ulong, ushort, System.Object, System.Type, or an enum
    // Or an szarray/vector of the previous types
    union CustomAttributeDataStorage
    {
        Il2CppObject* obj;
        int64_t i;
        double d;
    };

    struct CustomAttributeArgument
    {
        Il2CppClass* klass;
        CustomAttributeDataStorage data;
    };

    struct CustomAttributeFieldArgument
    {
        CustomAttributeArgument arg;
        const FieldInfo* field;
    };

    struct CustomAttributePropertyArgument
    {
        CustomAttributeArgument arg;
        const PropertyInfo* prop;
    };

    struct CustomAttributeData
    {
        const MethodInfo* ctor;
        std::vector<CustomAttributeArgument> arguments;
        std::vector<CustomAttributeFieldArgument> fields;
        std::vector<CustomAttributePropertyArgument> properties;
    };

    struct CustomAttributeLazyData
    {
        const MethodInfo* ctor;
        void* dataStart;
        uint32_t dataLength;
    };

    class CustomAttributeDataIterator;

    class CustomAttributeCtorIterator
    {
    private:
        CustomAttributeCtorIterator(const char* ctorBuffer) : ctorBuffer(ctorBuffer)
        {
        }

        const char* ctorBuffer;

        friend class CustomAttributeDataReader;
        friend class CustomAttributeDataIterator;
    };

    class CustomAttributeDataIterator
    {
    private:
        CustomAttributeDataIterator(const char* ctorBuffer, const char* dataBuffer) : dataBuffer(dataBuffer), ctorIter(ctorBuffer)
        {
        }

        const char* dataBuffer;
        CustomAttributeCtorIterator ctorIter;

        friend class CustomAttributeDataReader;
    };


    class CustomAttributeDataReader
    {
    public:

        CustomAttributeDataReader(const void* buffer, const void* bufferEnd);
        uint32_t GetCount();
        bool IterateAttributeCtors(const Il2CppImage* image, const MethodInfo** attributeCtor, CustomAttributeCtorIterator* iter);
        bool ReadCustomAttributeData(const Il2CppImage* image, CustomAttributeData* data, Il2CppException** exc, CustomAttributeDataIterator* iter);
        bool ReadLazyCustomAttributeData(const Il2CppImage* image, CustomAttributeLazyData* data, Il2CppException** exc, CustomAttributeDataIterator* iter);

        static bool ReadCustomAttributeData(const Il2CppImage* image, const MethodInfo* ctor, const void* dataStart, uint32_t dataLength, CustomAttributeData* data, Il2CppException** exc);

        CustomAttributeCtorIterator GetCtorIterator();
        CustomAttributeDataIterator GetDataIterator();

    private:

        const char* GetDataBufferStart();
        CustomAttributeDataReader(const char* dataStart, uint32_t dataLength);

        bool ReadCustomAttributeDataImpl(const Il2CppImage* image, const Il2CppClass* attrClass, CustomAttributeData* data, Il2CppException** exc, CustomAttributeDataIterator* iter);

        const char* bufferStart;
        const char* bufferEnd;
        uint32_t count;
    };
}     /* namespace vm */
} /* namespace il2cpp */
