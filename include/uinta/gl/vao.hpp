#ifndef UINTA_VAO_HPP
#define UINTA_VAO_HPP

#include <spdlog/fwd.h>

#include <uinta/fwd.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/gl/vertex_attrib.hpp>
#include <vector>

namespace uinta {

class Vao {
 public:
  Vao(const std::vector<VertexAttrib>& attributes);

  u32 id() const noexcept {
    return m_id;
  }

  std::vector<VertexAttrib> attributes() const noexcept {
    return m_attributes;
  }

  Vbo& index_buffer() noexcept {
    return m_index_buffer;
  }

  void init(spdlog::logger* logger = nullptr);

  void bind() const;

  void index_buffer(const std::span<u32> data);

  void index_buffer(const u32* const data, u32 size);

  void init_attributes() const;

  void enable_attributes() const;

  void disable_attributes();

  void destroy();

  void unbind() const;

 private:
  u32 m_id = 0;
  std::vector<VertexAttrib> m_attributes;
  Vbo m_index_buffer;
  spdlog::logger* m_logger;
};

}  // namespace uinta

#endif  // UINTA_VAO_HPP
