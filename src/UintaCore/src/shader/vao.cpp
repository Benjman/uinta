#include <uinta/shader/vao.h>
#include <uinta/shader/vertex_attribute.h>
#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_state.h>

#include <algorithm>

uinta::Vao uinta::Vao::requestVao() {
	Vao vao;

	glGenVertexArrays(1, &vao._id);
	glCheckError(GL_GEN_VERTEX_ARRAYS);

	vao.bind();
	return vao;
}

uinta::Vao::~Vao() {
	glDeleteVertexArrays(1, &_id);
	glCheckError(GL_DELETE_VERTEX_ARRAYS);
}

void uinta::Vao::bind() const {
//	if (gl_state::isNotBoundVertexArrayElseSet(_id)) {
	glBindVertexArray(_id);
	glCheckError(GL_BIND_VERTEX_ARRAY);
//	}
}

void uinta::Vao::unbind() {
	if (gl_state::isNotBoundVertexArrayElseSet(0)) {
		glBindVertexArray(0);
		// No need fo GL error checking. Binding 0 is permitted.
	}
}

uinta::VertexAttribute uinta::Vao::createAttribute(attrib_index_t index, attrib_size_t size, gl_type_t type,
												   attrib_normalize_t normalized, attrib_stride_t stride,
												   const void *offset) {
	VertexAttribute attrib(this, index, size, type, normalized, stride, offset);
	_attributes.emplace_back(&attrib);
	return attrib;
}

void uinta::Vao::enableAllAttributes() {
	for (auto attribute : _attributes) {
		attribute->enable();
	}
}

void uinta::Vao::removeAttribute(VertexAttribute *attribute) {
	auto element = std::find(_attributes.begin(), _attributes.end(), attribute);
	if (element != _attributes.end()) {
		attribute->disable();
		_attributes.erase(element);
	}
}
