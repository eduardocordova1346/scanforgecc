#include "services/cpu_service.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <intrin.h>
    #include <array>
#else
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <thread>
    #include <chrono>
#endif

#ifdef _WIN32

// Converte FILETIME para uint64
static uint64_t fileTimeToU64(const FILETIME& ft)
{
    return (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

static std::string getCpuBrand()
{
    std::array<int, 4> cpui{};
    char brand[49]{};

    for (int i = 0; i < 3; ++i) {
        __cpuid(cpui.data(), 0x80000002 + i);
        std::memcpy(brand + i * 16, cpui.data(), 16);
    }
    // Remove espaços iniciais
    std::string result(brand);
    auto start = result.find_first_not_of(' ');
    return (start != std::string::npos) ? result.substr(start) : result;
}

CpuInfo CpuService::fetch()
{
    CpuInfo info;

    // Modelo via CPUID
    info.modelName = getCpuBrand();

    // Núcleos e threads
    SYSTEM_INFO sysInfo{};
    GetNativeSystemInfo(&sysInfo);
    info.threadCount = static_cast<int>(sysInfo.dwNumberOfProcessors);

    // Núcleos físicos via GetLogicalProcessorInformation
    DWORD bufLen = 0;
    GetLogicalProcessorInformation(nullptr, &bufLen);
    std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buf(
        bufLen / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
    if (GetLogicalProcessorInformation(buf.data(), &bufLen)) {
        int cores = 0;
        for (const auto& entry : buf) {
            if (entry.Relationship == RelationProcessorCore)
                ++cores;
        }
        info.coreCount = cores;
    }

    // Uso de CPU: comparação de idle time entre dois instantes
    FILETIME idleA{}, kernelA{}, userA{};
    FILETIME idleB{}, kernelB{}, userB{};
    GetSystemTimes(&idleA, &kernelA, &userA);
    Sleep(250);
    GetSystemTimes(&idleB, &kernelB, &userB);

    uint64_t idle = fileTimeToU64(idleB) - fileTimeToU64(idleA);
    uint64_t kernel = fileTimeToU64(kernelB) - fileTimeToU64(kernelA);
    uint64_t user = fileTimeToU64(userB) - fileTimeToU64(userA);
    uint64_t total = kernel + user;

    if (total > 0)
        info.usagePercent = (1.0 - static_cast<double>(idle) / total) * 100.0;

    // Frequência via registro
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD mhz = 0, size = sizeof(mhz);
        if (RegQueryValueExA(hKey, "~MHz", nullptr, nullptr,
                reinterpret_cast<LPBYTE>(&mhz), &size) == ERROR_SUCCESS) {
            info.frequencyMhz = static_cast<double>(mhz);
        }
        RegCloseKey(hKey);
    }

    return info;
}

#else // Linux

CpuInfo CpuService::fetch()
{
    CpuInfo info;

    // /proc/cpuinfo para modelo, núcleos e frequência
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    int coreCount = 0;
    int threadCount = 0;

    while (std::getline(cpuinfo, line)) {
        if (line.rfind("model name", 0) == 0 && info.modelName.empty()) {
            auto pos = line.find(':');
            if (pos != std::string::npos)
                info.modelName = line.substr(pos + 2);
        }
        if (line.rfind("cpu MHz", 0) == 0 && info.frequencyMhz == 0.0) {
            auto pos = line.find(':');
            if (pos != std::string::npos)
                info.frequencyMhz = std::stod(line.substr(pos + 2));
        }
        if (line.rfind("cpu cores", 0) == 0 && coreCount == 0) {
            auto pos = line.find(':');
            if (pos != std::string::npos)
                coreCount = std::stoi(line.substr(pos + 2));
        }
        if (line.rfind("processor", 0) == 0)
            ++threadCount;
    }
    info.coreCount = (coreCount > 0) ? coreCount : threadCount;
    info.threadCount = threadCount;

    // Uso de CPU via /proc/stat (duas leituras com intervalo)
    auto readCpuTimes = []() -> std::pair<uint64_t, uint64_t> {
        std::ifstream stat("/proc/stat");
        std::string label;
        uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
        stat >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        uint64_t total = user + nice + system + idle + iowait + irq + softirq + steal;
        return {idle + iowait, total};
    };

    auto [idle1, total1] = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    auto [idle2, total2] = readCpuTimes();

    uint64_t idleDelta = idle2 - idle1;
    uint64_t totalDelta = total2 - total1;
    if (totalDelta > 0)
        info.usagePercent = (1.0 - static_cast<double>(idleDelta) / totalDelta) * 100.0;

    return info;
}

#endif
