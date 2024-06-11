#include "il2cpp-config.h"

#if IL2CPP_TARGET_ANDROID
#include <cmath>

/*  CLOCK_MONOTONIC is the most reliable clock type on Android. However, it
    does not tick when the device is sleeping (case 867885, case 1037712).
    CLOCK_BOOTTIME includes that time, but is very unreliable. Some older
 
    devices had this time ticking back or jumping back and forth (case 970945)
    To fix this issue we combine both clocks to produce a CLOCK_MONOTONIC-based
    clock that ticks even when the device is disabled.
    */
    class TimeSinceStartupMonotonicBoottimeClockCombiner
    {
    public:
        TimeSinceStartupMonotonicBoottimeClockCombiner() :
                m_MonotonicStartTime(-HUGE_VAL),
                m_BoottimeStartTime(-HUGE_VAL),
                m_BoottimeAdjustment(0),
                m_BrokenBoottime(false),
                m_BrokenBoottimeDetectionHysteresis(0.001),
                m_AdjustmentHysteresisWhenBootimeGood(0.001),
                m_AdjustmentHysteresisWhenBootimeBroken(8)
        {
        }
        int64_t GetTimeSinceStartup(double currentMonotonicTime, double currentBoottimeTime)
        {
            if (m_MonotonicStartTime == -HUGE_VAL)
                m_MonotonicStartTime = currentMonotonicTime;
            if (m_BoottimeStartTime == -HUGE_VAL)
                m_BoottimeStartTime = currentBoottimeTime;
            double monotonicSinceStart = currentMonotonicTime - m_MonotonicStartTime;
            double boottimeSinceStart = currentBoottimeTime - m_BoottimeStartTime;
            /*  In theory, boottime can only go faster than monotonic, so whenever we detect
                this condition we assume that device was asleep and we must adjust the returned
                time by the amount of time that the boottime jumped forwards.
                In the real world, boottime can go slower than monotonic or even backwards.
                We work around this by only taking into account the total difference between
                boottime and monotonic times and only adjusting monotonic time when this difference
                increases.
                There's also a problem that on some devices the boottime continuously jumps
                forwards and backwards by ~4 seconds. This means that a naive implementation would
                often do more than one time jump after device sleeps, depending on which part
                of the jump "cycle" we landed. We work around this by introducing hysteresis of
                hysteresisSeconds seconds and adjusting monotonic time only when this adjustment
                changes by more than hysteresisSeconds amount, but only on broken devices.
                On devices with broken CLOCK_BOOTTIME behaviour this would ignore device sleeps of
                hysteresisSeconds or less, which is small compromise to make.
            */
            if (boottimeSinceStart - monotonicSinceStart < -m_BrokenBoottimeDetectionHysteresis)
                m_BrokenBoottime = true;
            double hysteresisSeconds = m_BrokenBoottime ? m_AdjustmentHysteresisWhenBootimeBroken : m_AdjustmentHysteresisWhenBootimeGood;
            if (boottimeSinceStart - monotonicSinceStart > m_BoottimeAdjustment + hysteresisSeconds)
                m_BoottimeAdjustment = boottimeSinceStart - monotonicSinceStart;
            return (int64_t)(monotonicSinceStart + m_BoottimeAdjustment);
        }

    private:
        double m_MonotonicStartTime;
        double m_BoottimeStartTime;
        double m_BoottimeAdjustment;
        bool m_BrokenBoottime;
        double m_BrokenBoottimeDetectionHysteresis;
        double m_AdjustmentHysteresisWhenBootimeGood;
        double m_AdjustmentHysteresisWhenBootimeBroken;
    };
#endif
