#include "platform/platform.h"

#include <windows.h>

extern "C" void platform_time_init(Platform *platform)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    if (platform == 0) {
        return;
    }

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    platform->qpc_frequency = (double)frequency.QuadPart;
    platform->qpc_origin = counter.QuadPart;
    platform->start_time = 0.0;
}

extern "C" double platform_now_seconds(const Platform *platform)
{
    LARGE_INTEGER counter;

    if (platform == 0 || platform->qpc_frequency <= 0.0) {
        return 0.0;
    }

    QueryPerformanceCounter(&counter);
    return (double)(counter.QuadPart - platform->qpc_origin) / platform->qpc_frequency;
}

extern "C" void platform_sleep_seconds(double seconds)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    double target;
    double now;
    double remain;

    if (seconds <= 0.0) {
        return;
    }

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    target = (double)counter.QuadPart / (double)frequency.QuadPart + seconds;

    for (;;) {
        QueryPerformanceCounter(&counter);
        now = (double)counter.QuadPart / (double)frequency.QuadPart;
        remain = target - now;
        if (remain <= 0.0) {
            break;
        }
        if (remain > 0.002) {
            Sleep(1);
        } else {
            /* 最后 2ms 自旋，避免 Sleep 粒度导致帧率抖动。 */
            YieldProcessor();
        }
    }
}
