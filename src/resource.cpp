#include <uinta/resource.hpp>
#include <uinta/cfg.hpp>

#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace uinta::resource;

namespace internal {
uint16_t parsePaths(const char *const paths, std::string *const buffer, const logger_t logger);
} // namespace internal

ResourceManager::ResourceManager() : pathsCount(0) {
  logger = spdlog::stdout_color_mt("ResourceManager");

  pathsCount = internal::parsePaths(RES_PATHS, paths, logger);
  if (pathsCount > 0) {
    SPDLOG_LOGGER_INFO(logger, "Found {} resource paths.", pathsCount);
  } else {
    SPDLOG_LOGGER_WARN(logger, "Failed to parse any resource paths from input '{}'!", RES_PATHS);
  }
}

ResourceManager::~ResourceManager() { pathsCount = 0; }

uint16_t internal::parsePaths(const char *const paths, std::string *const buffer, const logger_t logger) {
  std::string val;
  std::string s = paths;

  int count = 0;
  int start = 0;
  int end   = s.find(RES_PATHS_DELIM);

  while (end != -1) {
    val = std::string(s.substr(start, end - start));
    if (val.length()) {
      buffer[count] = std::string(val);
      SPDLOG_LOGGER_DEBUG(logger, "Resource path '{}' registered", val);
      count++;
    }

    start = end + 1; // + 1 is the size of the RES_PATHS_DELIM
    end   = s.find(RES_PATHS_DELIM, start);
  }

  val = std::string(s.substr(start, end - start));
  if (val.length()) {
    buffer[count] = std::string(val);
    SPDLOG_LOGGER_DEBUG(logger, "Resource path '{}' registered", val);
    count++;
  }

  return count;
}
