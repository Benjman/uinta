#ifndef UINTA_HEXAGON_HPP
#define UINTA_HEXAGON_HPP

#include <uinta/types.h>

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <uinta/utils/buffer2d.hpp>
#include <vector>

namespace uinta {

/// Number of indices per hexagon for index buffering.
static constexpr u32 IndicesPerHex = 18;

/// Each hexagon has its six outer points, along with an additional center point to allow for triangle mesh generation.
static constexpr u32 VerticesPerHex = 7;

enum class hex_direction { East = 0, NorthEast = 1, NorthWest = 2, West = 3, SouthWest = 4, SouthEast = 5 };

/**
 * @brief Precalculated collection of all directions for a hexagon.
 */
static constexpr glm::ivec3 cube_directions[] = {
    {+1, 0, -1}, {+1, -1, 0}, {0, -1, +1}, {-1, 0, +1}, {-1, +1, 0}, {0, +1, -1},
};

/**
 * @brief Calculates the spacing of a hexagon.
 *
 * @param radius The radius of the hexagon.
 * @return 2D floating-point vector representing the horizontal and vertical spacing between hexagons along a 2D plane.
 */
constexpr glm::vec2 hex_spacing(f32 radius) {
  return {glm::root_three<f32>() * radius, 3.0 / 2.0 * radius};
}

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
 * @param normals Collection of surface normals to pack into the vertex buffer. Number of surface normals must match number of
 * points.
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
 *   hexagon_pack(points, normals, colors, vtxBuffer, idxBuffer, idxOffset);
 *   initVao(vao);
 *   uploadVbo(vbo, vtxBuffer, sizeof(vtxBuffer));
 *   initVertexAttribs(vao);
 *   indexBuffer(vao, idxBuffer, sizeof(idxBuffer));
 *   indexCount = IndicesPerHex * points.size() / VerticesPerHex;
 * @endcode
 */
void hexagon_pack(const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals,
                  const std::vector<glm::vec3>& colors, f32* const vtxBuffer, u32* const idxBuffer, u32& idxOffset);

/**
 * @brief Generates a collection of surface normals, one per point. Note: Points should have Y-axis data for this to produce
 * anything useful.
 *
 * @param points Collection of points to generate surface normals for.
 * @return Collection of surface normals related to the provided points.
 */
std::vector<glm::vec3> hex_normals(const std::vector<glm::vec3>& points);

/**
 * @brief Given a height map, this will update the provided collection of points to reflect the height value provided in the
 * `heightMap`.
 *
 * @param points The collection of points to update height values for.
 * @param heightMap The buffer to sample for height map data.
 * @param scale The scale to transform the height data to.
 */
void hex_set_heights(std::vector<glm::vec3>& points, const Buffer2d<f32>& heightMap, f32 scale);

/**
 * @brief Generates a collection of points representing a radial grid of hexagons.
 *
 * @param origin The origin point of the radial hexagon grid.
 * @param rings The number of rings for the radial grid.
 * @param radius The radius of each hexagon.
 * @return A collection of points representing a radial grid of hexagons.
 */
std::vector<glm::vec3> radial_hexagons(const glm::vec3& origin, u32 rings, f32 radius);

/**
 * @brief Converts a 2D axial coordinate to a 3D cube coordinate
 *
 * @param hex The 2D axial coordinate.
 * @return The 3D cube coordinate.
 */
glm::ivec3 axial_to_cube(const glm::vec2& hex);

/**
 * @brief Converts a 3D cube coordinate to a 2D axial coordinate
 *
 * @param The 3D cube coordinate.
 * @return The 2D axial coordinate.
 */
glm::vec2 cube_to_axial(const glm::ivec3& cube);

/**
 * @brief Returns the distance between two cube based coordinates.
 *
 * @param a Cube coordinate of the first hexagon.
 * @param b Cube coordinate of the second hexagon.
 * @return Distance between cube coordinates a, and b.
 */
f32 cube_distance(const glm::vec3& a, const glm::vec3& b);

/**
 * @brief Generate an axial coordinate when given a world position along a 2D plane (usually XZ).
 *
 * @param pos The world position.
 * @param origin The grid origin.
 * @param radius The radius of the hexagon.
 * @return The axial coordinate.
 */
glm::ivec2 world_to_axial(const glm::vec2& pos, const glm::vec2& origin, f32 radius);

/**
 * @brief Calculate the neighbor's cube coordinate of a given cube and direction.
 *
 * @param cube The cube coordinate to calculate its neighbor.
 * @param direction The direction to calculate from a given cube coordinate.
 * @return 3D cube coordinate coordinate of the neighbor of the given cube towards the given direction.
 */
/**
 * @brief Calculates the coordinate for a neighbor of a given cube towards a given direction.
 *
 * @param cube The cube coordinate to find a neighbor for.
 * @param direction The direction to calculate the neighbor's coordinate.
 * @return The neighbor's 3D cube coordinate.
 */
glm::ivec3 cube_neighbor(const glm::ivec3& cube, hex_direction direction);

/**
 * @brief Generates a collection of points representing a ring of hexagons around a specified origin.
 *
 * @param origin The origin to calculate the rings for.
 * @param radius The radius from the origin to calculate the rings for.
 * @ return A collection of points representing hexagons within a ring.
 */
std::vector<glm::ivec3> cube_ring(const glm::ivec3& origin, u32 radius);

/**
 * @brief A collection of hexagons representing every hexagon belonging to a radial grid from a specified origin out a specified
 * radius.
 *
 * @param origin The origin to calculate the spiral for.
 * @param radius The radius from the origin to calculate the spiral for.
 * @return A collection of points representing hexagons within a spiral.
 */
std::vector<glm::ivec3> cube_spiral(const glm::ivec3& origin, u32 radius);

/**
 * @brief Calculate the total number of hexagons in a radial grid.
 *
 * @param rings The number of rings in the radial grid.
 * @return The total number of hexagons in a radial grid.
 */
constexpr u32 hexagon_count(u32 rings) {
  return 1 + 3 * rings * (rings + 1);
}

}  // namespace uinta

#endif  // UINTA_HEXAGON_HPP
