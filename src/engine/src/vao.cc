#include "uinta/vao.h"

#include "uinta/gl.h"
#include "uinta/vbo.h"

namespace uinta {

Vao::Vao(const OpenGLApi* gl) noexcept : gl_(gl) {
  gl_->genVertexArrays(1, &id_);
}

Vao::~Vao() noexcept {
  gl_->deleteVertexArrays(1, &id_);
  id_ = 0;
}

Vao::Vao(Vao&& other) noexcept
    : id_(other.id_),
      indexBuffer_(std::move(other.indexBuffer_)),
      gl_(other.gl_) {
  other.id_ = 0;
  other.gl_ = nullptr;
}

Vao& Vao::operator=(Vao&& other) noexcept {
  if (this != &other) {
    gl_->deleteVertexArrays(1, &id_);

    id_ = other.id_;
    indexBuffer_ = std::move(other.indexBuffer_);
    gl_ = other.gl_;

    other.id_ = 0;
    other.gl_ = nullptr;
  }
  return *this;
}

void Vao::linkAttribute(const Attribute& attrib) const noexcept {
  VaoGuard vaoGuard(this);
  VboGuard vboGuard(attrib.vbo);
  gl_->vertexAttribPointer(attrib.index, attrib.size, attrib.type,
                           attrib.normalized, attrib.stride, attrib.pointer);
  gl_->enableVertexAttribArray(attrib.index);
}

void Vao::indexBuffer(const std::span<const u32> data) noexcept {
  indexBuffer(data.data(), data.size());
}

void Vao::indexBuffer(const u32* data, u32 count) noexcept {
  VaoGuard guard(this);
  indexBuffer_ = std::make_unique<Vbo>(GL_ELEMENT_ARRAY_BUFFER);
  gl_->bindBuffer(indexBuffer_->target(), indexBuffer_->id());
  gl_->bufferData(indexBuffer_->target(), count * sizeof(u32), data,
                  GL_STATIC_DRAW);
  indexBuffer_->size_ = count;
  gl_->bindBuffer(indexBuffer_->target(), 0);
}

VaoGuard::VaoGuard(const Vao* vao) noexcept : vao_(vao) {
  vao_->gl_->bindVertexArray(vao_->id());
  if (vao_->indexBuffer())
    vao_->gl_->bindBuffer(vao_->indexBuffer()->target(),
                          vao_->indexBuffer()->id());
}

VaoGuard::~VaoGuard() noexcept {
  vao_->gl_->bindVertexArray(0);
  if (vao_->indexBuffer())
    vao_->gl_->bindBuffer(vao_->indexBuffer()->target(), 0);
}

}  // namespace uinta
