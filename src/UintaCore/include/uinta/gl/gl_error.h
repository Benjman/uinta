#ifndef UINTA_GL_ERROR_H
#define UINTA_GL_ERROR_H

namespace uinta {

	enum gl_error_check_type {
		VBO,
		GenBuffer,
		Shader
	};

	void checkGlErrors(gl_error_check_type type);

}

#endif //UINTA_GL_ERROR_H
