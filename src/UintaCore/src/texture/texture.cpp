#include <uinta/texture/texture.h>

#include <uinta/gl.h>

namespace uinta {
	using namespace gl_state;

	Texture *Texture::requestTexture(const uint32_t width, const uint32_t height, GLint internalFormat, GLenum type,
									 const void *data) {
		auto texture = new Texture;

		glGenTextures(1, &texture->_id);
		glCheckError(GL_GEN_TEXTURES);

		texture->bind();
		texture->upload(width, height, internalFormat, type, data);

		return texture;
	}

	void Texture::upload(uint32_t width, uint32_t height, GLint internalFormat, GLenum type, const void *data) const {
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, type, data);
		glCheckError(GL_TEX_IMAGE2D);
	}

	void Texture::bind() const {
		if (!isActiveElseSet(BOUND_TEXTURE, GL_TEXTURE_2D, _id)) {
			glBindTexture(GL_TEXTURE_2D, _id);
			glCheckError(GL_BIND_TEXTURE);
		}
	}

	void Texture::setParameter(GLenum name, GLint param) {
		glTexParameteri(GL_TEXTURE_2D, name, param);
		glCheckError(GL_TEX_PARAMETERI);
	}

	void Texture::linear() const {
		bind();
		setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &_id);
	}

}
