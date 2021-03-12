#ifndef UINTA_GL_ERROR_H
#define UINTA_GL_ERROR_H

namespace uinta {

	enum gl_error_check_type {
		GL_BIND_BUFFER,
		GL_BUFFER_DATA,
		GL_BUFFER_SUB_DATA,
		GL_DISABLE_VERTEX_ATTRIB_ARRAY,
		GL_ENABLE_VERTEX_ATTRIB_ARRAY,
		GL_GEN_BUFFERS,
		GL_VERTEX_ATTRIB_POINTER,
		GL_CLEAR,
		GL_CREATE_SHADER,
		GL_USE_PROGRAM,
		GL_DRAW_ARRAYS,
		GL_SHADER_SOURCE,
		GL_COMPILE_SHADER,
		GL_CREATE_PROGRAM,
		GL_ATTACH_SHADER,
		GL_LINK_PROGRAM,
		GL_DELETE_SHADER,
		GL_DELETE_BUFFERS,
		GL_GEN_VERTEX_ARRAYS,
		GL_DELETE_VERTEX_ARRAYS,
		GL_BIND_VERTEX_ARRAY,
	};

	extern void glCheckError(gl_error_check_type type);

}

#endif //UINTA_GL_ERROR_H
