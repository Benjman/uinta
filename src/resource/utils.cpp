#include "./utils.hpp"

#include <spdlog/spdlog.h>

#include <fstream>

#include "uinta/cfg.hpp"

void uinta::parseResourcePaths(const std::string& paths, const char delim, std::vector<std::string>& buffer,
                               const logger_t& logger) {
  std::string val;
  int start = 0;
  int end = 0;
  buffer.clear();
  do {
    end = paths.find(RES_PATHS_DELIM, start);
    val = std::string(paths.substr(start, end - start));
    if (val.length()) {
      if (val.at(val.length() - 1) != '/') val = val + '/';
      buffer.emplace_back(std::string(val));
      SPDLOG_LOGGER_DEBUG(logger, "Resource path '{}' registered", val);
    }
    start = end + 1;  // +1 for delim
  } while (end != -1);
  if (buffer.size() > 0) {
    SPDLOG_LOGGER_INFO(logger, "Found {} resource paths.", buffer.size());
  } else {
    SPDLOG_LOGGER_WARN(logger, "Failed to parse any resource paths from input '{}'!", RES_PATHS);
  }
}

std::string uinta::findPath(const std::string& path, const std::vector<std::string>& resPaths) {
  if (std::ifstream(path)) return path;
  for (auto& resPath : resPaths)
    if (std::ifstream(resPath + path)) return resPath + path;
  return "";
}
