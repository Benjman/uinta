#include "uinta/environment.h"

#include <algorithm>
#include <limits>

#include "absl/random/random.h"
#include "glm/ext/matrix_transform.hpp"
#include "uinta/color.h"

namespace uinta {

namespace tree {
Primitive canopy(size_t* idxOffset) noexcept;
Primitive trunk(size_t* idxOffset) noexcept;
}  // namespace tree

Primitive Environment::Tree(size_t* idxOffset) noexcept {
  assert(idxOffset && "`*idxOffset` cannot be null.");
  auto canopy = tree::canopy(idxOffset);
  *idxOffset += 24;
  auto trunk = tree::trunk(idxOffset);
  *idxOffset += 24;
  std::vector<Primitive> collection = {canopy, trunk};
  return Primitive(collection);
}

std::vector<std::pair<glm::vec3, glm::vec3>> primitiveDisplacementMap(
    const Primitive& cube, f32 magnitude, glm::vec3 mask = glm::vec3(1));

Primitive tree::canopy(size_t* idxOffset) noexcept {
  auto cube = Primitive::Cube(*idxOffset);
  auto displacement = primitiveDisplacementMap(cube, 0.1);
  std::for_each(
      cube.vertices().begin(), cube.vertices().end(),
      [&displacement](auto& vertex) {
        auto transform = glm::scale(glm::mat4(1), glm::vec3(0.75, 1, 0.75));
        transform = glm::translate(transform, glm::vec3(0, 1.1, 0));
        auto& position = vertex.position;
        /* auto& normal = v.normal; */
        auto rand = std::find_if(displacement.begin(), displacement.end(),
                                 [&position](auto& r) {
                                   return glm::length(r.first - position) <
                                          std::numeric_limits<f32>::epsilon();
                                 });
        position = transform * glm::vec4(position + rand->second, 1);
        vertex.color = color::Green600;
      });
  return cube;
}

Primitive tree::trunk(size_t* idxOffset) noexcept {
  auto cube = Primitive::Cube(*idxOffset);
  auto displacement = primitiveDisplacementMap(cube, 0.1);
  std::for_each(
      cube.vertices().begin(), cube.vertices().end(), [](auto& vertex) {
        auto transform = glm::scale(glm::mat4(1), glm::vec3(0.25, 1, 0.25));
        transform = glm::rotate(transform, 0.f, glm::vec3(0, 1, 0));
        transform = glm::translate(transform, glm::vec3(0, 0.5, 0));
        vertex.position = transform * glm::vec4(vertex.position, 1);
        vertex.color = color::Brown500;
      });
  return cube;
}

std::vector<std::pair<glm::vec3, glm::vec3>> primitiveDisplacementMap(
    const Primitive& cube, f32 magnitude, glm::vec3 mask) {
  absl::BitGen bitgen;
  absl::uniform_real_distribution<float> displacement(-magnitude, magnitude);
  std::vector<std::pair<glm::vec3, glm::vec3>> result;
  for (auto& vertex : cube.vertices()) {
    auto& position = vertex.position;
    if (std::find_if(result.begin(), result.end(), [&position](auto& r) {
          return glm::length(r.first - position) <
                 std::numeric_limits<f32>::epsilon();
        }) == result.end()) {
      result.push_back(
          {position, glm::vec3(displacement(bitgen), displacement(bitgen),
                               displacement(bitgen)) *
                         mask});
    }
  }
  return result;
}

}  // namespace uinta
