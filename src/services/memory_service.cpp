#include "services/memory_service.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <fstream>
    #include <string>
#endif

MemoryInfo MemoryService::fetch()
{
    MemoryInfo info;

#ifdef _WIN32
    MEMORYSTATUSEX memStatus{};
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        info.totalBytes = memStatus.ullTotalPhys;
        info.availableBytes = memStatus.ullAvailPhys;
        info.usedBytes = info.totalBytes - info.availableBytes;
        info.usagePercent = static_cast<double>(memStatus.dwMemoryLoad);
    }

#else
    // Linux: /proc/meminfo
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    uint64_t totalKb = 0, availableKb = 0, freeKb = 0, buffersKb = 0, cachedKb = 0;

    while (std::getline(meminfo, line)) {
        if (line.rfind("MemTotal:", 0) == 0)
            totalKb = std::stoull(line.substr(line.find(':') + 1));
        else if (line.rfind("MemAvailable:", 0) == 0)
            availableKb = std::stoull(line.substr(line.find(':') + 1));
        else if (line.rfind("MemFree:", 0) == 0)
            freeKb = std::stoull(line.substr(line.find(':') + 1));
        else if (line.rfind("Buffers:", 0) == 0)
            buffersKb = std::stoull(line.substr(line.find(':') + 1));
        else if (line.rfind("Cached:", 0) == 0 && line.rfind("SwapCached:", 0) != 0)
            cachedKb = std::stoull(line.substr(line.find(':') + 1));
    }

    info.totalBytes = totalKb * 1024;
    // MemAvailable é mais preciso; fallback para Free+Buffers+Cached
    if (availableKb > 0)
        info.availableBytes = availableKb * 1024;
    else
        info.availableBytes = (freeKb + buffersKb + cachedKb) * 1024;
    info.usedBytes = info.totalBytes - info.availableBytes;
    if (info.totalBytes > 0)
        info.usagePercent = static_cast<double>(info.usedBytes) * 100.0 / info.totalBytes;
#endif

    return info;
}
