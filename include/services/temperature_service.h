#pragma once

#include "models/temperature_info.h"

class TemperatureService
{
public:
    static TemperatureInfo fetch();
};
