#include <spdlog/spdlog.h>

#include <uinta/mesh.hpp>

const inline auto LINE_SIZE = 4;
const inline auto LINE_SIZE_HALF = LINE_SIZE / 2.0f;

namespace uinta {
std::string to_string(MeshAttribType);

MeshAttrib findMeshAttrib(MeshAttribType type, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  if (attribs.find(type) == attribs.end()) {
    SPDLOG_ERROR("Unable to find MeshAttrib '{}'!", to_string(type));
    return {0, 0};
  }
  return attribs.at(type);
}

bool hasMeshAttrib(MeshAttribType type, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  return attribs.find(type) != attribs.end();
}

}  // namespace uinta
