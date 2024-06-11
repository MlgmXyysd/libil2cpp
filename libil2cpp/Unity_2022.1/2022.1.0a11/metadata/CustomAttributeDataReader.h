#pragma once

#include <stdint.h>
#include <vector>
#include "il2cpp-object-internals.h"
#include "gc/Allocator.h"

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

    struct LazyCustomAttributeData
    {
        const MethodInfo* ctor;
        const void* dataStart;
        uint32_t dataLength;
    };

    struct CustomAttributeData
    {
        const MethodInfo* ctor;
    };

    class CustomAttributeReaderVisitor
    {
    public:
        // This Visitor methods will be called in the defined order
        virtual void VisitArgumentSizes(uint32_t argumentCount, uint32_t fieldCount, uint32_t propertyCount) {}
        virtual void VisitArgument(const CustomAttributeArgument& argument, uint32_t index) {}
        virtual void VisitCtor(const MethodInfo* ctor, CustomAttributeArgument args[], uint32_t arguementCount) {}
        virtual void VisitField(const CustomAttributeFieldArgument& field, uint32_t index) {}
        virtual void VisitProperty(const CustomAttributePropertyArgument& prop, uint32_t index) {}
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
        bool ReadLazyCustomAttributeData(const Il2CppImage* image, LazyCustomAttributeData* data, CustomAttributeDataIterator* iter, Il2CppException** exc);

        bool VisitCustomAttributeData(const Il2CppImage* image, CustomAttributeDataIterator* iter, CustomAttributeReaderVisitor* visitor, Il2CppException** exc);

        static bool VisitCustomAttributeData(const Il2CppImage* image, const MethodInfo* ctor, const void* dataStart, uint32_t dataLength, CustomAttributeReaderVisitor* visitor, Il2CppException** exc);

        CustomAttributeCtorIterator GetCtorIterator();
        CustomAttributeDataIterator GetDataIterator();

    private:

        const char* GetDataBufferStart();
        CustomAttributeDataReader(const char* dataStart, uint32_t dataLength);
        bool VisitCustomAttributeDataImpl(const Il2CppImage* image, const MethodInfo* ctor, CustomAttributeDataIterator* iter, CustomAttributeReaderVisitor* visitor, Il2CppException** exc, bool deserializedManagedObjects);

        const char* bufferStart;
        const char* bufferEnd;
        uint32_t count;
    };
}     /* namespace vm */
} /* namespace il2cpp */
