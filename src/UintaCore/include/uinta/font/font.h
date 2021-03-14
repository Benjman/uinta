#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <uinta/types.h>

namespace uinta {

	class Font {
		friend class FontLoader;

	public:
		uint32_t _textureId;
		static Font loadFont(const char *trueTypePath);

	private:
		u_char atlasData[1024*1024]{};
		u_char fontData[1048576]{};

		Font() = default;

	}; // class Font

} // namespace uinta

#endif //UINTA_FONT_H
