#include "services/disk_service.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <sys/statvfs.h>
    #include <set>
#endif

DiskInfo DiskService::fetch()
{
    DiskInfo info;

#ifdef _WIN32
    // Enumera drives lógicos (C:\, D:\, etc.)
    char driveStrings[256]{};
    DWORD len = GetLogicalDriveStringsA(sizeof(driveStrings), driveStrings);

    for (DWORD i = 0; i < len;) {
        const char* drive = &driveStrings[i];
        UINT type = GetDriveTypeA(drive);
        // Só inclui discos fixos e removíveis
        if (type == DRIVE_FIXED || type == DRIVE_REMOVABLE) {
            ULARGE_INTEGER freeBytesAvailable{}, totalBytes{}, freeBytes{};
            if (GetDiskFreeSpaceExA(drive, &freeBytesAvailable, &totalBytes, &freeBytes)) {
                DiskPartition part;
                part.mountPoint = drive;
                part.totalBytes = totalBytes.QuadPart;
                part.freeBytes = freeBytes.QuadPart;
                part.usedBytes = part.totalBytes - part.freeBytes;
                if (part.totalBytes > 0)
                    part.usagePercent = static_cast<double>(part.usedBytes) * 100.0 / part.totalBytes;

                // Tipo do sistema de arquivos
                char fsName[64]{};
                if (GetVolumeInformationA(drive, nullptr, 0, nullptr, nullptr, nullptr, fsName, sizeof(fsName)))
                    part.fileSystem = fsName;

                info.partitions.push_back(std::move(part));
            }
        }
        i += static_cast<DWORD>(strlen(drive) + 1);
    }

#else
    // Linux: lê /proc/mounts para listar partições reais
    std::ifstream mounts("/proc/mounts");
    std::string line;
    std::set<std::string> seen; // evita duplicatas

    while (std::getline(mounts, line)) {
        std::istringstream iss(line);
        std::string device, mountPoint, fsType;
        iss >> device >> mountPoint >> fsType;

        // Ignora sistemas de arquivos virtuais
        if (device.rfind("/dev/", 0) != 0)
            continue;
        if (seen.count(mountPoint))
            continue;
        seen.insert(mountPoint);

        struct statvfs stat{};
        if (statvfs(mountPoint.c_str(), &stat) != 0)
            continue;

        DiskPartition part;
        part.mountPoint = mountPoint;
        part.fileSystem = fsType;
        part.totalBytes = static_cast<uint64_t>(stat.f_blocks) * stat.f_frsize;
        part.freeBytes = static_cast<uint64_t>(stat.f_bavail) * stat.f_frsize;
        part.usedBytes = part.totalBytes - part.freeBytes;
        if (part.totalBytes > 0)
            part.usagePercent = static_cast<double>(part.usedBytes) * 100.0 / part.totalBytes;

        info.partitions.push_back(std::move(part));
    }
#endif

    return info;
}
