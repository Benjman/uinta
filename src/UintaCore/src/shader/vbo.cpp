#include <uinta/shader/vao.h>
#include <uinta/shader/vbo.h>
#include <uinta/gl.h>

// TODO Need tests. Figure out how to create an OpenGL context within gtest, and cover this class.
// TODO Pool VBOs, then return potentially released VBO in `requestVbo()`.

using namespace uinta;
using namespace uinta::gl_state;

Vbo::~Vbo() {
	glDeleteBuffers(1, &_id);
	glCheckError(GL_DELETE_BUFFERS);
}

Vbo *Vbo::requestVbo(Vao *vao, vbo_target_t target, vbo_usage_t usage, vbo_size_t size, const void *data) {
	auto vbo = new Vbo(target, usage);

	glGenBuffers(1, &vbo->_id);
	glCheckError(GL_GEN_BUFFERS);

	if (vao != nullptr) {
		vao->bind();
	}

	vbo->bind();
	vbo->storeData(data, size);

	return vbo;
}

void Vbo::resize(vbo_size_t size, const void *data) {
	// TODO this needs to copy current contents to new buffer (pref on the gpu only) https://stackoverflow.com/a/28056825
	bind();
	glBufferData(_target, size, data, _usage);
	glCheckError(GL_BUFFER_DATA);
	_size = size;
}

void Vbo::storeData(const void *data, vbo_size_t size, vbo_size_t offset) {
	if (size > _size || _size == 0) {
		resize(size, data);
	} else {
		bind();
		glBufferSubData(_target, offset, size, data);
		glCheckError(GL_BUFFER_SUB_DATA);
	}
}

void Vbo::bind() const {
	if (!isActiveElseSet(BOUND_BUFFER, _target, _id)) {
		glBindBuffer(_target, _id);
		glCheckError(GL_BIND_BUFFER);
	}
}

void Vbo::unbind(vbo_target_t target) {
	if (!isActiveElseSet(BOUND_BUFFER, target, 0)) {
		glBindBuffer(target, 0);
		// No need fo GL error checking. Binding 0 is always permitted.
	}
}

void Vbo::unbind() const {
	Vbo::unbind(_target);
}
