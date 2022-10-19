#include <glm/vec3.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

const glm::vec3 WORLD_FORWARD{0, 0, -1};
const glm::vec3 WORLD_HORIZONTAL = WORLD_RIGHT + WORLD_FORWARD;
const glm::vec3 WORLD_RIGHT{1, 0, 0};
const glm::vec3 WORLD_UP{0, 1, 0};

}  // namespace uinta
