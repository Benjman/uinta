#ifndef UINTA_TEXT_H
#define UINTA_TEXT_H

#include <uinta/types.h>
#include <uinta/ui/ui.h>
#include <uinta/ui/ui_element.h>

#include <string>

namespace uinta {

	class Font;

	class Text : public UiElement {
		friend class TextMeshGenerator;

		Font *_font;
		std::string _value;

	public:
		static const size_t VERTICES_PER_CHAR = 8;
		static const size_t INDICES_PER_CHAR = 6;

		Text(const char *value, Font *font, UiElement *parent = nullptr) :
				UiElement(parent, 0, 0, 0, 0, 0),
				_value(value), _font(font) {}

		[[nodiscard]] Font *getFont() const {
			return _font;
		}

		void generateMesh(float_t *data, uint32_t *indices, size_t *vertexCount, size_t *indexCount);

		static void generateMesh(Text &text, float_t *data, uint32_t *indices, size_t *vertexCount, size_t *indexCount);

		size_t getNonSpaceCharacterCount();

		static size_t getNonSpaceCharacterCount(Text &text);

	};

} // namespace uinta

#endif //UINTA_TEXT_H
