#pragma once

#include <cstdint>
#include <string>

namespace Formatter
{
    std::string bytesToHuman(uint64_t bytes);
    std::string secondsToUptime(long totalSeconds);
}
