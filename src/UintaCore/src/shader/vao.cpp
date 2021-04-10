#include <uinta/shader/vao.h>
#include <uinta/shader/vertex_attribute.h>
#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_state.h>

#include <algorithm>

using namespace uinta;
using namespace gl_state;

Vao *Vao::requestVao() {
	return new Vao;
}

Vao::Vao() {
	glGenVertexArrays(1, &_id);
	glCheckError(GL_GEN_VERTEX_ARRAYS);
	bind();
}

void Vao::bind() const {
	if (!isActiveElseSet(BOUND_VERTEX_ARRAY, _id)) {
		glBindVertexArray(_id);
		glCheckError(GL_BIND_VERTEX_ARRAY);
	}
}

void Vao::unbind() {
	if (!isActiveElseSet(BOUND_VERTEX_ARRAY, 0)) {
		glBindVertexArray(0);
		// No need fo GL error checking. Binding 0 is always permitted.
	}
}

VertexAttribute *Vao::createAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
									  const void *offset) {
	bind();
	auto *attribute = new VertexAttribute(index, size, type, normalized, stride, offset);
	_attributes.emplace_back(attribute);
	return attribute;
}

void Vao::disableAllAttributes() {
	for (auto attribute : _attributes) {
		attribute->disable();
	}
}

void Vao::enableAllAttributes() {
	for (auto attribute : _attributes) {
		attribute->enable();
	}
}

void Vao::removeAttribute(VertexAttribute *attribute) {
	auto element = std::find(_attributes.begin(), _attributes.end(), attribute);
	if (element != _attributes.end()) {
		attribute->disable();
		_attributes.erase(element);
		// TODO getWhitespaceCount to see if attribute is deallocated
	}
}

Vao::~Vao() {
	glDeleteVertexArrays(1, &_id);
	glCheckError(GL_DELETE_VERTEX_ARRAYS);

	for (auto attribute : _attributes) {
		attribute->disable();
		delete attribute;
	}
}
