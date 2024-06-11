#include "il2cpp-config.h"
#include "pal_platform.h"

#if IL2CPP_USES_POSIX_CLASS_LIBRARY_PAL

extern "C"
{
    // Items needed by System.Core

    IL2CPP_EXPORT void SystemNative_GetDomainSocketSizes(int32_t* a, int32_t* b, int32_t* c);

    // Items needed by System.IO.Compression

    IL2CPP_EXPORT void* BrotliDecoderCreateInstance(intptr_t a, intptr_t b, intptr_t c);
    IL2CPP_EXPORT int32_t BrotliDecoderDecompressStream(void* a, intptr_t* b, uint8_t** c, intptr_t* d, uint8_t** e, intptr_t* f);
    IL2CPP_EXPORT int32_t BrotliDecoderDecompress(intptr_t a, uint8_t* b, intptr_t* c, uint8_t* d);
    IL2CPP_EXPORT void BrotliDecoderDestroyInstance(intptr_t a);
    IL2CPP_EXPORT int32_t BrotliDecoderIsFinished(void* a);
    IL2CPP_EXPORT void* BrotliEncoderCreateInstance(intptr_t a, intptr_t b, intptr_t c);
    IL2CPP_EXPORT int32_t BrotliEncoderSetParameter(void* a, int32_t b, uint32_t c);
    IL2CPP_EXPORT int32_t BrotliEncoderCompressStream(void* a, int32_t b, intptr_t* c, uint8_t** d, intptr_t* e, uint8_t** f, intptr_t* g);
    IL2CPP_EXPORT int32_t BrotliEncoderHasMoreOutput(void* a);
    IL2CPP_EXPORT void BrotliEncoderDestroyInstance(intptr_t a);
    IL2CPP_EXPORT int32_t BrotliEncoderCompress(int32_t a, int32_t b, int32_t c, intptr_t d, uint8_t* e, intptr_t* f, uint8_t* g);
}

// Items needed by System.Core

void SystemNative_GetDomainSocketSizes(int32_t* a, int32_t* b, int32_t* c)
{
    // This PAL call should be implemented when .NET Standard 2.1 is supported, as
    // it is used by the UnixDomainSocketEndPoint class.
    IL2CPP_NOT_IMPLEMENTED(SystemNative_GetDomainSocketSizes);
}

// Items needed by System.IO.Compression

// These PAL calls are all for Brotli, which first appeared in .NET Standard 2.1
// We should implement them when .NET Standard 2.1 support is added in IL2CPP.

void* BrotliDecoderCreateInstance(intptr_t a, intptr_t b, intptr_t c)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliDecoderCreateInstance);
    return NULL;
}

int32_t BrotliDecoderDecompressStream(void* a, intptr_t* b, uint8_t** c, intptr_t* d, uint8_t** e, intptr_t* f)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliDecoderDecompressStream);
    return 0;
}

int32_t BrotliDecoderDecompress(intptr_t a, uint8_t* b, intptr_t* c, uint8_t* d)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliDecoderDecompress);
    return 0;
}

void BrotliDecoderDestroyInstance(intptr_t a)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliDecoderDestroyInstance);
}

int32_t BrotliDecoderIsFinished(void* a)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliDecoderIsFinished);
    return 0;
}

void* BrotliEncoderCreateInstance(intptr_t a, intptr_t b, intptr_t c)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderCreateInstance);
    return NULL;
}

int32_t BrotliEncoderSetParameter(void* a, int32_t b, uint32_t c)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderSetParameter);
    return 0;
}

int32_t BrotliEncoderCompressStream(void* a, int32_t b, intptr_t* c, uint8_t** d, intptr_t* e, uint8_t** f, intptr_t* g)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderCompressStream);
    return 0;
}

int32_t BrotliEncoderHasMoreOutput(void* a)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderHasMoreOutput);
    return 0;
}

void BrotliEncoderDestroyInstance(intptr_t a)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderDestroyInstance);
}

int32_t BrotliEncoderCompress(int32_t a, int32_t b, int32_t c, intptr_t d, uint8_t* e, intptr_t* f, uint8_t* g)
{
    IL2CPP_NOT_IMPLEMENTED(BrotliEncoderCompress);
    return 0;
}

#endif
