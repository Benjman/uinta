#ifndef UINTA_MOCK_GRID_HPP
#define UINTA_MOCK_GRID_HPP

#include <functional>
#include <uinta/grid.hpp>

#include "./mock_scene.hpp"

namespace uinta {

class MockGridRenderer : public GridRenderer {
 public:
  MockGridRenderer(const Grid& grid) : GridRenderer(grid) {
  }

  std::function<uinta_error_code()> on_init;
  uinta_error_code init(FileManager& fileManager) override {
    if (on_init) return on_init();
    return SUCCESS_EC;
  }

  std::function<void()> on_render;
  void render(const glm::mat4& projectViewMatrix) const override {
    if (on_render) on_render();
  }

  std::function<void()> on_upload;
  void upload(const f32* const buffer, size_t size, size_t offset) override {
    if (on_upload) on_upload();
  }
};

class MockGrid : public Grid {
 public:
  MockGrid() : Grid(MockScene(), std::make_unique<MockGridRenderer>(*this)) {
  }
};

}  // namespace uinta

#endif  // UINTA_MOCK_GRID_HPP
