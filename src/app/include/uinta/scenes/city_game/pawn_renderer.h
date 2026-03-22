#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_RENDERER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_RENDERER_H_

#include <array>
#include <memory>

#include "./pawn_data.h"
#include "./pawn_types.h"
#include "uinta/color.h"
#include "uinta/gl.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class PawnRenderer {
 public:
  explicit PawnRenderer(BasicShaderManager* bsm, const OpenGLApi* gl) noexcept;

  void render(const Pawns& pawns) noexcept;

  // Get color for pawn state
  static Color getPawnColor(PawnState state) noexcept;

 private:
  static constexpr size_t STATE_COUNT = 5;

  // One VAO/VBO per state for different colors
  std::array<std::unique_ptr<Vao>, STATE_COUNT> vaos_;
  std::array<std::unique_ptr<Vbo>, STATE_COUNT> vbos_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
  size_t cubeIndexCount_ = 0;

  void initStateMeshes() noexcept;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_PAWN_RENDERER_H_
