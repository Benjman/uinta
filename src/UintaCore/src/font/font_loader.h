#ifndef UINTA_FONT_LOADER_H
#define UINTA_FONT_LOADER_H

#include <uinta/font/font.h>
#include <uinta/io.h>
#include <uinta/util/assert.h>
#include <uinta/gl.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION

#include <stb_rect_pack.h>
#include <stb_truetype.h>

#undef STB_TRUETYPE_IMPLEMENTATION
//#undef STB_RECT_PACK_IMPLEMENTATION

#include <string>
#include <iostream>

namespace uinta {

	struct FontLoader {
		static Font loadFond(const char *trueTypePath) {
			Font font;
			stbtt_packedchar charInfo[95];

			// TODO convert to uinta file
			FILE *file = fopen(trueTypePath, "rb");
			fread(font.fontData, 1, sizeof(font.fontData), file);
			fclose(file);
			stbtt_pack_context context;

			if (!stbtt_PackBegin(&context, font.atlasData, 1024, 1024, 0, 1, nullptr)) {
				std::cerr << "Error beginning packing with stb_truetype." << std::endl;
			}

			stbtt_PackSetOversampling(&context, 2, 2);
			if (!stbtt_PackFontRange(&context, font.fontData, 0, 95, 32, 95, charInfo)) {
				std::cerr << "Error packing ranges with stb_truetype." << std::endl;
			}

			stbtt_PackEnd(&context);

			// TODO texture abstraction
			// Generate and upload texture
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &font._textureId);
			glBindTexture(GL_TEXTURE_2D, font._textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, font.atlasData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			return font;
		}
	};

}

#endif //UINTA_FONT_LOADER_H
