#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct ProcessEntry
{
    uint32_t pid = 0;
    std::string name;
    uint64_t memoryBytes = 0;
    double cpuPercent = 0.0;
};

struct ProcessList
{
    std::vector<ProcessEntry> entries;
};
