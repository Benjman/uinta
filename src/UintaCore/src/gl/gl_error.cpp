#include <uinta/gl/gl_error.h>

#include <GLES3/gl3.h>

#include <iostream> // TODO remove when logging is implemented

using namespace uinta;

bool checkGenBuffersError(gl_error_check_type type) {
	if (type != GenBuffer) return false;

	GLenum err = glGetError();

	if (!err) return true;

	// TODO for glGenBuffers
	return true;
}

bool checkVboErrors(gl_error_check_type type) {
	if (type != VBO) return false;

	// VBO error reference: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glBufferSubData.xhtml
	GLenum err = glGetError();

	if (!err) return true;

	switch (err) {

		case GL_INVALID_ENUM:
			// TODO error logging
			std::cerr << "GL_INVALID_ENUM: Unacceptable VBO target type." << std::endl;
			return true;

		case GL_INVALID_VALUE:
			// TODO error logging
			std::cerr
					<< "GL_INVALID_VALUE: VBO offset or size is negative, or together they define a region of memory that extends beyond the buffer object's allocated data store."
					<< std::endl;
			return true;

		case GL_INVALID_OPERATION:
			// TODO error logging
			std::cerr
					<< "GL_INVALID_OPERATION: VBO operation invalid because either;\n\t1) The reserved buffer object name O is bound to target. Or,\n\t2) The buffer object being updated is mapped."
					<< std::endl;
			return true;

		default:
			std::cerr << "Unknown VBO error." << std::endl;
			return true;

	}
}

bool checkShaderErrors(gl_error_check_type type) {
	if (type != Shader) return false;

	GLenum err = glGetError();

	if (!err) return true;

	// TODO
	return true;
}

void uinta::checkGlErrors(gl_error_check_type type) {
#ifdef UINTA_DEBUG
	if (
			checkGenBuffersError(type) ||
			checkShaderErrors(type) ||
			checkVboErrors(type)
	) return;
#endif
}
