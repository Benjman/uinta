#ifndef UINTA_VBO_HPP
#define UINTA_VBO_HPP

#include <spdlog/fwd.h>
#include <uinta/gl/types.h>

#include <memory>

namespace uinta {

class Vbo {
 public:
  Vbo(gl_enum target, gl_enum usage, size_t reserve = 0) : m_target(target), m_usage(usage), m_size(reserve) {
  }

  void init(const spdlog::logger* logger = nullptr);

  void bind() const;

  void upload(const void* const data, size_t size, size_t offset);

  void resize(size_t size);

  void unbind() const;

  void destroy();

  u32 id() const noexcept {
    return m_id;
  }

  gl_enum target() const noexcept {
    return m_target;
  }

  gl_enum usage() const noexcept {
    return m_usage;
  }

  size_t size() const noexcept {
    return m_size;
  }

  void size(size_t v) noexcept;

  size_t max() const noexcept {
    return m_max;
  }

 private:
  u32 m_id = 0;
  gl_enum m_target;
  gl_enum m_usage;
  size_t m_size = 0;
  size_t m_max = 0;
  const spdlog::logger* m_logger;
};

}  // namespace uinta

#endif  // UINTA_VBO_HPP
