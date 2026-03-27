#pragma once

#include <string>

struct CpuInfo
{
    std::string modelName;
    int coreCount = 0;
    int threadCount = 0;
    double usagePercent = 0.0;
    double frequencyMhz = 0.0;
};
