#include <uinta/shader/vbo.h>
#include <uinta/gl.h>

// TODO Need tests. Figure out how to create an OpenGL context within gtest, and cover this class.

uinta::Vbo uinta::Vbo::requestVbo(vbo_target_t target, vbo_usage_t usage, vbo_size_t size, const void *data) {
	Vbo vbo(target, usage);

	glGenBuffers(1, &vbo._id);
	glCheckError(GL_GEN_BUFFERS);

	vbo.bind();
	vbo.storeData(data, size);

	return vbo;
}

void uinta::Vbo::resize(vbo_size_t size) {
	glBufferData(_target, size, nullptr, _usage);
	glCheckError(GL_BUFFER_DATA);
	_size = size;
}

void uinta::Vbo::storeData(const void *data, vbo_size_t size, vbo_size_t offset) {
	if (size > _size) {
		resize(size);
	}
	glBufferSubData(_target, offset, size, data);
	glCheckError(GL_BUFFER_SUB_DATA);
}

void uinta::Vbo::bind() const {
	if (gl_state::getBoundBuffer(_target) == _id) return;
	glBindBuffer(_target, _id);
	glCheckError(GL_BIND_BUFFER);
	gl_state::setBoundBuffer(_target, _id);
}
