#include <uinta/gl/api.h>

#include <uinta/error.hpp>
#include <uinta/gl/utils/type_utils.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/logging.hpp>
#include <uinta/utils/formatters.hpp>

namespace uinta {

void Vbo::size(size_t v) noexcept {
  assert(!m_id && "Cannot set size after VBO has been initialized.");
  m_size = v;
}

void Vbo::init(spdlog::logger* logger) {
  assert(!m_id && "Cannot re-initialize VBO.");
  if (logger) m_logger = logger;
  glGenBuffers(1, &m_id);
  if (m_size) {
    auto size = m_size;
    m_size = 0;
    resize(size);
  } else {
    if (m_logger)
      SPDLOG_LOGGER_DEBUG(m_logger, "Initialized {} {}, drawing {}.", getGlEnumName(m_target), m_id, getGlEnumName(m_usage));
  }
}

void Vbo::bind() const {
  assert(m_id && "Cannot bind an uninitialized VBO.");
  glBindBuffer(m_target, m_id);
  if (m_logger) SPDLOG_LOGGER_TRACE(m_logger, "Bound VBO.");
}

void Vbo::upload(const void* const data, size_t size, size_t offset) {
  assert(m_id && "Cannot upload to an uninitialized VBO.");
  if (offset + size > m_max) resize(offset + size);
  bind();
  glBufferSubData(m_target, offset, size, data);
  m_size = std::max(offset + size, m_size);
  if (m_logger)
    SPDLOG_LOGGER_DEBUG(m_logger, "Uploaded {} to {} {}, with an offset of {}.", formatMemory(size), getGlEnumName(m_target),
                        m_id, formatMemory(offset));
}

void Vbo::resize(size_t size) {
  assert(m_id && "Cannot resize an uninitialized VBO.");
  assert(size && "Cannot resize to zero.");
  if (m_size == 0) {
    m_size = size;
    bind();
    glBufferData(m_target, m_size, nullptr, m_usage);
    if (m_logger) SPDLOG_LOGGER_DEBUG(m_logger, "Allocated {} for {} {}.", formatMemory(m_size), getGlEnumName(m_target), m_id);
  } else {
    GLuint newId;
    glGenBuffers(1, &newId);
    glBindBuffer(m_target, newId);
    glBufferData(m_target, size, nullptr, m_usage);
    if (m_logger)
      SPDLOG_LOGGER_DEBUG(m_logger, "Resized {} from {} to {}.", getGlEnumName(m_target), formatMemory(m_size),
                          formatMemory(size));
    glBindBuffer(GL_COPY_WRITE_BUFFER, newId);
    glBindBuffer(GL_COPY_READ_BUFFER, m_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_size);
    if (m_logger)
      SPDLOG_LOGGER_DEBUG(m_logger, "Copied {} for {} from {} to {}.", formatMemory(m_size), getGlEnumName(m_target), m_id,
                          newId);
    glDeleteBuffers(1, &m_id);
    m_id = newId;
  }
  m_max = size;
}

void Vbo::unbind() const {
  glBindBuffer(m_target, 0);
}

void Vbo::destroy() {
  glDeleteBuffers(1, &m_id);
  if (m_logger) SPDLOG_LOGGER_TRACE(m_logger, "Destroyed VBO.");
}

}  // namespace uinta
