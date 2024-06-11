#include "os/MemoryMappedFile.h"
#include "os/c-api/MemoryMappedFile-c-api.h"

extern "C"
{
void* UnityPalMemoryMappedFileMap(UnityPalFileHandle* file)
{
    return il2cpp::os::MemoryMappedFile::Map(file);
}

void UnityPalMemoryMappedFileUnmap(void* address)
{
    il2cpp::os::MemoryMappedFile::Unmap(address);
}

void* UnityPalMemoryMappedFileMapWithParams(UnityPalFileHandle* file, size_t length, size_t offset)
{
    return il2cpp::os::MemoryMappedFile::Map(file, length, offset);
}

void UnityPalMemoryMappedFileUnmapWithParams(void* address, size_t length)
{
    il2cpp::os::MemoryMappedFile::Unmap(address, length);
}
}
