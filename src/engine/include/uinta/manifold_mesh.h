#ifndef SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_

#include <manifold/manifold.h>

#include "uinta/mesh.h"

namespace uinta {

Mesh ToMesh(const manifold::Manifold& m, double minSharpAngle = 60.0) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_
