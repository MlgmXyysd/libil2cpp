#include "CustomAttributeDataReader.h"
#include "il2cpp-metadata.h"
#include "gc/WriteBarrier.h"
#include "utils/MemoryRead.h"
#include "vm-utils/BlobReader.h"
#include "vm/Class.h"
#include "vm/Exception.h"
#include "vm/MetadataCache.h"

// Custom attribute metadata format
//
// Custom attribute data is tightly packed and is not stored aligned
// it must be read with the helpers in MemoryRead
//
// Header:
// 1 Compressed uint32: Count of attributes types
// n uint32:            Attribute constructor indexes
//
// Argument data immediate follows the header
// There is no size data stored for arguments they must be serialized
// out as they are read.  This relies the writing code exactly matching
// the reading code.  Or else data will be read at the wrong offsets.
//
// Argument data
// 1 Compressed uint32:         Count of constructor arguments
//  n Blob data, variable sized:   Argument data
// 1 Compressed uint32:         Count of field arguments
//  n Blob data, variable sized:   Field argument data
//     Each field data ends with a compressed uint32 of the field index on the type
// 1 Compressed uint32:         Count of property arguments
//  n Blob data, variable sized:   Property argument data
//     Each propert data ends with a compressed uint32 of the property index on the type

// An example format is:
//
// 0x02         - Count of custom attribute constructors
// 0x0010023f   - Method definition index for ctor1
// 0x02001fc1   - Method definition index for ctor2
// 0x01         - Constructor argument count for ctor1
// 0x03         - argument 1 type code
// ....         - argument 1 data
// 0x00         - Field argument for ctor1
// 0x01         - Property count for ctor1
// 0x55         - property type code (enum)
// 0x023F       - type index for enum type (compressed)
// ....         - property 1 data
// 0x02         - Constructor argument count for ctor2
// 0x03         - argument 1 type code
// ....         - argument 1 data
// 0x04         - argument 2 type code
// ....         - argument 2 data
// 0x01         - Field argument count for ctor2
// 0x04         - field 1 type code
// ....         - field 1 data
// 0x00         - Property count for ctor2

static void SetInvalidDataException(Il2CppException** exc)
{
    il2cpp::gc::WriteBarrier::GenericStore(exc, il2cpp::vm::Exception::GetCustomAttributeFormatException("Binary format of the specified custom attribute was invalid."));
}

static bool ReadAttributeDataValue(const Il2CppImage* image, const char** buffer, il2cpp::metadata::CustomAttributeArgument* arg, Il2CppException** exc, bool deserializedManagedObjects)
{
    const Il2CppTypeEnum type = il2cpp::utils::BlobReader::ReadEncodedTypeEnum(image, buffer, &arg->klass);

    if (!il2cpp::utils::BlobReader::GetConstantValueFromBlob(image, type, buffer, &arg->data, deserializedManagedObjects))
    {
        SetInvalidDataException(exc);
        return false;
    }

    if (deserializedManagedObjects && type == IL2CPP_TYPE_SZARRAY && arg->data.obj != NULL)
    {
        // For arrays get the actual array class, not just System.Array
        arg->klass = ((Il2CppArray*)arg->data.obj)->klass;
    }

    return true;
}

namespace il2cpp
{
namespace metadata
{
    CustomAttributeDataReader::CustomAttributeDataReader(const void* buffer, const void* bufferEnd) :
        bufferStart((const char*)buffer), bufferEnd((const char*)bufferEnd)
    {
        if (bufferStart != NULL)
            count = utils::ReadCompressedUInt32(&bufferStart);
        else
            count = 0;
    }

    // private, used by CustomAttributeDataReader::ReadCustomAttributeData(const MethodInfo* ctor, const void* dataStart, uint32_t dataLength, CustomAttributeData* data, Il2CppException** exc)
    CustomAttributeDataReader::CustomAttributeDataReader(const char* dataStart, uint32_t dataLength) :
        bufferStart(dataStart), bufferEnd(dataStart + dataLength), count(0)
    {
    }

    uint32_t CustomAttributeDataReader::GetCount()
    {
        return count;
    }

    CustomAttributeCtorIterator CustomAttributeDataReader::GetCtorIterator()
    {
        return CustomAttributeCtorIterator(bufferStart);
    }

    CustomAttributeDataIterator CustomAttributeDataReader::GetDataIterator()
    {
        return CustomAttributeDataIterator(bufferStart, GetDataBufferStart());
    }

    const char* CustomAttributeDataReader::GetDataBufferStart()
    {
        return (const char*)(((uint32_t*)bufferStart) + count);
    }

