#include <glad/glad.h>
#include <file.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <font.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace font;
using namespace font::internal;

static const char* getFontPath(const FontType type) {
    switch (type) {
        case DejaVuMathTeXGyre: return "font/dejavu/DejaVuMathTeXGyre.ttf";
        case DejaVuSans_BoldOblique: return "font/dejavu/DejaVuSans_BoldOblique.ttf";
        case DejaVuSans_Bold: return "font/dejavu/DejaVuSans_Bold.ttf";
        case DejaVuSansCondensed_BoldOblique: return "font/dejavu/DejaVuSansCondensed_BoldOblique.ttf";
        case DejaVuSansCondensed_Bold: return "font/dejavu/DejaVuSansCondensed_Bold.ttf";
        case DejaVuSansCondensed_Oblique: return "font/dejavu/DejaVuSansCondensed_Oblique.ttf";
        case DejaVuSansCondensed: return "font/dejavu/DejaVuSansCondensed.ttf";
        case DejaVuSans_ExtraLight: return "font/dejavu/DejaVuSans_ExtraLight.ttf";
        case DejaVuSansMono_BoldOblique: return "font/dejavu/DejaVuSansMono_BoldOblique.ttf";
        case DejaVuSansMono_Bold: return "font/dejavu/DejaVuSansMono_Bold.ttf";
        case DejaVuSansMono_Oblique: return "font/dejavu/DejaVuSansMono_Oblique.ttf";
        case DejaVuSansMono: return "font/dejavu/DejaVuSansMono.ttf";
        case DejaVuSans_Oblique: return "font/dejavu/DejaVuSans_Oblique.ttf";
        case DejaVuSans: return "font/dejavu/DejaVuSans.ttf";
        case DejaVuSerif_BoldItalic: return "font/dejavu/DejaVuSerif_BoldItalic.ttf";
        case DejaVuSerif_Bold: return "font/dejavu/DejaVuSerif_Bold.ttf";
        case DejaVuSerifCondensed_BoldItalic: return "font/dejavu/DejaVuSerifCondensed_BoldItalic.ttf";
        case DejaVuSerifCondensed_Bold: return "font/dejavu/DejaVuSerifCondensed_Bold.ttf";
        case DejaVuSerifCondensed_Italic: return "font/dejavu/DejaVuSerifCondensed_Italic.ttf";
        case DejaVuSerifCondensed: return "font/dejavu/DejaVuSerifCondensed.ttf";
        case DejaVuSerif_Italic: return "font/dejavu/DejaVuSerif_Italic.ttf";
        case DejaVuSerif: return "font/dejavu/DejaVuSerif.ttf";
        default: throw std::runtime_error("Not implemented for font type\n");
    }
}

static const unsigned int getFontSize(const FontType type) {
    switch (type) {
        case DejaVuMathTeXGyre: return 577604u;
        case DejaVuSans_BoldOblique: return 643288u;
        case DejaVuSans_Bold: return 705684u;
        case DejaVuSansCondensed_BoldOblique: return 641696u;
        case DejaVuSansCondensed_Bold: return 703880u;
        case DejaVuSansCondensed_Oblique: return 633824u;
        case DejaVuSansCondensed: return 755124u;
        case DejaVuSans_ExtraLight: return 356760u;
        case DejaVuSansMono_BoldOblique: return 253580u;
        case DejaVuSansMono_Bold: return 331992u;
        case DejaVuSansMono_Oblique: return 251932u;
        case DejaVuSansMono: return 340712u;
        case DejaVuSans_Oblique: return 635412u;
        case DejaVuSans: return 757076u;
        case DejaVuSerif_BoldItalic: return 358820u;
        case DejaVuSerif_Bold: return 356088u;
        case DejaVuSerifCondensed_BoldItalic: return 357888u;
        case DejaVuSerifCondensed_Bold: return 355220u;
        case DejaVuSerifCondensed_Italic: return 358324u;
        case DejaVuSerifCondensed: return 379296u;
        case DejaVuSerif_Italic: return 359000u;
        case DejaVuSerif: return 380132u;
        default: throw std::runtime_error("Not implemented for font type\n");
    }
}

void font::load_font(font_ctx& font) {
    unsigned char bitmap[font.tex_width * font.tex_height];
    unsigned char data[getFontSize(font.type)];
    float line_size = 32.0;

    read_file_binary(getFontPath(font.type), (char*) data);

    stbtt_pack_context ctx;
    if (!stbtt_PackBegin(&ctx, bitmap, font.tex_width, font.tex_height, 0, 1, nullptr))
        printf("some kinda error happened with stbtt_PackBegin\n");
    stbtt_PackFontRange(&ctx, data, 0, line_size, 32, 95, font.chardata);
    stbtt_PackEnd(&ctx);

    stbtt_aligned_quad q;
    float xpos, ypos;
    stbtt_GetPackedQuad(font.chardata, font.tex_width, font.tex_height, 'W' - 32, &xpos, &ypos, &q, 0);

    // get vertical metrics
    stbtt_fontinfo info;
    stbtt_InitFont(&info, data, 0);
    int asc, dsc, gap;
    stbtt_GetFontVMetrics(&info, &asc, &dsc, &gap);
    float scale = stbtt_ScaleForPixelHeight(&info, line_size);
    font.asc = asc * scale;
    font.dsc = dsc * scale;
    font.gap = gap * scale;

    // flip vertically to comply with OpenGL texture coordinates
    stbi__vertical_flip(bitmap, font.tex_width, font.tex_height, 1);

    glGenTextures(1, (GLuint*) &font.textureId);
    glBindTexture(GL_TEXTURE_2D, font.textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.tex_width, font.tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // stbi_write_png("/tmp/test.png", font.tex_width, font.tex_height, 1, bitmap, 0);
}

font::font_ctx::font_ctx(const FontType type, const float tex_width, const float tex_height) noexcept :
    type(type), tex_width(tex_width), tex_height(tex_height) {
    chardata = new stbtt_packedchar[96];
}

font::font_ctx::~font_ctx() noexcept {
    delete[] chardata;
}

void font::getCharQuad(const char c, const font_ctx& ctx, stbtt_aligned_quad* quad) {
    float xpos = 0.0, ypos = 0.0;
    getCharQuad(c, ctx, quad, &xpos, &ypos);
}

void font::getCharQuad(const char c, const font_ctx& font, stbtt_aligned_quad* quad, float* xpos, float* ypos) {
    /*
        FIXME   font.chardata isn't thread safe and will cause collisions here.

        Possible solution:  This method could take an instance of stbtt_packedchar*, and each 
                            consumer would have their own copy of that array.

        Another solution:   Create a "requestTextMesh" pipeline, which can pool objects which have 
                            their own instances of the chardata collection, and other needed objects.
                            This way it would be data-oriented, and thread safe.
    */
    stbtt_GetPackedQuad(font.chardata, font.tex_width, font.tex_height, c - 32, xpos, ypos, quad, 0);
}
