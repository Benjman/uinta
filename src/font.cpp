#include <glad/glad.h>
#include <stdexcept>
#include <file.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#undef STB_RECT_PACK_IMPLEMENTATION

#define STB_TRUETYPE_IMPLEMENTATION
#include <font.hpp>
#undef STB_TRUETYPE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

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

void load_font(const font_ctx& ctx) {
    GLuint texId;
    unsigned char data[getFontSize(ctx.type)];
    read_file_binary(getFontPath(ctx.type), (char*) data);
    // read_file_binary(getFontPath(ctx.type), (char*) ctx.ttfData);

    unsigned char bitmap[ctx.stbtt_ctx.width * ctx.stbtt_ctx.height];

    if (!stbtt_PackBegin((stbtt_pack_context*) &ctx.stbtt_ctx, bitmap, ctx.stbtt_ctx.width, ctx.stbtt_ctx.height, 0, 1, nullptr))
        printf("some kinda error happened with stbtt_PackBegin\n");

    stbtt_PackFontRange((stbtt_pack_context*) &ctx.stbtt_ctx, data, 0, 32.0, 32, 95, (stbtt_packedchar*) ctx.stbtt_chardata);
    stbtt_PackEnd((stbtt_pack_context *)&ctx.stbtt_ctx);

    // flip vertically to comply with OpenGL texture coordinates
    stbi__vertical_flip(bitmap, ctx.stbtt_ctx.width, ctx.stbtt_ctx.height, 1);

    glGenTextures(1, (GLuint*) &ctx.textureId);
    glBindTexture(GL_TEXTURE_2D, ctx.textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ctx.stbtt_ctx.width, ctx.stbtt_ctx.height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_write_png("/tmp/test.png", ctx.stbtt_ctx.width, ctx.stbtt_ctx.height, 1, bitmap, 0);
}

#include <regex>
int getRenderableCharCount(const char* s, const unsigned int size) noexcept {
    const std::regex expression("[\x21-\x7E]");
    return std::ptrdiff_t(std::distance(
        std::cregex_iterator(s, &s[size], expression),
        std::cregex_iterator()));
}

font_ctx::font_ctx(const FontType type, const float tex_width, const float tex_height) noexcept :
    type(type) {
    stbtt_ctx.width = tex_width;
    stbtt_ctx.height = tex_height;
    ttfData = new unsigned char[getFontSize(type)];
}

font_ctx::~font_ctx() noexcept {
    delete[] ttfData;
}
