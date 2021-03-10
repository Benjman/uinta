#include <uinta/shader/vbo.h>
#include <uinta/gl.h>

#include <iostream> // TODO remove when logging is implemented

// TODO Need tests. Figure out how to create an OpenGL context within gtest, and cover this class.

uinta::Vbo uinta::Vbo::requestVbo(vbo_target_t target, vbo_usage_t usage, vbo_size_t size, const void *data) {
	Vbo vbo(target, usage);

	glGenBuffers(1, &vbo._id);
	if (vbo._id == INVALID_VBO_ID) {
		// TODO error logging
		std::cerr << "Invalid OpenGL buffer ID returned for VBO." << std::endl;
		return vbo;
	}
	checkGlErrors();

	vbo.bind();
	vbo.resize(size);
	vbo.storeData(data, size);

	return vbo;
}

void uinta::Vbo::checkGlErrors() {
	// TODO do this kind of error checking in the shader loader

#ifdef UINTA_DEBUG
	// VBO error reference: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBufferSubData.xhtml
	GLenum err = glGetError();

	if (!err) return;

	switch (err) {

		case GL_INVALID_ENUM:
			// TODO error logging
			std::cerr << "GL_INVALID_ENUM: Unacceptable VBO target type." << std::endl;
			return;

		case GL_INVALID_VALUE:
			// TODO error logging
			std::cerr << "GL_INVALID_VALUE: VBO offset or size is negative, or together they define a region of memory that extends beyond the buffer object's allocated data store." << std::endl;
			return;

		case GL_INVALID_OPERATION:
			// TODO error logging
			std::cerr << "GL_INVALID_OPERATION: VBO operation invalid because either;\n\t1) The reserved buffer object name O is bound to target. Or,\n\t2) The buffer object being updated is mapped." << std::endl;
			return;

		default:
			std::cerr << "Unknown VBO error." << std::endl;

	}
#endif

}

void uinta::Vbo::resize(vbo_size_t size) {
	glBufferData(_target, size, _data, _usage);
	checkGlErrors();
	_size = size;
}

void uinta::Vbo::storeData(const void *data, vbo_size_t size, vbo_size_t offset) {
	if (size > _size) {
		resize(size);
	}
	glBufferSubData(_target, offset, _size, data);
	checkGlErrors();
	_data = data;
}

void uinta::Vbo::bind() const {
	if (gl_state::getBoundBuffer(_target) == _id) return;
	glBindBuffer(_target, _id);
	checkGlErrors();
	gl_state::setBoundBuffer(_target, _id);
}
