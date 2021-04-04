#ifndef UINTA_TEXT_CONTROLLER_H
#define UINTA_TEXT_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>

#include <uinta/gl/gl_types.h>

namespace uinta {

	class Mesh;

	class TextController : public Controller, public IRenderable {
	protected:
		Text *_text;
		Font *_font;
		Mesh *_mesh;

		size_t _charCount = 0;
		size_t _maxChars = 0;

	public:
		static const size_t VERTICES_PER_CHAR = 4;
		static const size_t INDICES_PER_CHAR = 6;
		static const size_t ELEMENTS_PER_VERTEX = 4; // vec2 position, vec2 uv // TODO add color

		explicit TextController(BufferController *parent, Text &text, Font *font);

		~TextController();

		void initialize() override;

		[[nodiscard]] Mesh *getMesh() const { return _mesh; }
		[[nodiscard]] size_t getCharCount() const { return _charCount; }
		[[nodiscard]] size_t getIBufferLen() const { return _maxChars * INDICES_PER_CHAR; }
		[[nodiscard]] size_t getIBufferSize() const { return getIBufferLen() * sizeof(GLuint); }
		[[nodiscard]] size_t getICount() const { return _charCount * INDICES_PER_CHAR; }
		[[nodiscard]] size_t getMaxChars() const { return _maxChars; }
		[[nodiscard]] size_t getMaxIdxCount() const { return _maxChars * VERTICES_PER_CHAR; }
		[[nodiscard]] size_t getVBufferLen() const { return _maxChars * VERTICES_PER_CHAR * ELEMENTS_PER_VERTEX; }
		[[nodiscard]] size_t getVBufferSize() const { return getVBufferLen() * sizeof(GLfloat); }

		void setValue(const char *value, bool updateMetadata = true) {
			_text->_value = value;
			if (updateMetadata) {
				doUpdateMetadata();
			}
		}

		void generateMesh(GLfloat *vBuffer, GLuint *iBuffer, size_t iOffset = 0) const;

		void doUpdateMetadata();

		void render() override;

	}; // class TextController

}// namespace uinta

#endif //UINTA_TEXT_CONTROLLER_H
