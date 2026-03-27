#include <gtest/gtest.h>
#include "utils/formatter.h"

TEST(FormatterTest, BytesToHuman_Bytes)
{
    EXPECT_EQ(Formatter::bytesToHuman(0), "0 B");
    EXPECT_EQ(Formatter::bytesToHuman(512), "512 B");
    EXPECT_EQ(Formatter::bytesToHuman(1023), "1023 B");
}

TEST(FormatterTest, BytesToHuman_Kilobytes)
{
    EXPECT_EQ(Formatter::bytesToHuman(1024), "1.0 KB");
    EXPECT_EQ(Formatter::bytesToHuman(1536), "1.5 KB");
}

TEST(FormatterTest, BytesToHuman_Megabytes)
{
    EXPECT_EQ(Formatter::bytesToHuman(1048576), "1.0 MB");
    EXPECT_EQ(Formatter::bytesToHuman(1572864), "1.5 MB");
}

TEST(FormatterTest, BytesToHuman_Gigabytes)
{
    uint64_t gb = 1024ULL * 1024 * 1024;
    EXPECT_EQ(Formatter::bytesToHuman(gb), "1.0 GB");
    EXPECT_EQ(Formatter::bytesToHuman(16 * gb), "16.0 GB");
}

TEST(FormatterTest, BytesToHuman_Terabytes)
{
    uint64_t tb = 1024ULL * 1024 * 1024 * 1024;
    EXPECT_EQ(Formatter::bytesToHuman(tb), "1.0 TB");
}

TEST(FormatterTest, SecondsToUptime_MinutesOnly)
{
    EXPECT_EQ(Formatter::secondsToUptime(0), "0m");
    EXPECT_EQ(Formatter::secondsToUptime(60), "1m");
    EXPECT_EQ(Formatter::secondsToUptime(300), "5m");
}

TEST(FormatterTest, SecondsToUptime_Hours)
{
    EXPECT_EQ(Formatter::secondsToUptime(3600), "1h 0m");
    EXPECT_EQ(Formatter::secondsToUptime(3660), "1h 1m");
    EXPECT_EQ(Formatter::secondsToUptime(7200), "2h 0m");
}

TEST(FormatterTest, SecondsToUptime_Days)
{
    EXPECT_EQ(Formatter::secondsToUptime(86400), "1d 0h 0m");
    EXPECT_EQ(Formatter::secondsToUptime(90061), "1d 1h 1m");
}
