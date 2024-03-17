#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_

#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

struct PrimitiveShader : Shader {
  explicit PrimitiveShader(
      const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  Uniform1i sway = {"sway", this};
  Uniform1f time = {"time", this};
  Uniform1f ambientStr = {"ambientStr", this};
  Uniform1f diffuseMin = {"diffuseMin", this};
  Uniform3f lightColor = {"lightColor", this};
  Uniform3f lightDir = {"lightDir", this};
  UniformMatrix4fv model = {"model", this};
  UniformMatrix4fv projection = {"projection", this};
  UniformMatrix4fv view = {"view", this};
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
