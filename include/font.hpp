#ifndef UINTA_FONT_H
#define UINTA_FONT_H

enum FontType {
	DejaVuMathTeXGyre,
	DejaVuSans_BoldOblique,
	DejaVuSans_Bold,
	DejaVuSansCondensed_BoldOblique,
	DejaVuSansCondensed_Bold,
	DejaVuSansCondensed_Oblique,
	DejaVuSansCondensed,
	DejaVuSans_ExtraLight,
	DejaVuSansMono_BoldOblique,
	DejaVuSansMono_Bold,
	DejaVuSansMono_Oblique,
	DejaVuSansMono,
	DejaVuSans_Oblique,
	DejaVuSans,
	DejaVuSerif_BoldItalic,
	DejaVuSerif_Bold,
	DejaVuSerifCondensed_BoldItalic,
	DejaVuSerifCondensed_Bold,
	DejaVuSerifCondensed_Italic,
	DejaVuSerifCondensed,
	DejaVuSerif_Italic,
	DejaVuSerif
};


int getRenderableCharCount(const char* s, const unsigned int size) noexcept;


#include <GL/gl.h>
#include <stb_truetype.h>
struct font_ctx {
    const FontType type;
    stbtt_packedchar *stbtt_chardata = nullptr;
    unsigned char *ttfData = nullptr;
    GLuint textureId = GL_ZERO;
    unsigned int tex_width, tex_height;

    font_ctx(const FontType, const float tex_width, const float tex_height) noexcept;

    ~font_ctx() noexcept;
};
void load_font(const font_ctx&);
void getCharQuad(const char c, const font_ctx& ctx, stbtt_aligned_quad* quad);
void getCharQuad(const char c, const font_ctx& ctx, stbtt_aligned_quad* quad, float* xpos, float* ypos);

#endif // UINTA_FONT_H
