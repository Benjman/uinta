#include <uinta/shader/vao.h>
#include <uinta/shader/vbo.h>
#include <uinta/gl.h>

// TODO Need tests. Figure out how to create an OpenGL context within gtest, and cover this class.

uinta::Vbo::~Vbo() {
	glDeleteBuffers(1, &_id);
	glCheckError(GL_DELETE_BUFFERS);
}

uinta::Vbo uinta::Vbo::requestVbo(Vao *vao, vbo_target_t target, vbo_usage_t usage, vbo_size_t size, const void *data) {
	Vbo vbo(target, usage);

	glGenBuffers(1, &vbo._id);
	glCheckError(GL_GEN_BUFFERS);

	if (vao != nullptr) {
		vao->bind();
	}

	vbo.bind();
	vbo.storeData(data, size);

	return vbo;
}

void uinta::Vbo::resize(vbo_size_t size, const void *data) {
	glBufferData(_target, size, data, _usage);
	glCheckError(GL_BUFFER_DATA);
	_size = size;
}

void uinta::Vbo::storeData(const void *data, vbo_size_t size, vbo_size_t offset) {
	if (size > _size) {
		resize(size, data);
	} else {
		glBufferSubData(_target, offset, size, data);
		glCheckError(GL_BUFFER_SUB_DATA);
	}
}

void uinta::Vbo::bind() const {
	if (isNotBoundBufferElseSet(_target, _id)) {
		glBindBuffer(_target, _id);
		glCheckError(GL_BIND_BUFFER);
	}
}

void uinta::Vbo::unbind(uinta::vbo_target_t target) {
	if (isNotBoundBufferElseSet(target, 0)) {
		glBindBuffer(target, 0);
		glCheckError(GL_BIND_BUFFER);
	}
}

void uinta::Vbo::unbind() const {
	Vbo::unbind(_target);
}
