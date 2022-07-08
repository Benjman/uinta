// This file is included in /uinta/test/resource.cpp, so syntax errors in this file will work when compiling

#include <gtest/gtest.h>

#define private public
#include <uinta/resource.hpp>
#include <uinta/cfg.hpp>

using namespace uinta;
using namespace uinta::resource;

TEST(resources, parsePaths) {
  const char *const input = "/my/user/path;/second/user/path";
  std::vector<std::string> buffer;
  internal::parsePaths(input, buffer, logger);
  ASSERT_EQ(2, buffer.size());
  ASSERT_EQ("/my/user/path", buffer.at(0));
  ASSERT_EQ("/second/user/path", buffer.at(1));
}

TEST(resources, parsePaths_singlePath) {
  const char *const input = "/my/user/path";
  std::vector<std::string> buffer;
  internal::parsePaths(input, buffer, logger);
  ASSERT_EQ(1, buffer.size());
  ASSERT_EQ("/my/user/path", buffer.at(0));
}

TEST(resources, semicolon_only) {
  const char *const input = ";";
  std::vector<std::string> buffer;
  internal::parsePaths(input, buffer, logger);
  ASSERT_EQ(0, buffer.size());
}

TEST(resources, buffer_clears) {
  const char *const input = ";";
  std::vector<std::string> buffer;
  buffer.emplace_back("Hello");
  internal::parsePaths(input, buffer, logger);
  ASSERT_EQ(0, buffer.size());
}

TEST(resources, parsePaths_manyPaths) {
  int pathCount = 100;
  std::string input;
  for (int i = 0; i < pathCount; i++) {
    input += "/path/to/folder/" + std::to_string(i);
    if (i + 1 != pathCount)
      input += ";";
  }
  std::vector<std::string> buffer;
  internal::parsePaths(input.c_str(), buffer, logger);
  ASSERT_EQ(pathCount, buffer.size());
  for (int i = 0; i < pathCount; i++)
    ASSERT_EQ(std::string("/path/to/folder/" + std::to_string(i)), buffer.at(i));
}

TEST(resources, delimiter) {
  ASSERT_EQ(";", RES_PATHS_DELIM) << "Delimiter is expected to match cmake's semicolon concatenation.";
}
