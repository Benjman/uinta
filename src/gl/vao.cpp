#include <uinta/error.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vertex_attrib.hpp>
#include <uinta/logging.hpp>

namespace uinta {

void Vao::init() {
  assert(!m_id && "Cannot re-initialized an already initialized VAO.");
  glGenVertexArrays(1, &m_id);
  SPDLOG_DEBUG("Initialized VAO {}.", m_id);
  bind();
  init_attributes();
  m_index_buffer.init();
}

void Vao::bind() const {
  assert(m_id && "Cannot bind an unitialized VAO.");
  glBindVertexArray(m_id);
}

void Vao::index_buffer(const u32* const data, u32 size) {
  assert(m_id && "Cannot create index buffer for an uninitialized VAO.");
  assert(!m_index_buffer.id() && "Cannot recreate an already initialized index buffer.");
  m_index_buffer.init();
  bind();
  m_index_buffer.upload(data, size, 0);
  init_attributes();
}

void Vao::init_attributes() const {
  assert(m_id && "Cannot initialize attributes of an uninitialized VAO..");
  for (auto& attribute : m_attributes)
    glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride,
                          attribute.pointer);
  enable_attributes();
}

void Vao::enable_attributes() const {
  assert(m_id && "Cannot enable attributes of an uninitialized VAO.");
  bind();
  for (const auto& attribute : m_attributes) glEnableVertexAttribArray(attribute.index);
}

void Vao::disable_attributes() {
  assert(m_id && "Cannot disable attributes of an uninitialized VAO.");
  if (!m_id) return;
  bind();
  for (const auto& attribute : m_attributes) glDisableVertexAttribArray(attribute.index);
}

void Vao::destroy() {
  assert(m_id && "Cannot destroy an uninitialized VAO.");
  bind();
  disable_attributes();
  glDeleteVertexArrays(1, &m_id);
  m_id = 0;
  m_attributes = {};
  m_index_buffer.destroy();
}

void Vao::unbind() const {
  glBindVertexArray(0);
}

}  // namespace uinta
