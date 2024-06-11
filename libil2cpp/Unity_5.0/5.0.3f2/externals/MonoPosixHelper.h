#pragma once

#include <stdint.h>

#include "il2cpp-config.h"
#include "object-internals.h"

extern "C"
{
  struct ZStream;
	
	IL2CPP_EXPORT Il2CppIntPtr CreateZStream(int32_t compress, uint8_t gzip, Il2CppIntPtr func, Il2CppIntPtr gchandle);
	IL2CPP_EXPORT int32_t CloseZStream(Il2CppIntPtr zstream);
	IL2CPP_EXPORT int32_t Flush(Il2CppIntPtr zstream);
	IL2CPP_EXPORT int32_t ReadZStream(Il2CppIntPtr zstream, Il2CppIntPtr buffer, int32_t length);
	IL2CPP_EXPORT int32_t WriteZStream(Il2CppIntPtr zstream, Il2CppIntPtr buffer, int32_t length);
}
