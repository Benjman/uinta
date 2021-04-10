#ifndef UINTA_GL_ERROR_H
#define UINTA_GL_ERROR_H

namespace uinta {
	enum gl_error_check_type {
		GL_ACTIVATE_TEXTURE,
		GL_ATTACH_SHADER,
		GL_BIND_BUFFER,
		GL_BIND_TEXTURE,
		GL_BIND_VERTEX_ARRAY,
		GL_BLEND_FUNC,
		GL_BUFFER_DATA,
		GL_BUFFER_SUB_DATA,
		GL_CLEAR,
		GL_COMPILE_SHADER,
		GL_CREATE_PROGRAM,
		GL_CREATE_SHADER,
		GL_DELETE_BUFFERS,
		GL_DELETE_SHADER,
		GL_DELETE_VERTEX_ARRAYS,
		GL_DISABLE_VERTEX_ATTRIB_ARRAY,
		GL_DRAW_ARRAYS,
		GL_DRAW_ELEMENTS,
		GL_ENABLE,
		GL_ENABLE_VERTEX_ATTRIB_ARRAY,
		GL_GEN_BUFFERS,
		GL_GEN_TEXTURES,
		GL_GEN_VERTEX_ARRAYS,
		GL_LINK_PROGRAM,
		GL_SHADER_SOURCE,
		GL_TEX_IMAGE2D,
		GL_TEX_PARAMETERI,
		GL_UNIFORM,
		GL_USE_PROGRAM,
		GL_VERTEX_ATTRIB_POINTER,
	};
}

#ifndef glCheckError
#define glCheckError(m)
#endif // glCheckError

#ifdef UINTA_DEBUG

#undef glCheckError
#define glCheckError(m) uinta::gl::uintaGlCheckError(m)

#include <uinta/gl_types.h>

namespace uinta {

	namespace gl {

		void checkGlActivateTexture(GLenum err);

		void checkGlAttachShader(GLenum err);

		void checkGlBindBuffer(GLenum err);

		void checkGlBindTexture(GLenum err);

		void checkGlBindVertexArray(GLenum err);

		void checkGlBufferData(GLenum err);

		void checkGlBufferSubData(GLenum err);

		void checkGlClear(GLenum err);

		void checkGlCompileShader(GLenum err);

		void checkGlCreateProgram([[maybe_unused]] GLenum err);

		void checkGlCreateShader(GLenum err);

		void checkGlDeleteBuffers(GLenum err);

		void checkGlDeleteShader(GLenum err);

		void checkGlDeleteVertexArrays(GLenum err);

		void checkGlDisableVertexAttribArray(GLenum err);

		void checkGlDrawArrays(GLenum err);

		void checkGlDrawElements(GLenum err);

		void checkGlEnable(GLenum err);

		void checkGlEnableVertexAttribArray(GLenum err);

		void checkGlGenBuffers(GLenum err);

		void checkGlGenVertexArrays(GLenum err);

		void checkGlLinkProgram(GLenum err);

		void checkGlShaderSource(GLenum err);

		void checkGlTexImage2D(GLenum err);

		void checkGlTexParameteri(GLenum err);

		void checkGlUniform(GLenum err);

		void checkGlUseProgram(GLenum err);

		void checkGlVertexAttribPointer(GLenum err);

		extern void uintaGlCheckError(gl_error_check_type type);

	}

}

#endif // UINTA_GL_ERROR_H
#endif // UINTA_DEBUG
