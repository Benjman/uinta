#ifndef UINTA_FONT_LOADER_H
#define UINTA_FONT_LOADER_H

#include <uinta/text/font.h>
#include <uinta/gl.h>

#include <iostream>

#ifndef UINTA_TRUETYPE_LOADED

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <stb_image_write.h>

#undef STB_TRUETYPE_IMPLEMENTATION
#undef STB_RECT_PACK_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#define UINTA_TRUETYPE_LOADED 1
#endif // UINTA_TRUETYPE_LOADED

namespace uinta {

	struct FontLoader {
		static Font *loadFond(const char *trueTypePath) {
			auto font = new Font;

			FILE *file = fopen(trueTypePath, "rb");
			if (!file) {
				std::cerr << "TrueType file at path `" << trueTypePath << "` not found." << std::endl;
				return font;
			}
			fread(font->_fontData, 1, MEGABYTES(1), file);
			fclose(file);

			int32_t ascent, descent, lineGap;

			stbtt_InitFont_internal(&font->_stbttFontInfo, font->_fontData, 0);
			stbtt_BakeFontBitmap(font->_fontData, 0, Font::LINE_HEIGHT, font->_atlasData, Font::ATLAS_WIDTH,
								 Font::ATLAS_HEIGHT, 32, 96, font->_stbttBakedChar);
			stbtt_GetFontVMetrics(&font->_stbttFontInfo, &ascent, &descent, &lineGap);

			font->_scale = stbtt_ScaleForPixelHeight(&font->_stbttFontInfo, Font::LINE_HEIGHT);

			font->_texture = Texture::requestTexture(Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, GL_RED,
												   GL_UNSIGNED_BYTE, font->_atlasData);
			font->_texture->linear();
			glActiveTexture(GL_TEXTURE0); // TODO feels like this should be in the Texture class
			glCheckError(GL_ACTIVATE_TEXTURE);


			// Write the atlas to an image file for debugging purposes
//			stbi_write_jpg("getWhitespaceCount.jpg", Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, 1, font._atlasData, 100);

			return font;
		}
	};

}

#endif //UINTA_FONT_LOADER_H
