#ifndef UINTA_TEXT_CONTROLLER_H
#define UINTA_TEXT_CONTROLLER_H

#include <uinta/controller/controller.h>

namespace uinta {

	class TextController : public Controller {
		static const size_t VERTICES_PER_CHAR = 4;
		static const size_t INDICES_PER_CHAR = 6;

		Font *_font;

		float_t *_vBuffer{};
		size_t _vBufferSize = 0;

		uint32_t *_iBuffer{};
		size_t _iBufferSize = 0;

		size_t _vertexCount = 0;
		size_t _charCount = 0;
		size_t _charCountRenderable = 0;

		void updateVertexCount() {
			_vertexCount = _charCount * VERTICES_PER_CHAR;
		}

		void updateCharCount() {
			_charCount = _text->getValue().size();
			_charCountRenderable = _text->getValue().size() - findWhitespaceCount();
		}

		void updateCounts() {
			updateCharCount();
			updateVertexCount();
		}

		void updateBuffers();

	protected:
		Text *_text;

	public:
		explicit TextController(const Controller *parent, Text &text, Font *font);

		~TextController() {
			delete[] _vBuffer;
			delete[] _iBuffer;
		}

		size_t findWhitespaceCount();

		void initialize() override;

		[[nodiscard]] const Text *getText() const { return _text; }
		[[nodiscard]] float_t *getVertexBuffer() const { return _vBuffer; }
		[[nodiscard]] size_t getCharCount() const { return _charCount; }
		[[nodiscard]] size_t getCharCountRenderable() const { return _charCountRenderable; }
		[[nodiscard]] size_t getIndexBufferSize() const { return _iBufferSize; }
		[[nodiscard]] size_t getVertexBufferSize() const { return _vBufferSize; }
		[[nodiscard]] size_t getVertexCount() const { return _vertexCount; }
		[[nodiscard]] uint32_t *getIndexBuffer() const { return _iBuffer; }

		void setTextAndInit(const char *value) {
			_text->getValue() = value;
			initialize();
		}

	}; // class TextController

}// namespace uinta

#endif //UINTA_TEXT_CONTROLLER_H
