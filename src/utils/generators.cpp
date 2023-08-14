#include <spdlog/spdlog.h>
#include <uinta/types.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <uinta/utils/generators.hpp>

namespace uinta {

void generateTerrain(float* const vertices, uint* const indices, uint* const indexCount) {
  uint cellSize = 1;
  uint blockSize = 10;

  // clang-format off
  float cube[] = {
     0.5,  0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5,  0.5,  0.5,
     0.5, -0.5,  0.5,
    -0.5,  0.5, -0.5,
    -0.5, -0.5, -0.5,
    -0.5,  0.5,  0.5,
    -0.5, -0.5,  0.5,
  };
  // clang-format on

  for (int i = 0; i < blockSize; i++) {
    for (int j = 0; j < 8; j++) {
      float* loc = &cube[3 * j];
      glm::vec3 v(loc[0], loc[1], loc[2]);
    }
  }
}

}  // namespace uinta
