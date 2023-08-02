#ifndef UINTA_HEXAGON_HPP
#define UINTA_HEXAGON_HPP

#include <uinta/types.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace uinta {

/// Number of indices per hexagon for index buffering.
static constexpr u32 IndicesPerHex = 18;

/// Each hexagon has its six outer points, along with an additional center point to allow for triangle mesh generation.
static constexpr u32 VerticesPerHex = 7;

/**
 * @brief Calculates the spacing of a hexagon.
 *
 * @param radius The radius of the hexagon.
 * @return 2D floating-point vector representing the horizontal and vertical spacing between hexagons along a 2D plane.
 */
glm::vec2 hex_spacing(f32 radius);

/**
 * @brief Generates a single point for a pointy-top hexagon.
 *
 * @param origin The origin of the hexagon.
 * @param radius The radius of the hexagon.
 * @param index Which of the six points to calculate.
 * @return Point in 3D space along the XZ-plane.
 */
glm::vec3 hex_point(const glm::vec2& origin, f32 radius, u32 index);

/**
 * @brief Generate six radial points representing a hexagon.
 *
 * @param origin The origin of the hexagon.
 * @param radius The radius of the hexagon.
 * @return Collection of points in 3D space along the XZ-plane.
 */
std::array<glm::vec3, VerticesPerHex> hex_points(const glm::vec2& origin, f32 radius);

/**
 * @brief Packs vertex and index buffers with related data for uploading to the GPU.
 *
 * @param points Collection of points to pack into the vertex buffer. Number of points must match number of surface normals.
 * @param colors Colors for each hexagon. A modulus function is used, so the count of these doesn't matter, as long as there is at
 * least one.
 * @param vtxBuffer The buffer used for vertex data.
 * @param idxBuffer The buffer used for index data.
 * @param idxOffset The offset to use when generating the index buffer.
 *
 * Usage:
 * @code
 *   const auto points = radial_hexagons(origin, rings, radius);
 *   const auto normals = hex_normals(points);
 *   const std::vector<glm::vec3> colors(points.size() / VerticesPerHex);
 *   f32 vtxBuffer[VerticesPerHex * 9 * hexagons.size()];
 *   u32 idxBuffer[IndicesPerHex * hexagons.size()];
 *   u32 idxOffset = 0;
 *   hexagon_pack(points, colors, vtxBuffer, idxBuffer, idxOffset);
 *   initVao(vao);
 *   uploadVbo(vbo, vtxBuffer, sizeof(vtxBuffer));
 *   initVertexAttribs(vao);
 *   indexBuffer(vao, idxBuffer, sizeof(idxBuffer));
 *   indexCount = IndicesPerHex * points.size() / VerticesPerHex;
 * @endcode
 */
void hexagon_pack(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& colors, f32* const vtxBuffer,
                  u32* const idxBuffer, u32& idxOffset);

}  // namespace uinta

#endif  // UINTA_HEXAGON_HPP
