#ifndef UINTA_VAO_H
#define UINTA_VAO_H

#include <uinta/gl_types.h>

#include <vector>

namespace uinta {

	class VertexAttribute;

	class Vao {
		friend class VertexAttribute;

		using AttributeCollection = std::vector<VertexAttribute *>;

	public:
		static void unbind();

		static Vao *requestVao();

	public:
		~Vao();

		void bind() const;

		VertexAttribute *createAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
										 const void *offset);

		void enableAllAttributes();

		void disableAllAttributes();

	private:
		AttributeCollection _attributes;
		GLuint _id = GL_ZERO;

		Vao();

		void removeAttribute(VertexAttribute *pAttribute);

	}; // class Vao

} // namespace uinta

#endif //UINTA_VAO_H
