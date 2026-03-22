#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_TEXTURE_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_TEXTURE_H_

#include "uinta/gl.h"
#include "uinta/types.h"

namespace uinta {

class MockTexture {
 public:
  MockTexture() = default;

  void setId(GLuint id) noexcept { id_ = id; }

  void setTarget(GLenum target) noexcept { target_ = target; }

  [[nodiscard]] GLuint id() const noexcept { return id_; }

  [[nodiscard]] GLenum target() const noexcept { return target_; }

  [[nodiscard]] i32 width() const noexcept { return 0; }

  [[nodiscard]] i32 height() const noexcept { return 0; }

  [[nodiscard]] i32 channels() const noexcept { return 0; }

 private:
  GLuint id_ = 0;
  GLenum target_ = GL_TEXTURE_2D;
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_TEXTURE_H_
