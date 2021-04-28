#include "font_loader.h"

#include <uinta/io/file.h>
#include <uinta/text/font.h>

using namespace uinta;

Font *Font::loadFont(const char *trueTypePath) {
	return FontLoader::loadFond(File::getFilePath(trueTypePath));
}

stbtt_aligned_quad Font::getQuadInfo(const char c, float_t *xCursor, float_t *yCursor) const {
	stbtt_aligned_quad quad;
	stbtt_GetBakedQuad(_stbttBakedChar, Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, c - 32, xCursor, yCursor, &quad, 0);
	return quad;
}
