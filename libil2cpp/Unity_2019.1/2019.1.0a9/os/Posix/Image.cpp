#include "il2cpp-config.h"

#if IL2CPP_TARGET_JAVASCRIPT || IL2CPP_TARGET_LINUX || IL2CPP_TARGET_ANDROID || IL2CPP_TARGET_LUMIN

#if IL2CPP_TARGET_JAVASCRIPT
#include <emscripten/emscripten.h>
#else
#include <dlfcn.h>
#endif

namespace il2cpp
{
namespace os
{
namespace Image
{
    void Initialize()
    {
    }

    void* GetImageBase()
    {
#if IL2CPP_TARGET_JAVASCRIPT
        emscripten_log(EM_LOG_NO_PATHS | EM_LOG_CONSOLE | EM_LOG_ERROR | EM_LOG_JS_STACK, "Warning: libil2cpp/os/Posix/Image.cpp: GetImageBase() called, but dynamic libraries are not available.");
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
}
}
}

#endif
