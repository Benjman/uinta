#ifndef UINTA_TEXTURE_H
#define UINTA_TEXTURE_H

#include <uinta/types.h>
#include <uinta/gl/gl_types.h>

namespace uinta {

	class Texture {
		texture_id_t _id = GL_ZERO;

	public:
		static Texture requestTexture(uint32_t width, uint32_t height, GLint internalFormat, GLenum type, const void *data);

		static void setParameter(GLenum name, GLint param);

	public:
		void upload(uint32_t width, uint32_t height, GLint internalFormat, GLenum type, const void *data);

		void bind() const;

		void linear() const;

	private:
		Texture() = default;
	};

}

#endif //UINTA_TEXTURE_H
