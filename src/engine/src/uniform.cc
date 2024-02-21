#include "uinta/uniform.h"

#include "absl/log/log.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/shader.h"

namespace uinta {

template <typename T>
Uniform<T>::Uniform(const std::string_view name, const Shader* shader) noexcept
    : gl_(shader->gl()) {
  if (!shader->id()) {
    LOG(FATAL) << "Shader not ready for uniform: " << name;
    return;
  }
  location_ = gl_->getUniformLocation(shader->id(), name.data());
  if (location_ == -1)
    LOG(WARNING) << StrFormat("Shader %i uniform location not found: %s",
                              shader->id(), name);
}

Uniform1f::Uniform1f(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform2f::Uniform2f(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform3f::Uniform3f(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform4f::Uniform4f(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform1i::Uniform1i(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform2i::Uniform2i(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform3i::Uniform3i(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform4i::Uniform4i(const std::string_view name, const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform1ui::Uniform1ui(const std::string_view name,
                       const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform2ui::Uniform2ui(const std::string_view name,
                       const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform3ui::Uniform3ui(const std::string_view name,
                       const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform4ui::Uniform4ui(const std::string_view name,
                       const Shader* shader) noexcept
    : Uniform(name, shader) {}

Uniform1fv::Uniform1fv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform2fv::Uniform2fv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform3fv::Uniform3fv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform4fv::Uniform4fv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform1iv::Uniform1iv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform2iv::Uniform2iv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform3iv::Uniform3iv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform4iv::Uniform4iv(const std::string_view name, const Shader* shader,
                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform1uiv::Uniform1uiv(const std::string_view name, const Shader* shader,
                         GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform2uiv::Uniform2uiv(const std::string_view name, const Shader* shader,
                         GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform3uiv::Uniform3uiv(const std::string_view name, const Shader* shader,
                         GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

Uniform4uiv::Uniform4uiv(const std::string_view name, const Shader* shader,
                         GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix2fv::UniformMatrix2fv(const std::string_view name,
                                   const Shader* shader, GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix3fv::UniformMatrix3fv(const std::string_view name,
                                   const Shader* shader, GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix4fv::UniformMatrix4fv(const std::string_view name,
                                   const Shader* shader, GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix2x3fv::UniformMatrix2x3fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix3x2fv::UniformMatrix3x2fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix2x4fv::UniformMatrix2x4fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix4x2fv::UniformMatrix4x2fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix3x4fv::UniformMatrix3x4fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

UniformMatrix4x3fv::UniformMatrix4x3fv(const std::string_view name,
                                       const Shader* shader,
                                       GLsizei count) noexcept
    : Uniform(name, shader), count_(count) {}

}  // namespace uinta