    bool CustomAttributeDataReader::IterateAttributeCtors(const Il2CppImage* image, const MethodInfo** attributeCtor, CustomAttributeCtorIterator* iter)
    {
        if (iter->ctorBuffer < GetDataBufferStart())
        {
            MethodIndex ctorIndex = utils::Read32(&iter->ctorBuffer);
            *attributeCtor = il2cpp::vm::MetadataCache::GetMethodInfoFromMethodDefinitionIndex(image, ctorIndex);
            return true;
        }

        *attributeCtor = NULL;
        return false;
    }

    bool CustomAttributeDataReader::ReadLazyCustomAttributeData(const Il2CppImage* image, CustomAttributeLazyData* data, Il2CppException** exc, CustomAttributeDataIterator* iter)
    {
        if (!IterateAttributeCtors(image, &data->ctor, &iter->ctorIter))
            return false;

        data->dataStart = (void*)iter->dataBuffer;
        bool success = ReadCustomAttributeDataImpl(image, data->ctor->klass, NULL, exc, iter);
        data->dataLength = (uint32_t)(iter->dataBuffer - (char*)data->dataStart);
        return success;
    }

    bool CustomAttributeDataReader::ReadCustomAttributeData(const Il2CppImage* image, CustomAttributeData* data, Il2CppException** exc, CustomAttributeDataIterator* iter)
    {
        data->arguments.clear();
        data->fields.clear();
        data->properties.clear();

        if (!IterateAttributeCtors(image, &data->ctor, &iter->ctorIter))
            return false;

        il2cpp::vm::Class::Init(data->ctor->klass);
        return ReadCustomAttributeDataImpl(image, data->ctor->klass, data, exc, iter);
    }

    bool CustomAttributeDataReader::ReadCustomAttributeData(const Il2CppImage* image, const MethodInfo* ctor, const void* dataStart, uint32_t dataLength, CustomAttributeData* data, Il2CppException** exc)
    {
        CustomAttributeDataReader reader = CustomAttributeDataReader((const char*)dataStart, dataLength);
        CustomAttributeDataIterator iter = CustomAttributeDataIterator(NULL, reader.bufferStart);

        data->ctor = ctor;
        return reader.ReadCustomAttributeDataImpl(image, ctor->klass, data, exc, &iter);
    }

    bool CustomAttributeDataReader::ReadCustomAttributeDataImpl(const Il2CppImage* image, const Il2CppClass* attrClass, CustomAttributeData* data, Il2CppException** exc, CustomAttributeDataIterator* iter)
    {
        il2cpp::gc::WriteBarrier::GenericStoreNull(exc);

        if (iter->dataBuffer >= bufferEnd)
        {
            // This should never happen
            IL2CPP_ASSERT(false);
            SetInvalidDataException(exc);
            return false;
        }

        uint32_t argCount = utils::ReadCompressedUInt32(&iter->dataBuffer);
        if (data != NULL)
            data->arguments.reserve(argCount);
        for (; argCount > 0; argCount--)
        {
            CustomAttributeArgument arg = {0};
            if (!ReadAttributeDataValue(image, &iter->dataBuffer, &arg, exc, data != NULL))
                return false;

            if (data != NULL)
                data->arguments.push_back(arg);
        }

        uint32_t fieldCount = utils::ReadCompressedUInt32(&iter->dataBuffer);
        if (data != NULL)
            data->fields.reserve(fieldCount);
        for (; fieldCount > 0; fieldCount--)
        {
            CustomAttributeFieldArgument fieldArg = {0};
            if (!ReadAttributeDataValue(image, &iter->dataBuffer, &fieldArg.arg, exc, data != NULL))
                return false;

            uint32_t fieldIndex = utils::ReadCompressedUInt32(&iter->dataBuffer);
            if (data != NULL)
            {
                IL2CPP_ASSERT(fieldIndex < attrClass->field_count);
                fieldArg.field = &attrClass->fields[fieldIndex];
                data->fields.push_back(fieldArg);
            }
        }

        uint32_t propertyCount = utils::ReadCompressedUInt32(&iter->dataBuffer);
        if (data != NULL)
            data->properties.reserve(propertyCount);
        for (; propertyCount > 0; propertyCount--)
        {
            CustomAttributePropertyArgument propertyArg = {0};
            if (!ReadAttributeDataValue(image, &iter->dataBuffer, &propertyArg.arg, exc, data != NULL))
                return false;

            uint32_t propertyIndex = utils::ReadCompressedUInt32(&iter->dataBuffer);
            if (data != NULL)
            {
                IL2CPP_ASSERT(propertyIndex < attrClass->property_count);
                propertyArg.prop = &attrClass->properties[propertyIndex];
                data->properties.push_back(propertyArg);
            }
        }

        return true;
    }
}
}
