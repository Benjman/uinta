#include <string>
#include <vector>

#include "uinta/logging.hpp"
#include "uinta/resource/fwd.hpp"

namespace uinta {
void parseResourcePaths(const std::string& paths, const char delim, std::vector<std::string>& buffer, const logger_t& logger);
std::string findPath(const std::string& path, const std::vector<std::string>& resPaths);
}  // namespace uinta
