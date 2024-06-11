#if ENABLE_UNIT_TESTS
#if NET_4_0
#if IL2CPP_ENABLE_CPU_INFO

#include "UnitTest++.h"

#include "../CpuInfo-c-api.h"
#include "../../CpuInfo.h"

SUITE(CpuInfo)
{
    TEST(ApiCreateMatchesClassCreate)
    {
        CHECK_EQUAL(il2cpp::os::CpuInfo::CpuInfoCreate(), UnityPalCpuInfoCreate());
    }

    TEST(ApiCpuInfoUsageMatchesClassCpuInfo)
    {
        CHECK_EQUAL(il2cpp::os::CpuInfo::CpuInfoUsage(), UnityPalCpuInfoUsage());
    }
}

#endif  // IL2CPP_ENABLE_CPU_INFO
#endif  // NET_4_0
#endif // ENABLE_UNIT_TESTS
