#include "services/temperature_service.h"

#ifdef _WIN32
    // Sem implementação nativa viável no Windows
#else
    #include <filesystem>
    #include <fstream>
    #include <string>
#endif

TemperatureInfo TemperatureService::fetch()
{
    TemperatureInfo info;

#ifdef _WIN32
    // Placeholder: temperatura não disponível nativamente no Windows
    // sem bibliotecas externas. Retorna lista vazia com flag de indisponibilidade.
    info.sensors.push_back({"Status", -1.0}); // -1 = indisponível

#else
    namespace fs = std::filesystem;

    // Tenta /sys/class/thermal/thermal_zone*
    if (fs::exists("/sys/class/thermal")) {
        for (const auto& entry : fs::directory_iterator("/sys/class/thermal")) {
            std::string name = entry.path().filename().string();
            if (name.rfind("thermal_zone", 0) != 0)
                continue;

            std::string typePath = entry.path().string() + "/type";
            std::string tempPath = entry.path().string() + "/temp";

            std::string label = name;
            std::ifstream typeFile(typePath);
            if (typeFile)
                std::getline(typeFile, label);

            std::ifstream tempFile(tempPath);
            int milliCelsius = 0;
            if (tempFile >> milliCelsius) {
                double celsius = milliCelsius / 1000.0;
                info.sensors.push_back({label, celsius});
            }
        }
    }

    // Tenta /sys/class/hwmon para sensores adicionais
    if (fs::exists("/sys/class/hwmon")) {
        for (const auto& hwmon : fs::directory_iterator("/sys/class/hwmon")) {
            std::string basePath = hwmon.path().string();

            // Lê nome do sensor
            std::string hwmonName;
            std::ifstream nameFile(basePath + "/name");
            if (nameFile)
                std::getline(nameFile, hwmonName);

            // Procura temp*_input
            for (const auto& file : fs::directory_iterator(basePath)) {
                std::string fname = file.path().filename().string();
                if (fname.find("temp") == 0 && fname.find("_input") != std::string::npos) {
                    std::ifstream tempFile(file.path());
                    int milliCelsius = 0;
                    if (tempFile >> milliCelsius) {
                        // Tenta ler o label correspondente
                        std::string labelPath = basePath + "/" +
                            fname.substr(0, fname.find("_input")) + "_label";
                        std::string label = hwmonName + " " + fname;
                        std::ifstream labelFile(labelPath);
                        if (labelFile)
                            std::getline(labelFile, label);

                        info.sensors.push_back({label, milliCelsius / 1000.0});
                    }
                }
            }
        }
    }

    // Se nenhum sensor encontrado
    if (info.sensors.empty())
        info.sensors.push_back({"Status", -1.0});
#endif

    return info;
}
