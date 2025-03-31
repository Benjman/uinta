#include "uinta/app_config_yaml.h"

#include <gtest/gtest.h>

#include <string>

#include "uinta/args.h"
#include "uinta/mock/mock_file_system.h"

namespace uinta {

class AppConfigYamlTest : public testing::Test {
 protected:
  MockFileSystem mockFs;
  ArgsProcessor args = ArgsProcessor(0, nullptr);

  void SetUp() override {
    mockFs.onReadAll = []() {
      return R"(
top:
  sub_a:
    int_val_a: 4
    str_val_a: abc
    str_val_b: def
  sub_b:
    int_val_a: 7
    str_val_a: ghi
    str_val_b: jkl
      )";
    };
    mockFs.onIsDirectory = [](std::string) noexcept { return false; };
  }
};

TEST_F(AppConfigYamlTest, InitialState) {
  AppConfigYamlImpl appConfig(&args, &mockFs);

  ASSERT_EQ(4, appConfig.getInt("top.sub_a.int_val_a"));
  ASSERT_STREQ("abc", appConfig.getString("top.sub_a.str_val_a")->c_str());
  ASSERT_STREQ("def", appConfig.getString("top.sub_a.str_val_b")->c_str());

  ASSERT_EQ(7, appConfig.getInt("top.sub_b.int_val_a"));
  ASSERT_STREQ("ghi", appConfig.getString("top.sub_b.str_val_a")->c_str());
  ASSERT_STREQ("jkl", appConfig.getString("top.sub_b.str_val_b")->c_str());
}

}  // namespace uinta
