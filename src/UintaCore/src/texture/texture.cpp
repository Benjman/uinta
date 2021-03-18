#include <uinta/texture/texture.h>

#include <uinta/gl.h>

uinta::Texture uinta::Texture::requestTexture(const uint32_t width, const uint32_t height, GLint internalFormat, GLenum type, const void *data) {
	Texture texture;

	glGenTextures(0, &texture._id);
	glCheckError(GL_GEN_TEXTURES);

	texture.bind();
	texture.upload(width, height, internalFormat, type, data);

	return texture;
}

void uinta::Texture::upload(uint32_t width, uint32_t height, GLint internalFormat, GLenum type, const void *data) {
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, type, data);
	glCheckError(GL_TEX_IMAGE2D);
}

void uinta::Texture::bind() const {
	if (!gl_state::isActiveElseSet(gl_state::BOUND_TEXTURE_2D, _id)) {
		glBindTexture(GL_TEXTURE_2D, _id);
		glCheckError(GL_BIND_TEXTURE);
	}
}

void uinta::Texture::setParameter(GLenum name, GLint param) {
	glTexParameteri(GL_TEXTURE_2D, name, param);
	glCheckError(GL_TEX_PARAMETERI);
}

void uinta::Texture::linear() const {
	bind();
	setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
