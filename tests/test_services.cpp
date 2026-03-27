#include <gtest/gtest.h>
#include "services/cpu_service.h"
#include "services/memory_service.h"
#include "services/system_info_service.h"
#include "services/disk_service.h"
#include "services/process_service.h"
#include "services/temperature_service.h"

TEST(CpuServiceTest, FetchReturnValidData)
{
    auto info = CpuService::fetch();
    EXPECT_FALSE(info.modelName.empty());
    EXPECT_GT(info.coreCount, 0);
    EXPECT_GT(info.threadCount, 0);
    EXPECT_GE(info.usagePercent, 0.0);
    EXPECT_LE(info.usagePercent, 100.0);
}

TEST(MemoryServiceTest, FetchReturnValidData)
{
    auto info = MemoryService::fetch();
    EXPECT_GT(info.totalBytes, 0u);
    EXPECT_GT(info.usedBytes, 0u);
    EXPECT_GT(info.availableBytes, 0u);
    EXPECT_EQ(info.usedBytes + info.availableBytes, info.totalBytes);
    EXPECT_GE(info.usagePercent, 0.0);
    EXPECT_LE(info.usagePercent, 100.0);
}

TEST(SystemInfoServiceTest, FetchReturnValidData)
{
    auto info = SystemInfoService::fetch();
    EXPECT_FALSE(info.osName.empty());
    EXPECT_FALSE(info.hostname.empty());
    EXPECT_FALSE(info.architecture.empty());
    EXPECT_GT(info.uptimeSeconds, 0);
}

TEST(DiskServiceTest, FetchReturnAtLeastOnePartition)
{
    auto info = DiskService::fetch();
    EXPECT_FALSE(info.partitions.empty());

    for (const auto& part : info.partitions) {
        EXPECT_GT(part.totalBytes, 0u);
        EXPECT_GE(part.usagePercent, 0.0);
        EXPECT_LE(part.usagePercent, 100.0);
        EXPECT_FALSE(part.mountPoint.empty());
    }
}

TEST(ProcessServiceTest, FetchReturnProcesses)
{
    auto list = ProcessService::fetch();
    EXPECT_FALSE(list.entries.empty());

    for (const auto& proc : list.entries) {
        EXPECT_GT(proc.pid, 0u);
        EXPECT_FALSE(proc.name.empty());
    }
}

TEST(ProcessServiceTest, FetchReturnSortedByMemory)
{
    auto list = ProcessService::fetch();
    for (size_t i = 1; i < list.entries.size(); ++i) {
        EXPECT_GE(list.entries[i - 1].memoryBytes, list.entries[i].memoryBytes);
    }
}

TEST(TemperatureServiceTest, FetchReturnSensors)
{
    auto info = TemperatureService::fetch();
    EXPECT_FALSE(info.sensors.empty());
}
