#ifndef UINTA_VBO_H
#define UINTA_VBO_H

#include <uinta/gl_types.h>

namespace uinta {

	class Vbo {
		GLuint _id = GL_ZERO;
		GLenum _target;
		GLenum _usage;
		GLsizeiptr _size;

	public:
		static Vbo *requestVbo(Vao *vao, GLenum target, GLenum usage, GLsizeiptr size = 0, const void *data = nullptr);

		static void unbind(GLenum target);

	public:
		void bind() const;

		void storeData(const void *data, GLsizeiptr size, GLsizeiptr offset = 0);

		void unbind() const;

		~Vbo();

	private:
		Vbo(GLenum target, GLenum usage, GLsizeiptr size = 0) : _target(target), _usage(usage), _size(size) {}

		void resize(GLsizeiptr size, const void *data = nullptr);

	}; // class Vbo

} // namespace uinta

#endif //UINTA_VBO_H
