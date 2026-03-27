#pragma once

#include <cstdint>

struct MemoryInfo
{
    uint64_t totalBytes = 0;
    uint64_t usedBytes = 0;
    uint64_t availableBytes = 0;
    double usagePercent = 0.0;
};
