#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <uinta/types.h>
#include <uinta/texture/texture.h>

#include <stb_truetype.h>

namespace uinta {

	class File;
	class Font;
	class Texture;

	using TexturedQuadDto = stbtt_aligned_quad;

	class Font {
		static const size_t ATLAS_WIDTH = 512;
		static const size_t ATLAS_HEIGHT = 512;

		struct STBTTDto {
			stbtt_packedchar charInfo[95];
			stbtt_fontinfo info;
			stbtt_pack_context context;
		};

		friend class FontLoader;

	public:
		static Font loadFont(const char *trueTypePath);

	public:
		TexturedQuadDto getTexturedQuadInfo(char c, float_t *xCursor, float_t *yCursor);

		[[nodiscard]] size_t getLineHeight() const {
			return _lineHeight;
		}

	private:
		Texture _texture{};
		STBTTDto _tt{};

		u_char _atlasData[ATLAS_WIDTH * ATLAS_HEIGHT]{};
		u_char _fontData[MEGABYTES(1)]{};

		float_t _scale = 1;
		const size_t _lineHeight = 32;

		Font() = default;

	}; // class Font

} // namespace uinta

#endif //UINTA_FONT_H
