#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_RENDERER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_RENDERER_H_

#include "./road_types.h"
#include "uinta/gl.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/types.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class RoadRenderer {
 public:
  explicit RoadRenderer(BasicShaderManager* bsm,
                        const OpenGLApi* gl) noexcept;

  void addRoad(const Road& road) noexcept;
  void render() const noexcept;
  bool empty() const noexcept { return indexCount_ == 0; }

 private:
  Vao vao_;
  Vbo vbo_;
  BasicShaderManager* bsm_;
  BufferSegment vtxSegment_;
  BufferSegment idxSegment_;
  size_t indexCount_ = 0;
  idx_t idxOffset_ = 0;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_RENDERER_H_
