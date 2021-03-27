#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <uinta/types.h>
#include <uinta/texture/texture.h>

#include <stb_truetype.h>

namespace uinta {

	class File;
	class Font;
	class Texture;

	class Font {
		static const size_t ATLAS_WIDTH = 512;
		static const size_t ATLAS_HEIGHT = 512;

		friend class FontLoader;
		friend class TextMeshGenerator;

	public:
		static const size_t LINE_HEIGHT = 32;

		static Font loadFont(const char *trueTypePath);

	public:
		stbtt_aligned_quad getTexturedQuadInfo(char c, float_t *xCursor, float_t *yCursor);

	private:
		Texture _texture{};

		stbtt_fontinfo _stbttFontInfo{};
		stbtt_bakedchar _stbttBakedChar[96]{};

		float_t _ascent = 0, _descent = 0, _lineGap = 0;

		u_char _atlasData[ATLAS_WIDTH * ATLAS_HEIGHT]{};
		u_char _fontData[MEGABYTES(1)]{};

		float_t _scale = 1;

		Font() = default;

	}; // class Font

} // namespace uinta

#endif //UINTA_FONT_H
