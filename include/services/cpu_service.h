#pragma once

#include "models/cpu_info.h"

class CpuService
{
public:
    static CpuInfo fetch();
};
