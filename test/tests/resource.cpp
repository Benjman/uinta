#include <gtest/gtest.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define private public
#include <uinta/resource.hpp>
#include <uinta/cfg.hpp>

using namespace uinta;
using namespace uinta::resource;

static inline logger_t logger = spdlog::stdout_color_mt("ResourceManagerTest");

#include "./resource/parsePaths.cpp"
#include "./resource/resourceManager.cpp"
