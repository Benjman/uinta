#ifndef UINTA_TEXT_CONTROLLER_H
#define UINTA_TEXT_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>

#include <uinta/gl/gl_types.h>

namespace uinta {

	class TextController : public Controller {
	public:
		static const size_t VERTICES_PER_CHAR = 4;
		static const size_t INDICES_PER_CHAR = 6;
		static const size_t ELEMENTS_PER_VERTEX = 4; // vec2 position, vec2 uv

		Text *_text;
		Font *_font;

		size_t _charCount = 0;
		size_t _maxChars = 0;

		size_t _vOffset = 0;
		size_t _iOffset = 0;
		size_t _idxOffset = 0;

		explicit TextController(const BufferController *parent, Text &text, Font *font);

		void initialize() override;

		[[nodiscard]] const Font *getFont() const { return _font; }
		[[nodiscard]] const Text *getText() const { return _text; }
		[[nodiscard]] size_t getCharCount() const { return _charCount; }
		[[nodiscard]] size_t getIdxCount() const { return _maxChars * VERTICES_PER_CHAR; }
		[[nodiscard]] size_t getIBufferLen() const { return _maxChars * INDICES_PER_CHAR; }
		[[nodiscard]] size_t getIBufferSize() const { return getIBufferLen() * sizeof(GLuint); }
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
