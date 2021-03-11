#ifndef UINTA_GL_ERROR_H
#define UINTA_GL_ERROR_H

namespace uinta {

	enum gl_error_check_type {
		GL_BIND_BUFFER,
		GL_BUFFER_DATA,
		GL_BUFFER_SUB_DATA,
		GL_GEN_BUFFERS,
		GL_VERTEX_ATTRIB_POINTER,
	};

	void checkGlError(gl_error_check_type type);

}

#endif //UINTA_GL_ERROR_H
