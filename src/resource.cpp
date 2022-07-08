#include <uinta/resource.hpp>
#include <uinta/cfg.hpp>

#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace uinta::resource;
using namespace uinta::resource::internal;

ResourceManager::ResourceManager() {
  logger = spdlog::stdout_color_mt("ResourceManager");
  parsePaths(RES_PATHS, paths, logger);
}

ResourceHandle *const ResourceManager::registerResource(const std::string &relativePath) {
  if (ResourceHandle *const handle = findHandle(relativePath); handle)
    return handle;
  return generateHandle(relativePath);
}

ResourceHandle *const ResourceManager::findHandle(const std::string &relativePath) {
  for (int i = 0; i < paths.size(); i++)
    if (paths.at(i).compare(relativePath) == 0)
      return &handles.at(i);
  return nullptr;
}

ResourceHandle *const ResourceManager::generateHandle(const std::string &relativePath) { return nullptr; }

void ResourceManager::setId(ResourceHandle &handle, const resource_handle_t id) { handle.handle |= id & UINTA_RES_ID_MASK; }

void uinta::resource::internal::parsePaths(const char *const paths, std::vector<std::string> &buffer, const logger_t &logger) {
  buffer.clear();

  std::string val;
  std::string s = paths;

  int start = 0;
  int end   = 0;

  do {
    end = s.find(RES_PATHS_DELIM, start);

    val = std::string(s.substr(start, end - start));
    if (val.length()) {
      buffer.emplace_back(std::string(val));
      SPDLOG_LOGGER_DEBUG(logger, "Resource path '{}' registered", val);
    }

    start = end + 1; // + 1 is the size of the RES_PATHS_DELIM
  } while (end != -1);

  if (buffer.size() > 0) {
    SPDLOG_LOGGER_INFO(logger, "Found {} resource paths.", buffer.size());
  } else {
    SPDLOG_LOGGER_WARN(logger, "Failed to parse any resource paths from input '{}'!", RES_PATHS);
  }
}
