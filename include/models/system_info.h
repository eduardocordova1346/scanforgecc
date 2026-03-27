#pragma once

#include <string>

struct SystemInfo
{
    std::string osName;
    std::string osVersion;
    std::string hostname;
    std::string architecture;
    long uptimeSeconds = 0;
};
