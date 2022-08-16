#include <string>
#include <vector>

#include "uinta/io/fwd.hpp"
#include "uinta/logging.hpp"

namespace uinta {
void parseFileSearchPaths(const std::string& paths, const char delim, std::vector<std::string>& buffer);
std::string findPath(const std::string& path, const std::vector<std::string>& filePaths);
}  // namespace uinta
