#ifndef SRC_ENGINE_INCLUDE_UINTA_MESH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MESH_H_

#include <string_view>
#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/primitive.h"

namespace uinta {

StatusOr<std::vector<Primitive>> fbx(const std::string_view path) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MESH_H_
