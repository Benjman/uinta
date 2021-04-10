#include <uinta/shader/vertex_attribute.h>
#include <uinta/gl.h>

uinta::VertexAttribute::VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
										const void *offset, bool enabled)
		: _index(index),
		  _enabled(enabled) {
	glVertexAttribPointer(_index, size, type, normalized, stride, offset);
	glCheckError(GL_VERTEX_ATTRIB_POINTER);
	if (enabled) {
		enable(true);
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
