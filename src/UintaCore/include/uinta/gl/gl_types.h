#ifndef UINTA_GL_TYPES_H
#define UINTA_GL_TYPES_H

#include <GLES3/gl3.h>

namespace uinta {

	using gl_type_t = GLenum;

	using location_id_t = GLint;
	using program_id_t = GLuint;

	// VBO
	using vbo_id_t = GLuint;
	using vbo_target_t = GLenum;
	using vbo_usage_t = GLenum;
	using vbo_size_t = GLsizeiptr;

	// VAO
	using vao_id_t = GLuint;

	// Vertex Attribute
	using attrib_index_t = GLuint;
	using attrib_normalize_t = GLboolean;
	using attrib_stride_t = GLsizei;
	using attrib_size_t = GLint;




	static inline const location_id_t INVALID_VBO_ID = 0;

}

#endif //UINTA_GL_TYPES_H
