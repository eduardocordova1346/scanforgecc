# ScanForge CC

Ferramenta desktop de diagnóstico de sistema para técnicos de informática.
Coleta dados reais do hardware e SO, com suporte multiplataforma (Windows + Linux).

## Stack

- C++20
- Qt 6 Widgets
- CMake 3.20+
- vcpkg (opcional)

## Módulos

| Módulo | Dados coletados | Windows | Linux |
|---|---|---|---|
| Sistema | SO, versão, hostname, arquitetura, uptime | Win32 API | /etc/os-release, uname |
| CPU | Modelo, núcleos, threads, frequência, uso % | GetSystemTimes, CPUID | /proc/cpuinfo, /proc/stat |
| RAM | Total, usado, disponível, uso % | GlobalMemoryStatusEx | /proc/meminfo |
| Disco | Partições, espaço total/usado/livre | GetDiskFreeSpaceEx | statvfs, /proc/mounts |
| Processos | PID, nome, memória, CPU % | Toolhelp32, PSAPI | /proc/[pid]/* |
| Temperatura | Sensores de temp (CPU, GPU) | Placeholder* | /sys/class/thermal, hwmon |

\* No Windows, temperatura requer integração com LibreHardwareMonitor ou WMI — preparado como ponto de extensão.

## Build

### Pré-requisitos

- Compilador com suporte a C++20 (GCC 11+, Clang 14+, MSVC 2022+)
- Qt 6 instalado
- CMake 3.20+

### Windows (MSVC + Qt)

```powershell
cmake -B build -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2022_64
cmake --build build --config Release
.\build\Release\ScanForgeCC.exe
```

### Linux

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/caminho/para/qt6
cmake --build build
./build/ScanForgeCC
```

### Com vcpkg

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Estrutura do projeto

```
scanforgecc/
├── CMakeLists.txt
├── include/
│   ├── mainwindow.h
│   ├── models/           # DTOs (CpuInfo, MemoryInfo, DiskInfo, etc.)
│   ├── services/         # Coleta de dados reais do SO (multiplataforma)
│   ├── widgets/          # Componentes visuais reutilizáveis
│   └── utils/            # Helpers genéricos (Formatter)
├── src/
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── models/
│   ├── services/         # Implementações com #ifdef _WIN32 / Linux
│   ├── widgets/
│   └── utils/
├── ui/                   # Reservado para .ui do Qt Designer
└── tests/                # Reservado para testes unitários
```

## Arquitetura

```
MainWindow (abas: Visão Geral | Disco | Processos)
    ├── CpuWidget       → CpuService::fetch()       → CpuInfo
    ├── MemoryWidget     → MemoryService::fetch()    → MemoryInfo
    ├── SystemWidget     → SystemInfoService::fetch() → SystemInfo
    ├── TemperatureWidget→ TemperatureService::fetch()→ TemperatureInfo
    ├── DiskWidget       → DiskService::fetch()      → DiskInfo
    └── ProcessWidget    → ProcessService::fetch()   → ProcessList
```

- **Models**: structs que transportam dados entre camadas
- **Services**: coleta real de dados do SO via APIs nativas (`#ifdef _WIN32`)
- **Widgets**: componentes visuais com barras de progresso, herdam de `InfoCardWidget`
- **MainWindow**: orquestra widgets, abas e timer de atualização (2s)
