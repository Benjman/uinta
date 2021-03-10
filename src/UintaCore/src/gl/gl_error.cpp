#include <uinta/gl/gl_error.h>

#include <GLES3/gl3.h>

#include <iostream> // TODO remove when logging is implemented

using namespace uinta;

void checkGenBuffersError(GLenum err) {
	// TODO for glGenBuffers
}

void checkVboError(GLenum err) {
	switch (err) {

		case GL_INVALID_ENUM:
			// TODO error logging
			std::cerr << "GL_INVALID_ENUM: Unacceptable VBO target type." << std::endl;
			return;

		case GL_INVALID_VALUE:
			// TODO error logging
			std::cerr
					<< "GL_INVALID_VALUE: VBO offset or size is negative, or together they define a region of memory that extends beyond the buffer object's allocated data store."
					<< std::endl;
			return;

		case GL_INVALID_OPERATION:
			// TODO error logging
			std::cerr
					<< "GL_INVALID_OPERATION: VBO operation invalid because either;\n\t1) The reserved buffer object name O is bound to target. Or,\n\t2) The buffer object being updated is mapped."
					<< std::endl;
			return;

		default:
			std::cerr << "Unknown VBO error." << std::endl;
			return;

	}
}

void checkShaderError(GLenum err) {
	// TODO for shader_loader
}

void uinta::checkGlError(gl_error_check_type type) {
#ifdef UINTA_DEBUG
	GLenum err = glGetError();
	if (!err) return;

	switch (type) {
		case GenBuffer:
			checkGenBuffersError(err);
			return;
		case Shader:
			checkShaderError(err);
			return;
		case VBO:
			checkVboError(err);
			return;
	}
#endif
}
