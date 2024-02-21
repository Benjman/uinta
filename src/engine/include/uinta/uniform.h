#ifndef SRC_ENGINE_INCLUDE_UINTA_UNIFORM_H_
#define SRC_ENGINE_INCLUDE_UINTA_UNIFORM_H_

#include <string>
#include <utility>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "glm/ext/matrix_float2x2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/types.h"

namespace uinta {

// FIXME: Uniform values are not cached efficiently.
// https://trello.com/c/9iBv9Qxv

template <typename T>
class Uniform {
 public:
  Uniform(std::string name, const Shader* shader) noexcept : gl_(shader->gl()) {
    if (!shader->id()) {
      LOG(FATAL) << absl::StrFormat(("Shader not ready for uniform: %s"), name);
      return;
    }
    location_ = gl_->getUniformLocation(shader->id(), name.data());
    if (location_ == -1) {
      LOG(WARNING) << (absl::StrFormat(
          "Shader %i uniform location not found: %s", shader->id(), name));
    }
  }

  T value() const noexcept { return value_; }

  GLint location() const noexcept { return location_; }

 protected:
  const OpenGLApi* gl_;
  GLint location_ = -1;
  T value_;
};

class Uniform1f : public Uniform<GLfloat> {
 public:
  Uniform1f(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const GLfloat& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform1f(location_, v);
      value_ = std::move(v);
    }
  }
};

class Uniform2f : public Uniform<glm::vec2> {
 public:
  Uniform2f(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::vec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2f(location_, v.x, v.y);
      value_ = std::move(v);
    }
  }
};

class Uniform3f : public Uniform<glm::vec3> {
 public:
  Uniform3f(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::vec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3f(location_, v.x, v.y, v.z);
      value_ = std::move(v);
    }
  }
};

class Uniform4f : public Uniform<glm::vec4> {
 public:
  Uniform4f(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::vec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4f(location_, v.x, v.y, v.z, v.w);
      value_ = std::move(v);
    }
  }
};

class Uniform1i : public Uniform<GLint> {
 public:
  Uniform1i(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const GLint& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform1i(location_, v);
      value_ = std::move(v);
    }
  }
};

class Uniform2i : public Uniform<glm::ivec2> {
 public:
  Uniform2i(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::ivec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2i(location_, v.x, v.y);
      value_ = std::move(v);
    }
  }
};

class Uniform3i : public Uniform<glm::ivec3> {
 public:
  Uniform3i(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::ivec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3i(location_, v.x, v.y, v.z);
      value_ = std::move(v);
    }
  }
};

class Uniform4i : public Uniform<glm::ivec4> {
 public:
  Uniform4i(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::ivec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4i(location_, v.x, v.y, v.z, v.w);
      value_ = std::move(v);
    }
  }
};

class Uniform1ui : public Uniform<GLuint> {
 public:
  Uniform1ui(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const GLuint& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform1ui(location_, v);
      value_ = std::move(v);
    }
  }
};

class Uniform2ui : public Uniform<glm::uvec2> {
 public:
  Uniform2ui(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::uvec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2ui(location_, v.x, v.y);
      value_ = std::move(v);
    }
  }
};

class Uniform3ui : public Uniform<glm::uvec3> {
 public:
  Uniform3ui(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::uvec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3ui(location_, v.x, v.y, v.z);
      value_ = std::move(v);
    }
  }
};

class Uniform4ui : public Uniform<glm::uvec4> {
 public:
  Uniform4ui(std::string name, const Shader* shader) noexcept
      : Uniform(name, shader) {}

  void operator=(const glm::uvec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4ui(location_, v.x, v.y, v.z, v.w);
      value_ = std::move(v);
    }
  }
};

class Uniform1fv : public Uniform<GLfloat> {
 public:
  Uniform1fv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const GLfloat& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2fv(location_, count_, &v);
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform2fv : public Uniform<glm::vec2> {
 public:
  Uniform2fv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::vec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2fv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform3fv : public Uniform<glm::vec3> {
 public:
  Uniform3fv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::vec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3fv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform4fv : public Uniform<glm::vec4> {
 public:
  Uniform4fv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::vec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4fv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform1iv : public Uniform<GLint> {
 public:
  Uniform1iv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const GLint& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform1iv(location_, count_, &v);
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform2iv : public Uniform<glm::ivec2> {
 public:
  Uniform2iv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::ivec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2iv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform3iv : public Uniform<glm::ivec3> {
 public:
  Uniform3iv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::ivec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3iv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform4iv : public Uniform<glm::ivec4> {
 public:
  Uniform4iv(std::string name, const Shader* shader, GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::ivec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4iv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform1uiv : public Uniform<u32> {
 public:
  Uniform1uiv(std::string name, const Shader* shader,
              GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const u32& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform1uiv(location_, count_, &v);
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform2uiv : public Uniform<glm::uvec2> {
 public:
  Uniform2uiv(std::string name, const Shader* shader,
              GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::uvec2& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform2uiv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform3uiv : public Uniform<glm::uvec3> {
 public:
  Uniform3uiv(std::string name, const Shader* shader,
              GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::uvec3& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform3uiv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class Uniform4uiv : public Uniform<glm::uvec4> {
 public:
  Uniform4uiv(std::string name, const Shader* shader,
              GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::uvec4& v) noexcept {
    if (location_ != -1 && value_ != v) {
      gl_->uniform4uiv(location_, count_, glm::value_ptr(v));
      value_ = std::move(v);
    }
  }

 private:
  GLsizei count_;
};

class UniformMatrix2fv : public Uniform<glm::mat2> {
 public:
  UniformMatrix2fv(std::string name, const Shader* shader,
                   GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat2& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix2fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix3fv : public Uniform<glm::mat3> {
 public:
  UniformMatrix3fv(std::string name, const Shader* shader,
                   GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat3& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix3fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix4fv : public Uniform<glm::mat4> {
 public:
  UniformMatrix4fv(std::string name, const Shader* shader,
                   GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat4& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix4fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix2x3fv : public Uniform<glm::mat2x3> {
 public:
  UniformMatrix2x3fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat2x3& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix2x3fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix3x2fv : public Uniform<glm::mat3x2> {
 public:
  UniformMatrix3x2fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat3x2& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix3x2fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix2x4fv : public Uniform<glm::mat2x4> {
 public:
  UniformMatrix2x4fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat2x4& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix2x4fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix4x2fv : public Uniform<glm::mat4x2> {
 public:
  UniformMatrix4x2fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat4x2& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix4x2fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix3x4fv : public Uniform<glm::mat3x4> {
 public:
  UniformMatrix3x4fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat3x4& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix3x4fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

class UniformMatrix4x3fv : public Uniform<glm::mat4x3> {
 public:
  UniformMatrix4x3fv(std::string name, const Shader* shader,
                     GLsizei count = 1) noexcept
      : Uniform(name, shader), count_(count) {}

  void operator=(const glm::mat4x3& v) noexcept {
    if (location_ != -1)
      gl_->uniformMatrix4x3fv(location_, count_, GL_FALSE, glm::value_ptr(v));
  }

 private:
  GLsizei count_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_UNIFORM_H_
