#include "utils/formatter.h"

#include <sstream>
#include <iomanip>

namespace Formatter
{

std::string bytesToHuman(uint64_t bytes)
{
    constexpr double KB = 1024.0;
    constexpr double MB = KB * 1024;
    constexpr double GB = MB * 1024;
    constexpr double TB = GB * 1024;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);

    if (bytes >= static_cast<uint64_t>(TB))
        oss << (bytes / TB) << " TB";
    else if (bytes >= static_cast<uint64_t>(GB))
        oss << (bytes / GB) << " GB";
    else if (bytes >= static_cast<uint64_t>(MB))
        oss << (bytes / MB) << " MB";
    else if (bytes >= static_cast<uint64_t>(KB))
        oss << (bytes / KB) << " KB";
    else
        oss << bytes << " B";

    return oss.str();
}

std::string secondsToUptime(long totalSeconds)
{
    long days = totalSeconds / 86400;
    long hours = (totalSeconds % 86400) / 3600;
    long minutes = (totalSeconds % 3600) / 60;

    std::ostringstream oss;
    if (days > 0) oss << days << "d ";
    if (hours > 0 || days > 0) oss << hours << "h ";
    oss << minutes << "m";

    return oss.str();
}

} // namespace Formatter
