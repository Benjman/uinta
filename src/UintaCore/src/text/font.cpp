#include "font_loader.h"

//#include <uinta/text/font.h>
#include "../include/uinta/text/font.h"

namespace uinta {

	Font Font::loadFont(const char *trueTypePath) {
		return FontLoader::loadFond(trueTypePath);
	}

	TexturedQuadDto Font::getTexturedQuadInfo(const char c, float_t *xCursor, float_t *yCursor) {
		static stbtt_aligned_quad quad;
		stbtt_GetPackedQuad(_tt.charInfo, Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, c - 32, xCursor, yCursor, &quad, 0);
		quad.x0 *= _scale;
		quad.y0 *= _scale;
		quad.x1 *= _scale;
		quad.y1 *= _scale;
		return quad;
	}

}
