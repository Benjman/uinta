#include <uinta/shader/vertex_attribute.h>
#include <uinta/shader/vao.h>
#include <uinta/gl.h>

uinta::VertexAttribute::VertexAttribute(Vao *parent, attrib_index_t index, attrib_size_t size, gl_type_t type,
										attrib_normalize_t normalized, attrib_stride_t stride, const void *offset,
										bool enabled)
		: _parent(parent),
		  _index(index),
		  _enabled(enabled) {
	if (_parent != nullptr) {
		_parent->bind();
	}
	glVertexAttribPointer(_index, size, type, normalized, stride, (void *) offset);
	glCheckError(GL_VERTEX_ATTRIB_POINTER);
	if (enabled) {
		enable(true);
	}
	if (parent != nullptr) {
		parent->addAttribute(this);
	}
}

void uinta::VertexAttribute::enable(bool force) {
	if (_enabled && !force) return;
	glEnableVertexAttribArray(_index);
	glCheckError(GL_ENABLE_VERTEX_ATTRIB_ARRAY);
	_enabled = true;
}

void uinta::VertexAttribute::disable(bool force) {
	if (!_enabled && !force) return;
	glDisableVertexAttribArray(_index);
	glCheckError(GL_DISABLE_VERTEX_ATTRIB_ARRAY);
	_enabled = false;
}

uinta::VertexAttribute::~VertexAttribute() {
	_parent->removeAttribute(this);
}
