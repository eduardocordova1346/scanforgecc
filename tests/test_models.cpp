#include <gtest/gtest.h>
#include "models/cpu_info.h"
#include "models/memory_info.h"
#include "models/system_info.h"
#include "models/temperature_info.h"
#include "models/disk_info.h"
#include "models/process_info.h"

TEST(ModelsTest, CpuInfo_DefaultValues)
{
    CpuInfo info;
    EXPECT_EQ(info.coreCount, 0);
    EXPECT_EQ(info.threadCount, 0);
    EXPECT_DOUBLE_EQ(info.usagePercent, 0.0);
    EXPECT_DOUBLE_EQ(info.frequencyMhz, 0.0);
    EXPECT_TRUE(info.modelName.empty());
}

TEST(ModelsTest, MemoryInfo_DefaultValues)
{
    MemoryInfo info;
    EXPECT_EQ(info.totalBytes, 0u);
    EXPECT_EQ(info.usedBytes, 0u);
    EXPECT_EQ(info.availableBytes, 0u);
    EXPECT_DOUBLE_EQ(info.usagePercent, 0.0);
}

TEST(ModelsTest, SystemInfo_DefaultValues)
{
    SystemInfo info;
    EXPECT_TRUE(info.osName.empty());
    EXPECT_TRUE(info.hostname.empty());
    EXPECT_EQ(info.uptimeSeconds, 0);
}

TEST(ModelsTest, DiskInfo_EmptyPartitions)
{
    DiskInfo info;
    EXPECT_TRUE(info.partitions.empty());
}

TEST(ModelsTest, DiskPartition_DefaultValues)
{
    DiskPartition part;
    EXPECT_EQ(part.totalBytes, 0u);
    EXPECT_EQ(part.freeBytes, 0u);
    EXPECT_DOUBLE_EQ(part.usagePercent, 0.0);
    EXPECT_TRUE(part.mountPoint.empty());
}

TEST(ModelsTest, ProcessList_EmptyEntries)
{
    ProcessList list;
    EXPECT_TRUE(list.entries.empty());
}

TEST(ModelsTest, ProcessEntry_DefaultValues)
{
    ProcessEntry entry;
    EXPECT_EQ(entry.pid, 0u);
    EXPECT_EQ(entry.memoryBytes, 0u);
    EXPECT_DOUBLE_EQ(entry.cpuPercent, 0.0);
    EXPECT_TRUE(entry.name.empty());
}

TEST(ModelsTest, TemperatureInfo_EmptySensors)
{
    TemperatureInfo info;
    EXPECT_TRUE(info.sensors.empty());
}

TEST(ModelsTest, SensorReading_DefaultValues)
{
    SensorReading reading;
    EXPECT_DOUBLE_EQ(reading.temperatureCelsius, 0.0);
    EXPECT_TRUE(reading.label.empty());
}
