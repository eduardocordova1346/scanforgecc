#pragma once

#include <string>
#include <vector>

struct SensorReading
{
    std::string label;
    double temperatureCelsius = 0.0;
};

struct TemperatureInfo
{
    std::vector<SensorReading> sensors;
};
