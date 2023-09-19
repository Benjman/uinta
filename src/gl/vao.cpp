#include <uinta/error.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vertex_attrib.hpp>
#include <uinta/logging.hpp>

namespace uinta {

void Vao::init(const spdlog::logger* logger) {
  if (logger) m_logger = logger;
  assert(!m_id && "Cannot re-initialized an already initialized VAO.");
  glGenVertexArrays(1, &m_id);
  if (m_logger) SPDLOG_LOGGER_DEBUG(const_cast<spdlog::logger*>(m_logger), "Initialized VAO {}.", m_id);
  bind();
  m_index_buffer.init(m_logger);
}

void Vao::bind() const {
  assert(m_id && "Cannot bind an unitialized VAO.");
  glBindVertexArray(m_id);
}

void Vao::index_buffer(const u32* const data, u32 size) {
  assert(m_id && "Cannot create index buffer for an uninitialized VAO.");
  assert(!m_index_buffer.id() && "Cannot recreate an already initialized index buffer.");
  m_index_buffer.init(m_logger);
  bind();
  m_index_buffer.upload(data, size, 0);
  init_attributes();
  if (m_logger) SPDLOG_LOGGER_INFO(const_cast<spdlog::logger*>(m_logger), "Created index buffer.");
}

void Vao::init_attributes() const {
  assert(m_id && "Cannot initialize attributes of an uninitialized VAO..");
  for (auto& attribute : m_attributes)
    glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride,
                          attribute.pointer);
  enable_attributes();
  if (m_logger) SPDLOG_LOGGER_DEBUG(const_cast<spdlog::logger*>(m_logger), "Initialized attributes.");
}

void Vao::enable_attributes() const {
  assert(m_id && "Cannot enable attributes of an uninitialized VAO.");
  bind();
  for (const auto& attribute : m_attributes) glEnableVertexAttribArray(attribute.index);
  if (m_logger) SPDLOG_LOGGER_TRACE(const_cast<spdlog::logger*>(m_logger), "Enabled all attributes.");
}

void Vao::disable_attributes() {
  assert(m_id && "Cannot disable attributes of an uninitialized VAO.");
  if (!m_id) return;
  bind();
  for (const auto& attribute : m_attributes) glDisableVertexAttribArray(attribute.index);
  if (m_logger) SPDLOG_LOGGER_TRACE(const_cast<spdlog::logger*>(m_logger), "Disabled all attributes.");
}

void Vao::destroy() {
  assert(m_id && "Cannot destroy an uninitialized VAO.");
  bind();
  disable_attributes();
  glDeleteVertexArrays(1, &m_id);
  m_id = 0;
  m_attributes = {};
  m_index_buffer.destroy();
  if (m_logger) SPDLOG_LOGGER_INFO(const_cast<spdlog::logger*>(m_logger), "Destroyed VAO.");
}

void Vao::unbind() const {
  glBindVertexArray(0);
}

}  // namespace uinta
