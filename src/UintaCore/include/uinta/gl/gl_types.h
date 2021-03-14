#ifndef UINTA_GL_TYPES_H
#define UINTA_GL_TYPES_H

#include <GLES3/gl3.h>

namespace uinta {

	using attrib_index_t = GLuint;
	using attrib_normalize_t = GLboolean;
	using attrib_size_t = GLint;
	using attrib_stride_t = GLsizei;
	using gl_type_t = GLenum;
	using location_id_t = GLint;
	using program_id_t = GLuint;
	using texture_id_t = GLuint;
	using vao_id_t = GLuint;
	using vbo_id_t = GLuint;
	using vbo_size_t = GLsizeiptr;
	using vbo_target_t = GLenum;
	using vbo_usage_t = GLenum;

	static inline const location_id_t INVALID_VBO_ID = 0;

}

#endif //UINTA_GL_TYPES_H
