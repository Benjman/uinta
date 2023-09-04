#include <gtest/gtest.h>

#include <uinta/logging.hpp>
#include <uinta/utils/formatters.hpp>

using namespace uinta;

TEST(formatters_hpp, removeTrailingZeroes) {
  ASSERT_STREQ("1.02", removeTrailingZeroes("1.020").data()) << "Single trailing zero was expected to have been removed!";
  ASSERT_STREQ("1.02", removeTrailingZeroes("1.0200").data()) << "Multiple trailing zeroes were expected to have been removed!";
  ASSERT_STREQ("1.02", removeTrailingZeroes("1.0200000").data()) << "Lots of trailing zeroes were expected to have been removed!";
  ASSERT_STREQ("01.02", removeTrailingZeroes("01.02").data()) << "Why did we remove **leading** zeroes?!";
}

TEST(formatters_hpp, formatMemory) {
  ASSERT_STREQ("1 byte", formatMemory(1).data()) << "A single byte should not be pluralized!";
  ASSERT_STREQ("2 bytes", formatMemory(2).data()) << "More than one byte should be pliralized!";
  ASSERT_STREQ("30 bytes", formatMemory(30).data()) << "Double digit formats are expected!";
  ASSERT_STREQ("400 bytes", formatMemory(400).data()) << "Triple digit formats are expected!";
  ASSERT_STREQ("1023 bytes", formatMemory(1023).data()) << "Upper bound byte formatting unexpected!";

  ASSERT_STREQ("1 KB", formatMemory(KILOBYTES(1)).data()) << "1024 exactly translates to KB";
  ASSERT_STREQ("1 KB", formatMemory(KILOBYTES(1) * 1.001).data()) << "Rounding 2 decimal places expects to ignore extra byte.";
  ASSERT_STREQ("1.01 KB", formatMemory(KILOBYTES(1) * 1.01).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.09 KB", formatMemory(KILOBYTES(1) * 1.09).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.11 KB", formatMemory(KILOBYTES(1) * 1.11).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.15 KB", formatMemory(KILOBYTES(1) * 1.15).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.19 KB", formatMemory(KILOBYTES(1) * 1.191).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.2 KB", formatMemory(KILOBYTES(1) * 1.199).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.9 KB", formatMemory(KILOBYTES(1) * 1.9).data()) << "Rounding 2 decimal places expects to ignore extra 9 bytes!";
  ASSERT_STREQ("1.99 KB", formatMemory(KILOBYTES(1) * 1.99).data())
      << "Rounding 2 decimal places expects to ignore extra 9 bytes!";

  ASSERT_STREQ("1 MB", formatMemory(MEGABYTES(1)).data()) << "1024^2 exactly translates to MB";
  ASSERT_STREQ("1 MB", formatMemory(MEGABYTES(1) * 1.001).data()) << "Rounding 2 decimal places expects to ignore extra byte.";
  ASSERT_STREQ("1.01 MB", formatMemory(MEGABYTES(1) * 1.01).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.09 MB", formatMemory(MEGABYTES(1) * 1.09).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.11 MB", formatMemory(MEGABYTES(1) * 1.11).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.15 MB", formatMemory(MEGABYTES(1) * 1.15).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.19 MB", formatMemory(MEGABYTES(1) * 1.191).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.2 MB", formatMemory(MEGABYTES(1) * 1.199).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.9 MB", formatMemory(MEGABYTES(1) * 1.9).data()) << "Rounding 2 decimal places expects to ignore extra 9 bytes!";
  ASSERT_STREQ("1.99 MB", formatMemory(MEGABYTES(1) * 1.99).data())
      << "Rounding 2 decimal places expects to ignore extra 9 bytes!";

  ASSERT_STREQ("1 GB", formatMemory(GIGABYTES(1)).data()) << "1024^3 exactly translates to GB";
  ASSERT_STREQ("1 GB", formatMemory(GIGABYTES(1) * 1.001).data()) << "Rounding 2 decimal places expects to ignore extra byte.";
  ASSERT_STREQ("1.01 GB", formatMemory(GIGABYTES(1) * 1.01).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.09 GB", formatMemory(GIGABYTES(1) * 1.09).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.11 GB", formatMemory(GIGABYTES(1) * 1.11).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.15 GB", formatMemory(GIGABYTES(1) * 1.15).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.19 GB", formatMemory(GIGABYTES(1) * 1.191).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.2 GB", formatMemory(GIGABYTES(1) * 1.199).data()) << "Rounding 2 decimal places expected!";
  ASSERT_STREQ("1.9 GB", formatMemory(GIGABYTES(1) * 1.9).data()) << "Rounding 2 decimal places expects to ignore extra 9 bytes!";
  ASSERT_STREQ("1.99 GB", formatMemory(GIGABYTES(1) * 1.99).data())
      << "Rounding 2 decimal places expects to ignore extra 9 bytes!";
}
