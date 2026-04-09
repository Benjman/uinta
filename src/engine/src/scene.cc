#include "uinta/scene/scene.h"

#include <cassert>

namespace uinta {

Scene::Scene(Engine* engine) noexcept : engine_(engine) {}

Scene::Scene(Scene* parent) noexcept : parent_(parent), engine_(parent->engine()) {
  assert(parent && "`Scene*` cannot be null.");
}

}  // namespace uinta
