#ifndef SRC_ENGINE_INCLUDE_UINTA_UTILS_FBX_H_
#define SRC_ENGINE_INCLUDE_UINTA_UTILS_FBX_H_

#include <string_view>
#include <vector>

#include "uinta/status.h"
#include "uinta/mesh.h"

namespace uinta {

StatusOr<std::vector<Mesh>> loadFbx(const std::string_view path) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UTILS_FBX_H_
