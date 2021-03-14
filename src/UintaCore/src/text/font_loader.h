#ifndef UINTA_FONT_LOADER_H
#define UINTA_FONT_LOADER_H

#include <uinta/text/font.h>
#include <uinta/gl.h>
#include <uinta/io.h>
#include <uinta/texture.h>
#include <uinta/util/assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION

#include <stb_rect_pack.h>
#include <stb_truetype.h>

#undef STB_TRUETYPE_IMPLEMENTATION
#undef STB_RECT_PACK_IMPLEMENTATION

#include <string>
#include <iostream>

namespace uinta {

	struct FontLoader {
		static Font loadFond(const char *trueTypePath) {
			u_char _atlasData[1024 * 1024]{};
			u_char _fontData[1048576]{};
			stbtt_packedchar charInfo[95];

			// TODO convert to uinta file
			FILE *file = fopen(trueTypePath, "rb");
			fread(_fontData, 1, sizeof(_fontData), file);
			fclose(file);

			stbtt_pack_context context;
			if (!stbtt_PackBegin(&context, _atlasData, 1024, 1024, 0, 1, nullptr)) {
				std::cerr << "Error beginning packing with stb_truetype." << std::endl;
			}

			if (!stbtt_PackFontRange(&context, _fontData, 0, 195, 32, 95, charInfo)) {
				std::cerr << "Error packing ranges with stb_truetype." << std::endl;
			}

			stbtt_PackEnd(&context);

			Texture texture = createTexture(_atlasData);

			return Font(texture);
		}

	private:
		static Texture createTexture(const u_char *_atlasData) {
			Texture texture = Texture::requestTexture(1024, 1024, GL_RED, GL_UNSIGNED_BYTE, _atlasData);
			texture.linear();
			glActiveTexture(GL_TEXTURE0);
			return texture;
		}
	};

}

#endif //UINTA_FONT_LOADER_H
