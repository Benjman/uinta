#ifndef SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_
#define SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_

#include <manifold/manifold.h>

#include "uinta/mesh.h"

namespace uinta {

/// Converts a `manifold::Manifold` into an indexed `uinta::Mesh`.
///
/// Positions are read from Manifold's `MeshGL::vertProperties` (channels 0-2)
/// and indices are copied from `MeshGL::triVerts`. Normals are produced by
/// `Manifold::CalculateNormals(0, minSharpAngle)` and read back from property
/// channels 3-5 of the resulting `MeshGL` (Manifold writes normals after any
/// reserved user property channels; with 0 reserved, the layout is
/// `pos(0..2), normal(3..5)`).
///
/// `Vertex::color` and `Vertex::uv` are left at their defaults
/// (`{1,1,1}` and `{0,0}` respectively); Manifold does not produce UVs, and
/// color is intentionally not carried through here to keep the conversion a
/// pure geometric translation. Use `Mesh::color(...)` post-conversion if you
/// need a uniform tint.
///
/// @param m The source manifold.
/// @param minSharpAngle Degrees. Edges whose dihedral angle is sharper than
///        this value are kept faceted (vertices duplicated along the seam);
///        smoother edges share normals. Default 60 matches Manifold's own
///        convention.
/// @return A `Mesh` with positions, normals, and triangle indices populated.
///         Returns an empty mesh if the manifold is empty.
Mesh ToMesh(const manifold::Manifold& m, double minSharpAngle = 60.0) noexcept;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_MANIFOLD_MESH_H_
