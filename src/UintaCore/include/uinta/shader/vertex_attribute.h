#ifndef UINTA_VERTEX_ATTRIBUTE_H
#define UINTA_VERTEX_ATTRIBUTE_H

#include <uinta/gl_types.h>
#include "vao.h"


namespace uinta {

	class VertexAttribute {
		friend class Vao;

		GLuint _index;
		bool _enabled;

	public:
		void enable(bool force = false);

		void disable(bool force = false);

	private:
		VertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
						const void *offset, bool enable = true);

	}; // class VertexAttribute

} // namespace uinta

#endif //UINTA_VERTEX_ATTRIBUTE_H
