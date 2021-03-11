#include <uinta/shader/vertex_attribute.h>
#include <uinta/gl.h>

uinta::VertexAttribute::VertexAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type,
										attrib_normalize_t normalized, attrib_stride_t stride, const void *offset,
										bool enabled)
		: _index(index),
		  _size(size),
		  _type(type),
		  _normalized(normalized),
		  _stride(stride),
		  _offset(offset),
		  _enabled(enabled) {
	glVertexAttribPointer(_index, _size, _type, _normalized, _stride, _offset);
	checkGlError(GL_VERTEX_ATTRIB_POINTER);
	if (enabled) {
		enable();
	}
}

void uinta::VertexAttribute::disable() const {
	glDisableVertexAttribArray(_index);
	checkGlError(GL_DISABLE_VERTEX_ATTRIB_ARRAY);
}

void uinta::VertexAttribute::enable() const {
	glEnableVertexAttribArray(_index);
	checkGlError(GL_ENABLE_VERTEX_ATTRIB_ARRAY);
}
