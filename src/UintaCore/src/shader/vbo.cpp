#include <uinta/shader/vao.h>
#include <uinta/shader/vbo.h>
#include <uinta/gl.h>

// TODO Need tests. Figure out how to create an OpenGL context within gtest, and cover this class.

using namespace uinta::gl_state;

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
	// TODO this needs to copy current contents to new buffer (on GPU side) https://stackoverflow.com/a/28056825
	bind();
	glBufferData(_target, size, data, _usage);
	glCheckError(GL_BUFFER_DATA);
	_size = size;
}

void uinta::Vbo::storeData(const void *data, vbo_size_t size, vbo_size_t offset) {
	if (size > _size || _size == 0) {
		resize(size, data);
	} else {
		bind();
		glBufferSubData(_target, offset, size, data);
		glCheckError(GL_BUFFER_SUB_DATA);
	}
}

void uinta::Vbo::bind() const {
	if (!isActiveElseSet(BOUND_BUFFER, _target, _id)) {
		glBindBuffer(_target, _id);
		glCheckError(GL_BIND_BUFFER);
	}
}

void uinta::Vbo::unbind(uinta::vbo_target_t target) {
	if (!isActiveElseSet(BOUND_BUFFER, target, 0)) {
		glBindBuffer(target, 0);
		// No need fo GL error checking. Binding 0 is always permitted.
	}
}

void uinta::Vbo::unbind() const {
	Vbo::unbind(_target);
}
