#include "font_loader.h"

//#include <uinta/text/font.h>
#include "../include/uinta/text/font.h"

namespace uinta {

	Font Font::loadFont(const char *trueTypePath, size_t fontSize) {
		return FontLoader::loadFond(trueTypePath, fontSize);
	}

	void Font::getCharWidth(int c, float_t *advance, float_t *leftSideBearing) {
		int32_t advanceBuffer, leftSideBeringBuffer;
		stbtt_GetCodepointHMetrics(&_tt.info, c, &advanceBuffer, &leftSideBeringBuffer);
		*advance = (float_t) advanceBuffer * _scale;
		*leftSideBearing = (float_t) leftSideBeringBuffer * _scale;
	}

	TexturedQuadDto Font::getTexturedQuadInfo(const char c, float_t xCursor, float_t yCursor) {
		static stbtt_aligned_quad quad;
		stbtt_GetPackedQuad(_tt.charInfo, Font::ATLAS_WIDTH, Font::ATLAS_HEIGHT, c - 32, &xCursor, &yCursor, &quad, 0);
		size_t yDiff = quad.y1 - quad.y0;
		quad.y0 += yDiff;
		quad.y1 += yDiff;

		quad.x0 *= _scale;
		quad.y0 *= _scale;
		quad.x1 *= _scale;
		quad.y1 *= _scale;
		return quad;
	}

}
