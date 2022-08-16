#include "./utils.hpp"

#include <spdlog/spdlog.h>

#include <fstream>

#include "uinta/cfg.hpp"

namespace uinta {
inline std::string sanitizePath(const std::string& searchPath, const std::string& path) {
  // TODO ensure search path, or path aren't double slashing "//", or that a slash isn't missing between the two
  return searchPath + path;
}
}  // namespace uinta

void uinta::parseFileSearchPaths(const std::string& searchPaths, const char delim, std::vector<std::string>& buffer) {
  std::string val;
  int start = 0;
  int end = 0;
  buffer.clear();
  do {
    end = searchPaths.find(delim, start);
    val = std::string(searchPaths.substr(start, end - start));
    if (val.length()) {
      if (val.at(val.length() - 1) != '/') val = val + '/';
      buffer.emplace_back(std::string(val));
      SPDLOG_LOGGER_DEBUG(logger, "Search path '{}' registered", val);
    }
    start = end + 1;  // +1 for delim
  } while (end != -1);
  if (buffer.size() > 0) {
    SPDLOG_INFO("Found {} file paths.", buffer.size());
  } else {
    SPDLOG_WARN("Failed to parse any file search paths from input '{}'!", searchPaths);
  }
}

std::string uinta::findPath(const std::string& path, const std::vector<std::string>& searchPaths) {
  if (std::ifstream(path)) return path;
  for (auto& searchPath : searchPaths) {
    std::string absPath = sanitizePath(searchPath, path);
    if (std::ifstream(absPath)) return absPath;
  }
  return "";
}
