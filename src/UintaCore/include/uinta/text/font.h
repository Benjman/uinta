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
		static const size_t ATLAS_WIDTH = 1024;
		static const size_t ATLAS_HEIGHT = 1024;

		struct STBTTDto {
			stbtt_packedchar charInfo[95];
			stbtt_fontinfo info;
			stbtt_pack_context context;
		};

		friend class FontLoader;

	public:
		static Font loadFont(const char *trueTypePath, size_t fontSize);

	public:
		void getCharWidth(int c, float_t *advance, float_t *leftSideBearing);

		TexturedQuadDto getTexturedQuadInfo(const char c, float_t xCursor, float_t yCursor);

		[[nodiscard]] size_t getLineHeight() const {
			return _lineHeight;
		}

	private:
		Texture _texture{};
		STBTTDto _tt{};

		u_char _atlasData[1024 * 1024]{};
		u_char _fontData[MEGABYTES(1)]{};

		float_t _scale = 1;
		const size_t _lineHeight = 64;

		Font() = default;

	}; // class Font

} // namespace uinta

#endif //UINTA_FONT_H
