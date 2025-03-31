#include "uinta/app_config_yaml.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "uinta/args.h"

namespace uinta {

class AppConfigYamlTest : public testing::Test {
 protected:
  std::filesystem::path tempFile;
  std::string argString;

  void SetUp() override {
    // Create a temporary YAML file for testing
    tempFile = std::filesystem::temp_directory_path() / "test_config.yml";
    std::ofstream out(tempFile);
    out << R"(
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
    out.close();

    // Store the arg string to keep it alive
    argString = "--opt=" + tempFile.string();
  }

  void TearDown() override {
    // Clean up the temporary file
    if (std::filesystem::exists(tempFile)) {
      std::filesystem::remove(tempFile);
    }
  }
};

TEST_F(AppConfigYamlTest, InitialState) {
  const char* argv[] = {"test", argString.c_str()};
  ArgsProcessor args(2, argv);
  AppConfigYamlImpl appConfig(&args);

  ASSERT_EQ(4, appConfig.getInt("top.sub_a.int_val_a"));
  ASSERT_STREQ("abc", appConfig.getString("top.sub_a.str_val_a")->c_str());
  ASSERT_STREQ("def", appConfig.getString("top.sub_a.str_val_b")->c_str());

  ASSERT_EQ(7, appConfig.getInt("top.sub_b.int_val_a"));
  ASSERT_STREQ("ghi", appConfig.getString("top.sub_b.str_val_a")->c_str());
  ASSERT_STREQ("jkl", appConfig.getString("top.sub_b.str_val_b")->c_str());
}

}  // namespace uinta
