#include "il2cpp-config.h"

#if IL2CPP_TARGET_DARWIN

#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <mach-o/ldsyms.h>
#include <vector>

namespace il2cpp
{
namespace os
{
namespace Image
{
    static void* s_ImageBase = NULL;
    static void* s_ManagedSectionStart = NULL;
    static void* s_ManagedSectionEnd = NULL;

    static int GetImageIndex()
    {
        // OSX/iOS uses ASLR (address space layout randomization), so
        // find where the image is loaded. This is usually zero in the
        // debugger for an executable, but non-zero when a debugger is not
        // present.
        std::vector<char> path;
        uint32_t size = 0;
        int error = _NSGetExecutablePath(NULL, &size);
        IL2CPP_ASSERT(error == -1);
        if (error != -1)
            return -1;

        path.resize(size);
        error = _NSGetExecutablePath(&path[0], &size);
        IL2CPP_ASSERT(error == 0);
        if (error != 0)
            return -1;

        int gameAssemblyImageIndex = -1;
        int executableImageIndex = -1;
        int numberOfImages = _dyld_image_count();
        for (uint32_t i = 0; i < numberOfImages; i++)
        {
            const char* imageName = _dyld_get_image_name(i);
            if (strstr(imageName, "GameAssembly.dylib") != NULL)
                gameAssemblyImageIndex = i;
            else if (strcmp(imageName, &path[0]) == 0)
                executableImageIndex = i;
        }

        if (gameAssemblyImageIndex != -1)
            return gameAssemblyImageIndex;
        else if (executableImageIndex != -1)
            return executableImageIndex;

        return -1;
    }

    static void InitializeImageBase()
    {
        int imageIndex = GetImageIndex();
        if (imageIndex != -1)
            s_ImageBase = (void*)_dyld_get_image_vmaddr_slide(imageIndex);
        else
            s_ImageBase = NULL;
    }

    static void InitializeManagedSection()
    {
        int imageIndex = GetImageIndex();
        if (imageIndex == -1)
            return;

        const struct mach_header* header = _dyld_get_image_header(imageIndex);
#if IL2CPP_SIZEOF_VOID_P == 8
        const section_64* sectionData = getsectbynamefromheader_64((const struct mach_header_64*)header, "__TEXT", IL2CPP_BINARY_SECTION_NAME);
#else
        const section* sectionData = getsectbynamefromheader(header, "__TEXT", IL2CPP_BINARY_SECTION_NAME);
#endif

        s_ManagedSectionStart = (void*)((intptr_t)sectionData->addr + (intptr_t)s_ImageBase);
        s_ManagedSectionEnd = (uint8_t*)s_ManagedSectionStart + sectionData->size;
    }

    void Initialize()
    {
        InitializeImageBase();
#if IL2CPP_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        InitializeManagedSection();
#endif
    }

    void* GetImageBase()
    {
        return s_ImageBase;
    }

#if IL2CPP_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
    bool IsInManagedSection(void* ip)
    {
        IL2CPP_ASSERT(s_ManagedSectionStart != NULL && s_ManagedSectionEnd != NULL);
        return s_ManagedSectionStart <= ip && ip <= s_ManagedSectionEnd;
    }

#endif
}
}
}

#endif
