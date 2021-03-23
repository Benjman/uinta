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
		static Font loadFond(const char *trueTypePath, size_t fontSize) {
			static Font font;
			Font::STBTTDto &dto = font._tt;

			FILE *file = fopen(trueTypePath, "rb");
			if (!file) {
				std::cerr << "TrueType file at path `" << trueTypePath << "` not found." << std::endl;
			}
			fread(font._fontData, 1, MEGABYTES(1), file);
			fclose(file);

			if (!stbtt_PackBegin(&dto.context, font._atlasData, Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, 0, 1, nullptr)) {
				// TODO exception
				std::cerr << "Failed to start packing font with stb_truetype." << std::endl;
			}

			if (!stbtt_PackFontRange(&dto.context, font._fontData, 0, fontSize, 32, 95, dto.charInfo)) {
				// TODO exception
				std::cerr << "Failed to pack font with stb_truetype." << std::endl;
			}

			stbtt_PackEnd(&dto.context);

			// load properties
			stbtt_InitFont(&dto.info, (const u_char *) &font._fontData, 0);
			font._scale = stbtt_ScaleForPixelHeight(&dto.info, 32);
			int32_t advanceBuffer = 0, leftSideBeringBuffer = 0;
			stbtt_GetCodepointHMetrics(&dto.info, ' ', &advanceBuffer, &leftSideBeringBuffer);

			font._texture = createTexture(font._atlasData);

			stbi_write_jpg("test.jpg", Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, 1, font._atlasData, 100);

			return font;
		}

	private:
		static Texture createTexture(const u_char *data) {
			static Texture texture = Texture::requestTexture(1024, 1024, GL_RED, GL_UNSIGNED_BYTE, data);
			texture.linear();
			glActiveTexture(GL_TEXTURE0); // TODO feels like this should be in the Texture class
			glCheckError(GL_ACTIVATE_TEXTURE);
			return texture;
		}
	};

}

#endif //UINTA_FONT_LOADER_H
