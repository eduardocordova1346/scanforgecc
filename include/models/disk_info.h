#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct DiskPartition
{
    std::string mountPoint;   // "C:\" no Windows, "/" no Linux
    std::string fileSystem;   // "NTFS", "ext4", etc.
    uint64_t totalBytes = 0;
    uint64_t usedBytes = 0;
    uint64_t freeBytes = 0;
    double usagePercent = 0.0;
};

struct DiskInfo
{
    std::vector<DiskPartition> partitions;
};
