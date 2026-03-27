#include "services/system_info_service.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <fstream>
    #include <sstream>
#endif

SystemInfo SystemInfoService::fetch()
{
    SystemInfo info;

#ifdef _WIN32
    // Hostname
    char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(hostname);
    if (GetComputerNameA(hostname, &size))
        info.hostname = hostname;

    // SO e versão via RtlGetVersion (não deprecada como GetVersionEx)
    info.osName = "Windows";

    OSVERSIONINFOEXA osvi{};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    // RtlGetVersion retorna a versão real, sem manifestação
    using RtlGetVersionPtr = LONG(WINAPI*)(POSVERSIONINFOEXA);
    auto RtlGetVersion = reinterpret_cast<RtlGetVersionPtr>(
        GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion"));
    if (RtlGetVersion && RtlGetVersion(&osvi) == 0) {
        info.osVersion = std::to_string(osvi.dwMajorVersion) + "." +
                         std::to_string(osvi.dwMinorVersion) + " (Build " +
                         std::to_string(osvi.dwBuildNumber) + ")";
    }

    // Arquitetura
    SYSTEM_INFO sysInfo{};
    GetNativeSystemInfo(&sysInfo);
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: info.architecture = "x86_64"; break;
        case PROCESSOR_ARCHITECTURE_ARM64: info.architecture = "ARM64"; break;
        case PROCESSOR_ARCHITECTURE_INTEL: info.architecture = "x86"; break;
        default: info.architecture = "Desconhecida"; break;
    }

    // Uptime
    info.uptimeSeconds = static_cast<long>(GetTickCount64() / 1000);

#else
    // Linux: uname para kernel e arquitetura
    struct utsname buf{};
    if (uname(&buf) == 0) {
        info.architecture = buf.machine;
        info.hostname = buf.nodename;
    }

    // Nome e versão do SO via /etc/os-release
    std::ifstream osRelease("/etc/os-release");
    std::string line;
    while (std::getline(osRelease, line)) {
        if (line.rfind("PRETTY_NAME=", 0) == 0) {
            info.osName = line.substr(13);
            // Remove aspas
            if (!info.osName.empty() && info.osName.front() == '"')
                info.osName = info.osName.substr(1, info.osName.size() - 2);
        }
        if (line.rfind("VERSION_ID=", 0) == 0) {
            info.osVersion = line.substr(11);
            if (!info.osVersion.empty() && info.osVersion.front() == '"')
                info.osVersion = info.osVersion.substr(1, info.osVersion.size() - 2);
        }
    }

    // Uptime via /proc/uptime
    std::ifstream uptimeFile("/proc/uptime");
    double uptimeSec = 0;
    if (uptimeFile >> uptimeSec)
        info.uptimeSeconds = static_cast<long>(uptimeSec);
#endif

    return info;
}
