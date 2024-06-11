#include "il2cpp-config.h"

#if NET_4_0
#if IL2CPP_ENABLE_CPU_INFO
#include "os/CpuInfo.h"
#include "os/c-api/CpuInfo-c-api.h"

extern "C"
{
void* UnityPalCpuInfoCreate()
{
    return il2cpp::os::CpuInfo::Create();
}

int32_t UnityPalCpuInfoUsage(void* previous)
{
    return il2cpp::os::CpuInfo::Usage(previous);
}
}
#endif // IL2CPP_ENABLE_CPU_INFO
#endif // NET_4_0
