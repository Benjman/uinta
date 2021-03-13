#ifndef UINTA_FONT_LOADER_H
#define UINTA_FONT_LOADER_H

#include <uinta/font/font.h>
#include <uinta/io.h>
#include <uinta/util/assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION

#include <stb_truetype.h>
//#include <stb_rect_pack.h>

#undef STB_TRUETYPE_IMPLEMENTATION
//#undef STB_RECT_PACK_IMPLEMENTATION

#include <string>
#include <iostream>

namespace uinta {

	struct FontLoader {
		static Font loadFond(const char *trueTypePath) {
			Font font;
//			stbtt_packedchar charInfo[95];
//
//			FILE *file = fopen(trueTypePath, "rb");
//			fread(font.fontData, 1, sizeof(font.fontData), file);
//			fclose(file);
//			stbtt_pack_context context;
//			if (!stbtt_PackBegin(&context, font.atlasData, 1024, 1024, 0, 1, nullptr)) {
//				std::cerr << "Error beginning packing with stb_truetype." << std::endl;
//			}
//			stbtt_PackSetOversampling(&context, 2, 2);
//			if (!stbtt_PackFontRange(&context, font.fontData, 0, 32, 32, 95, charInfo)) {
//				std::cerr << "Error packing ranges with stb_truetype." << std::endl;
//			}
//			stbtt_PackEnd(&context);
			return font;
		}
	};

}

#endif //UINTA_FONT_LOADER_H
