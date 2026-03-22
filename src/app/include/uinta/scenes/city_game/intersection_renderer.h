#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_RENDERER_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_RENDERER_H_

#include "./intersection_data.h"
#include "uinta/gl.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class IntersectionRenderer {
 public:
  explicit IntersectionRenderer(BasicShaderManager* bsm,
                                const OpenGLApi* gl) noexcept;

  // Build mesh for all buildings (called once at init)
  void buildMesh(const IntersectionBuildings& buildings) noexcept;

  // Render the intersection (buildings + ground)
  void render() const noexcept;

  bool empty() const noexcept { return buildingIndexCount_ == 0; }

 private:
  Vao buildingVao_;
  Vbo buildingVbo_;
  Vao groundVao_;
  Vbo groundVbo_;
  BasicShaderManager* bsm_;

  BufferSegment buildingVtxSegment_;
  BufferSegment buildingIdxSegment_;
  size_t buildingIndexCount_ = 0;

  BufferSegment groundVtxSegment_;
  BufferSegment groundIdxSegment_;
  size_t groundIndexCount_ = 0;

  void initGroundMesh() noexcept;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_INTERSECTION_RENDERER_H_
