#include "private.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>

static timestamp qpc_scaled(timestamp per_sec)
{
    LARGE_INTEGER freq, cnt;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    /* split to avoid 64-bit overflow on the multiplication */
    return (cnt.QuadPart / freq.QuadPart) * per_sec
        + (cnt.QuadPart % freq.QuadPart) * per_sec / freq.QuadPart;
}

timestamp kl_monotonic(void) { return qpc_scaled(1000); }
timestamp kl_high_monotonic(void) { return qpc_scaled(1000000); }
#else
timestamp kl_monotonic(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        return 0;
    }
    return (uint64_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

timestamp kl_high_monotonic(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        return 0;
    }
    return (uint64_t)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}
#endif
