#include "il2cpp-config.h"

#if IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_NOVA || IL2CPP_TARGET_ANDROID

#include "os/Image.h"

#if !IL2CPP_TARGET_JAVASCRIPT
#include <dlfcn.h>
#endif

extern char __start_il2cpp;
extern char __stop_il2cpp;

namespace il2cpp
{
namespace os
{
namespace Image
{
    void* GetImageBase()
    {
#if IL2CPP_TARGET_JAVASCRIPT
        return NULL;
#else
        Dl_info info;
        void* const anySymbol = reinterpret_cast<void*>(&GetImageBase);
        if (dladdr(anySymbol, &info))
            return info.dli_fbase;
        else
            return NULL;
#endif
    }

    static IL2CPP_METHOD_ATTR void NoGeneratedCodeWorkaround()
    {
    }

    void InitializeManagedSection()
    {
        NoGeneratedCodeWorkaround();

        // Since the native linker creates the __start_il2cpp and __stop_il2cpp
        // globals, we can only use them when IL2CPP_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        // is defined. Otherwise, they will not exist, and this usage of them will cause
        // an unresolved external error in the native linker. This should be the only
        // place in runtime code that IL2CPP_PLATFORM_SUPPORTS_CUSTOM_SECTIONS is used.
#if IL2CPP_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        SetManagedSectionStartAndEnd(&__start_il2cpp, &__stop_il2cpp);
#endif
    }

    void Initialize()
    {
        InitializeManagedSection();
    }
}
}
}

#endif
