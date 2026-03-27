#include "services/process_service.h"

#include <algorithm>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <tlhelp32.h>
    #include <psapi.h>
#else
    #include <dirent.h>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <cctype>
#endif

ProcessList ProcessService::fetch()
{
    ProcessList list;

#ifdef _WIN32
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return list;

    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(entry);

    if (Process32First(snapshot, &entry)) {
        do {
            ProcessEntry proc;
            proc.pid = entry.th32ProcessID;
            proc.name = entry.szExeFile;

            // Tenta obter uso de memória do processo
            HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc.pid);
            if (hProc) {
                PROCESS_MEMORY_COUNTERS pmc{};
                if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)))
                    proc.memoryBytes = pmc.WorkingSetSize;
                CloseHandle(hProc);
            }

            list.entries.push_back(std::move(proc));
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);

#else
    // Linux: lê /proc/[pid]/stat e /proc/[pid]/status
    DIR* procDir = opendir("/proc");
    if (!procDir)
        return list;

    struct dirent* dirEntry;
    while ((dirEntry = readdir(procDir)) != nullptr) {
        // Só diretórios numéricos (PIDs)
        if (dirEntry->d_type != DT_DIR)
            continue;
        const std::string pidStr = dirEntry->d_name;
        if (!std::all_of(pidStr.begin(), pidStr.end(), ::isdigit))
            continue;

        ProcessEntry proc;
        proc.pid = static_cast<uint32_t>(std::stoul(pidStr));

        // Nome do processo via /proc/[pid]/comm
        std::ifstream commFile("/proc/" + pidStr + "/comm");
        if (commFile)
            std::getline(commFile, proc.name);

        // Memória via /proc/[pid]/status (VmRSS)
        std::ifstream statusFile("/proc/" + pidStr + "/status");
        std::string line;
        while (std::getline(statusFile, line)) {
            if (line.rfind("VmRSS:", 0) == 0) {
                std::istringstream iss(line.substr(6));
                uint64_t rssKb;
                if (iss >> rssKb)
                    proc.memoryBytes = rssKb * 1024;
                break;
            }
        }

        list.entries.push_back(std::move(proc));
    }
    closedir(procDir);
#endif

    // Ordena por uso de memória (decrescente)
    std::sort(list.entries.begin(), list.entries.end(),
        [](const ProcessEntry& a, const ProcessEntry& b) {
            return a.memoryBytes > b.memoryBytes;
        });

    return list;
}
