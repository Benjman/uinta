// This file is included in /uinta/test/resource.cpp, so syntax errors in this file will work when compiling

#include <gtest/gtest.h>

#define private public
#include <uinta/resource.hpp>
#include <uinta/cfg.hpp>

using namespace uinta;
using namespace uinta::resource;

TEST(ResourceManager, constructor) {
  ResourceManager manager;

}
