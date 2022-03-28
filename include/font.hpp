#ifndef UINTA_FONT_H
#define UINTA_FONT_H

static const inline unsigned int FONT_SIZE_DejaVuMathTeXGyre = 577604u;
static const inline unsigned int FONT_SIZE_DejaVuSans_BoldOblique = 643288u;
static const inline unsigned int FONT_SIZE_DejaVuSans_Bold = 705684u;
static const inline unsigned int FONT_SIZE_DejaVuSansCondensed_BoldOblique = 641696u;
static const inline unsigned int FONT_SIZE_DejaVuSansCondensed_Bold = 703880u;
static const inline unsigned int FONT_SIZE_DejaVuSansCondensed_Oblique = 633824u;
static const inline unsigned int FONT_SIZE_DejaVuSansCondensed = 755124u;
static const inline unsigned int FONT_SIZE_DejaVuSans_ExtraLight = 356760u;
static const inline unsigned int FONT_SIZE_DejaVuSansMono_BoldOblique = 253580u;
static const inline unsigned int FONT_SIZE_DejaVuSansMono_Bold = 331992u;
static const inline unsigned int FONT_SIZE_DejaVuSansMono_Oblique = 251932u;
static const inline unsigned int FONT_SIZE_DejaVuSansMono = 340712u;
static const inline unsigned int FONT_SIZE_DejaVuSans_Oblique = 635412u;
static const inline unsigned int FONT_SIZE_DejaVuSans = 757076u;
static const inline unsigned int FONT_SIZE_DejaVuSerif_BoldItalic = 358820u;
static const inline unsigned int FONT_SIZE_DejaVuSerif_Bold = 356088u;
static const inline unsigned int FONT_SIZE_DejaVuSerifCondensed_BoldItalic = 357888u;
static const inline unsigned int FONT_SIZE_DejaVuSerifCondensed_Bold = 355220u;
static const inline unsigned int FONT_SIZE_DejaVuSerifCondensed_Italic = 358324u;
static const inline unsigned int FONT_SIZE_DejaVuSerifCondensed = 379296u;
static const inline unsigned int FONT_SIZE_DejaVuSerif_Italic = 359000u;
static const inline unsigned int FONT_SIZE_DejaVuSerif = 380132u;

#include <GL/gl.h>
#include <stb_truetype.h>
void load_font(const unsigned char* ttfdata, GLuint *textureid,
               const unsigned int texture_width, const unsigned int texture_height,
               stbtt_pack_context &ctx, stbtt_packedchar* chardata);

int getRenderableCharCount(const char* s, const unsigned int size) noexcept;

#endif // UINTA_FONT_H
