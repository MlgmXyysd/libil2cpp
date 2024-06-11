#pragma once
#include <stdint.h>

namespace il2cpp
{
namespace utils
{
    class MemoryUtils
    {
    public:
        IL2CPP_NO_INLINE inline static int MemoryCompareByteByByte(const uint8_t* left, const uint8_t* right, size_t size)
        {
            for (size_t i = 0; i < size; i++)
            {
                uint8_t leftItem = left[i];
                uint8_t rightItem = right[i];

                if (leftItem != rightItem)
                {
                    if (leftItem < rightItem)
                        return -1;

                    return 1;
                }
            }

            return 0;
        }

        inline static int MemoryCompare(const void* left, const void* right, size_t size)
        {
            const uint8_t* leftBytes = static_cast<const uint8_t*>(left);
            const uint8_t* rightBytes = static_cast<const uint8_t*>(right);
            ptrdiff_t leftUnalignedByteCount = (reinterpret_cast<intptr_t>(leftBytes + 3) & ~3) - reinterpret_cast<intptr_t>(leftBytes);
            ptrdiff_t rightUnalignedByteCount = (reinterpret_cast<intptr_t>(rightBytes + 3) & ~3) - reinterpret_cast<intptr_t>(rightBytes);

            // Memory is aligned differently, so the best we can do is byte by byte comparison
            if (leftUnalignedByteCount != rightUnalignedByteCount)
                return MemoryCompareByteByByte(leftBytes, rightBytes, size);

            // Memory is aligned at same size of 4 boundaries, but the start is not at size of 4 alignment
            // In this case, we will scan first unaligned number of bytes and then continue with scanning multiples of 4
            if (leftUnalignedByteCount != 0)
            {
                int unalignedMemoryCompare = MemoryCompareByteByByte(leftBytes, rightBytes, leftUnalignedByteCount);
                if (unalignedMemoryCompare != 0)
                    return unalignedMemoryCompare;

                leftBytes += leftUnalignedByteCount;
                rightBytes += leftUnalignedByteCount;
                size -= leftUnalignedByteCount;
            }

            size_t count4 = size / 4;
            for (size_t i = 0; i < count4; i++)
            {
                uint32_t leftItem = reinterpret_cast<const uint32_t*>(leftBytes)[i];
                uint32_t rightItem = reinterpret_cast<const uint32_t*>(rightBytes)[i];

                if (leftItem != rightItem)
                {
                    if (leftItem < rightItem)
                        return -1;

                    return 1;
                }
            }

            size_t offset = count4 * 4;
            return MemoryCompareByteByByte(leftBytes + offset, rightBytes + offset, size - offset);
        }

        inline static void* MemorySet(void* targetMemory, int value, size_t size)
        {
            uint8_t* ptr = static_cast<uint8_t*>(targetMemory);
            for (size_t i = 0; i != size; i++)
            {
                ptr[i] = static_cast<uint8_t>(value);
            }

            return targetMemory;
        }

        inline static void MemoryCopyByteByByte(uint8_t* target, const uint8_t* source, size_t size)
        {
            for (size_t i = 0; i < size; i++)
                target[i] = source[i];
        }

        inline static void* MemoryCopy(void* target, const void* source, size_t size)
        {
            uint8_t* targetBytes = static_cast<uint8_t*>(target);
            const uint8_t* sourceBytes = static_cast<const uint8_t*>(source);
            ptrdiff_t targetUnalignedByteCount = (reinterpret_cast<intptr_t>(targetBytes + 3) & ~3) - reinterpret_cast<intptr_t>(targetBytes);
            ptrdiff_t sourceUnalignedByteCount = (reinterpret_cast<intptr_t>(sourceBytes + 3) & ~3) - reinterpret_cast<intptr_t>(sourceBytes);

            // Memory is aligned differently, so the best we can do is byte by byte copy
            if (targetUnalignedByteCount != sourceUnalignedByteCount)
            {
                MemoryCopyByteByByte(targetBytes, sourceBytes, size);
                return targetBytes;
            }

            // Memory is aligned at same size of 4 boundaries, but the start is not at size of 4 alignment
            // In this case, we will copy first unaligned number of bytes and then continue with copying multiples of 4
            if (targetUnalignedByteCount != 0)
            {
                MemoryCopyByteByByte(targetBytes, sourceBytes, targetUnalignedByteCount);

                targetBytes += targetUnalignedByteCount;
                sourceBytes += targetUnalignedByteCount;
                size -= targetUnalignedByteCount;
            }

            size_t count4 = size / 4;

            const uint32_t* source32 = reinterpret_cast<const uint32_t*>(sourceBytes);
            uint32_t* target32 = reinterpret_cast<uint32_t*>(targetBytes);

            for (size_t i = 0; i < count4; i++)
                target32[i] = source32[i];

            size_t offset = count4 * 4;
            MemoryCopyByteByByte(targetBytes + offset, sourceBytes + offset, size - offset);

            return target;
        }
    };
}
}
