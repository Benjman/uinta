/**
     Include this file in whatever places need to refer to it. In ONE C/C++
     file, write:
        #define UINTA_FONT_IMPLEMENTATION
     before the #include of this file. This expands out the actual
     implementation into that C/C++ file.
**/


#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __APPLE__
#define GL_SILENCe_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__

#ifdef UINTA_FONT_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#endif

#include <stb/stb_truetype.h>

namespace font {

using font_t = unsigned char;

enum font_mesh_attrib_t {
    FontMeshAttrib_Position,
    FontMeshAttrib_Color,
    FontMeshAttrib_UV,
};

struct font_mesh_attrib final {
    const unsigned int stride;
    const unsigned int offset;
    font_mesh_attrib(const unsigned int stride, const unsigned int offset) noexcept
        : stride(stride), offset(offset) {
    }
};

enum FontType {
    DejaVuMathTeXGyre,
    DejaVuSans,
    DejaVuSansCondensed,
    DejaVuSansCondensed_Bold,
    DejaVuSansCondensed_BoldOblique,
    DejaVuSansCondensed_Oblique,
    DejaVuSansMono,
    DejaVuSansMono_Bold,
    DejaVuSansMono_BoldOblique,
    DejaVuSansMono_Oblique,
    DejaVuSans_Bold,
    DejaVuSans_BoldOblique,
    DejaVuSans_ExtraLight,
    DejaVuSans_Oblique,
    DejaVuSerif,
    DejaVuSerifCondensed,
    DejaVuSerifCondensed_Bold,
    DejaVuSerifCondensed_BoldItalic,
    DejaVuSerifCondensed_Italic,
    DejaVuSerif_Bold,
    DejaVuSerif_BoldItalic,
    DejaVuSerif_Italic,

    ProggyCleanTT_Nerd_Font_Complete,
    ProggyCleanTT_Nerd_Font_Complete_Mono,
};

font_t init_font(FontType, unsigned int tex_width, unsigned int tex_height);


/********************
    FONT LOADING
********************/
struct font_ctx final {
    FontType type;
    stbtt_packedchar chardata[96];
    GLuint texture_id = GL_ZERO;
    unsigned int tex_width, tex_height;
    float asc = 0.0, dsc = 0.0, gap = 0.0, line_size = 32.0;

    font_ctx(const FontType, const float tex_width, const float tex_height) noexcept;

    font_ctx(const font_ctx& other) noexcept {
        *this = other;
    }

    font_ctx& operator=(const font_ctx& other) noexcept {
        type = other.type;
        texture_id = other.texture_id;
        tex_width = other.tex_width;
        tex_height = other.tex_height;
        asc = other.asc;
        dsc = other.dsc;
        gap = other.gap;
        line_size = other.line_size;
        memcpy(chardata, other.chardata, sizeof(chardata));
        return *this;
    }

};

struct font_ctx;
GLuint load_font(const font_t, unsigned char*);
void getCharQuad(const char, const font_ctx&, stbtt_aligned_quad*);
void getCharQuad(const char, const font_ctx&, stbtt_aligned_quad*, float*, float*);
GLuint getFontTexture(const font_t);
font_ctx& get_font_ctx(const font_t);

/*********************
    TEXT ELEMENT
********************/
struct text final {
    std::string value;
    float line_size;

    float color_r;
    float color_g;
    float color_b;

    float pos_x;
    float pos_y;

    float max_width;
    float max_height;

    text(const std::string value, const float line_size,
         float color_r = 0.0, float color_g = 0.0, float color_b = 0.0,
         float pos_x = 0.0, float pos_y = 0.0,
         float max_width = 0.0, float max_height = 0.0) noexcept :
        value(value),
        line_size(line_size),
        color_r(color_r), color_g(color_g), color_b(color_b),
        pos_x(pos_x), pos_y(pos_y),
        max_width(max_width), max_height(max_height) {
    }

    text(const text& other) noexcept {
        *this = other;
    }

    text& operator=(const text& other) noexcept {
        color_r = other.color_r;
        color_g = other.color_g;
        color_b = other.color_b;
        pos_x = other.pos_x;
        pos_y = other.pos_y;
        max_width = other.max_width;
        max_height = other.max_height;
        value = other.value;
        line_size = other.line_size;
        return *this;
    }

};

unsigned int getRenderableCharCount(const char* s, const unsigned int size) noexcept;
unsigned int getVertBufferSize(const char* s, const unsigned int size, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs) noexcept;
unsigned int getIndexBufferSize(const char* s, const unsigned int size) noexcept;
const unsigned int getFontSize(const FontType);
const char* const getFontPath(const FontType);

void generate_mesh(const text* root, const font_t f, const float frame_width, const float frame_height, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs, float* vbuf, unsigned int* vcount, unsigned int* ibuf, unsigned int* icount, unsigned int* ioffset);


/*********************
    INTERNAL ONLY
********************/
namespace internal {
struct mesh_ctx final {
    const text* root;
    const font_ctx* font;
    const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs;
    float frame_width;
    float frame_height;
    float* vbuf;
    unsigned int* vcount;
    unsigned int* ibuf;
    unsigned int* icount;
    unsigned int* ioffset;
    float xcursor = 0.0;
    float ycursor = 0.0;
    float text_scale = 1.0;
};

struct word final {
    std::string value;
    float width;

    word() {
        width = 0.0;
    }

    word(const word& other) noexcept {
        *this = other;
    }

    word& operator=(const word& other) noexcept {
        value = std::string(other.value);
        width = other.width;
        return *this;
    }
};

struct line final {
    std::vector<word> words;
    float width;
    float max_width;

    line() noexcept : line(0.0) {}

    line(const float max_width) noexcept : width(0.0), max_width(max_width) {
    }

    line(const line& other) noexcept {
        *this = other;
    }

    line& operator=(const line& other) noexcept {
        words = std::vector<word>(other.words);
        width = other.width;
        return *this;
    }
};

void generate_mesh(mesh_ctx);
void generate_structure(mesh_ctx, std::vector<line>*, stbtt_aligned_quad&);
bool try_add_word(line&, word&);
void add_char(word&, const char, float);
float find_xstart(const text*, const float);
const font_mesh_attrib* find_font_attrib(font_mesh_attrib_t, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>*); // TODO maybe this should be in mesh.hpp?
void store_quad_position(const stbtt_aligned_quad&, const font_mesh_attrib&, float*, unsigned int*);
void store_quad_uv(const stbtt_aligned_quad&, const font_mesh_attrib&, float*, unsigned int*);
void store_color(const float, const float, const float, const font_mesh_attrib&, float*, unsigned int*);

}}



#ifdef UINTA_FONT_IMPLEMENTATION

#include <stdexcept>

namespace font {
    std::vector<font_ctx> fonts;
}

using namespace font;
using namespace font::internal;

const char* const font::getFontPath(const FontType type) {
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

        case ProggyCleanTT_Nerd_Font_Complete_Mono: return "font/proggy/ProggyCleanTT Nerd Font Complete Mono.ttf";
        case ProggyCleanTT_Nerd_Font_Complete: return "font/proggy/ProggyCleanTT Nerd Font Complete Mono.ttf";

        default: throw std::runtime_error("Not implemented for font type\n");
    }
}

const unsigned int font::getFontSize(const FontType type) {
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

        case ProggyCleanTT_Nerd_Font_Complete_Mono: return 838628u;
        case ProggyCleanTT_Nerd_Font_Complete: return 874880u;

        default: throw std::runtime_error("Not implemented for font type\n");
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb/stb_image_write.h>

font::font_ctx::font_ctx(const FontType type, const float tex_width, const float tex_height) noexcept :
    type(type), tex_width(tex_width), tex_height(tex_height) {
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

#include <regex>
unsigned int font::getRenderableCharCount(const char* s, const unsigned int size) noexcept {
    const std::regex expression("[\x21-\x7E]");
    return std::ptrdiff_t(std::distance(
        std::cregex_iterator(s, &s[size], expression),
        std::cregex_iterator()));
}

unsigned int font::getVertBufferSize(const char* s, const unsigned int size, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs) noexcept {
    unsigned int bufsize = 0;
    if (const auto* attrib = find_font_attrib(FontMeshAttrib_Position, attribs))
        bufsize += 8;
    if (const auto* attrib = find_font_attrib(FontMeshAttrib_UV, attribs))
        bufsize += 8;
    if (const auto* attrib = find_font_attrib(FontMeshAttrib_Color, attribs))
        bufsize += 12;
    const int count = getRenderableCharCount(s, size);
    return bufsize * count;
}

unsigned int font::getIndexBufferSize(const char* s, const unsigned int size) noexcept {
    const int count = getRenderableCharCount(s, size);
    return 6 * count;
}

void font::generate_mesh(const text* root, const font_t f, const float frame_width, const float frame_height, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs, float* vbuf, unsigned int* vcount, unsigned int* ibuf, unsigned int* icount, unsigned int* ioffset) {
    assert(font::fonts.size() > f);
    mesh_ctx ctx;
    ctx.root = root;
    ctx.font = &font::fonts.at(f);
    ctx.frame_width = frame_width;
    ctx.frame_height = frame_height;
    ctx.attribs = attribs;
    ctx.vbuf = vbuf;
    ctx.vcount = vcount;
    ctx.ibuf = ibuf;
    ctx.icount = icount;
    ctx.ioffset = ioffset;
    ctx.text_scale = (float) root->line_size / (float) ctx.font->line_size;
    generate_mesh(ctx);
}

void font::internal::generate_mesh(mesh_ctx ctx) {
    if (ctx.root->value.empty())
        return;
    if (ctx.attribs->size() == 0) {
        printf("[WARN] No attributes were provided to generate a text mesh.");
        return;
    }
    const unsigned int vertexStride = ctx.attribs->begin()->second.stride * 4;
    std::vector<line> lines;
    stbtt_aligned_quad quad;
    generate_structure(ctx, &lines, quad);
    ctx.ycursor = ctx.font->asc;
    float xcursor = 0.0;
    for (const auto& line : lines) {
        ctx.xcursor = find_xstart(ctx.root, line.width);
        for (auto& word : line.words) {
            for (unsigned int i = 0u, len = word.value.length(); i < len; i++) {
                const char c = word.value.at(i);
                xcursor = 0.0;
                getCharQuad(c, *ctx.font, &quad, &xcursor, &ctx.ycursor);

                // scale from font to text size
                quad.x0 *= ctx.text_scale;
                quad.x1 *= ctx.text_scale;
                quad.y0 *= ctx.text_scale;
                quad.y1 *= ctx.text_scale;

                // place according to text position
                quad.x0 += ctx.root->pos_x + ctx.xcursor;
                quad.x1 += ctx.root->pos_x + ctx.xcursor;
                quad.y0 += ctx.root->pos_y;
                quad.y1 += ctx.root->pos_y;

                // increase cursor
                xcursor *= ctx.text_scale;
                ctx.xcursor += xcursor;

                // normalize to screen space
                quad.x0 /= ctx.frame_width;
                quad.x1 /= ctx.frame_width;
                quad.y0 /= ctx.frame_height;
                quad.y1 /= ctx.frame_height;
                
                // normalize to ndc unit
                quad.x0 =  2 * quad.x0 - 1;
                quad.x1 =  2 * quad.x1 - 1;
                quad.y0 = -2 * quad.y0 + 1;
                quad.y1 = -2 * quad.y1 + 1;

                // flip UV vertical axis for OpenGL texture coordinates
                quad.t0 *= -1;
                quad.t1 *= -1;

                // store quad to buffers
                if (const font_mesh_attrib* attrib = find_font_attrib(FontMeshAttrib_Position, ctx.attribs))
                    store_quad_position(quad, *attrib, ctx.vbuf, ctx.vcount);
                if (const font_mesh_attrib* attrib = find_font_attrib(FontMeshAttrib_UV, ctx.attribs))
                    store_quad_uv(quad, *attrib, ctx.vbuf, ctx.vcount);
                if (const font_mesh_attrib* attrib = find_font_attrib(FontMeshAttrib_Color, ctx.attribs))
                    store_color(ctx.root->color_r, ctx.root->color_g, ctx.root->color_b, *attrib, ctx.vbuf, ctx.vcount);

                ctx.ibuf[0] = 0 + *ctx.ioffset;
                ctx.ibuf[1] = 1 + *ctx.ioffset;
                ctx.ibuf[2] = 3 + *ctx.ioffset;
                ctx.ibuf[3] = 1 + *ctx.ioffset;
                ctx.ibuf[4] = 2 + *ctx.ioffset;
                ctx.ibuf[5] = 3 + *ctx.ioffset;
                *ctx.icount += 6;
                *ctx.ioffset += 4;

                // advance vertex buffer to next quad position
                ctx.vbuf = &ctx.vbuf[vertexStride];
                // advance index buffer
                ctx.ibuf = &ctx.ibuf[6];
            }
            // advance xcursor one space length
            xcursor = 0.0;
            getCharQuad(' ', *ctx.font, &quad, &xcursor, &ctx.ycursor);
            xcursor *= ctx.text_scale;
            ctx.xcursor += xcursor;
        }
        ctx.ycursor += ctx.root->line_size;
    }
}

void font::internal::generate_structure(mesh_ctx ctx, std::vector<line>* lines, stbtt_aligned_quad& quad) {
    line line(ctx.root->max_width);
    word word;

    float space_width = 0.0, xpos = 0.0, ypos = 0.0;
    getCharQuad(' ', *ctx.font, &quad, &space_width, &ypos);
    space_width *= ctx.text_scale;

    for (unsigned int i = 0, len = ctx.root->value.size(); i < len; i++) {
        char c = ctx.root->value.at(i);
        if (c == ' ') {
            if (!try_add_word(line, word)) {
                line.width -= space_width;
                lines->emplace_back(font::internal::line(line));
                line = font::internal::line(ctx.root->max_width); // TODO test the assignment operator. i'm worried it's operating like a copy.
                try_add_word(line, word);
            }
            word = font::internal::word();
            line.width += space_width;
            continue;
        }

        xpos = 0.0;
        c = ctx.root->value.at(i);
        getCharQuad(c, *ctx.font, &quad, &xpos, &ypos);
        xpos *= ctx.text_scale;
        add_char(word, c, xpos);
    }

    if (!try_add_word(line, word)) {
        lines->emplace_back(font::internal::line(line));
        line = font::internal::line(ctx.root->max_width);
        try_add_word(line, word);
    }

    lines->emplace_back(font::internal::line(line));
}

bool font::internal::try_add_word(line& line, word& word) {
    if (line.max_width && line.max_width < line.width + word.width)
        return false;
    line.words.emplace_back(font::internal::word(word));
    line.width += word.width;
    return true;
}

void font::internal::add_char(word& word, const char c, float width) {
    word.value += c;
    word.width += width;
}

float font::internal::find_xstart(const text* root, const float width) {
    // TODO horizontal alignment
    return 0;
}

const font_mesh_attrib* font::internal::find_font_attrib(font_mesh_attrib_t type, const std::unordered_map<font_mesh_attrib_t, font_mesh_attrib>* attribs) {
    if (attribs->find(type) == attribs->end())
        return nullptr;
    return &attribs->at(type);
}

void font::internal::store_quad_position(const stbtt_aligned_quad& quad, const font_mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
    vbuf[attrib.offset + attrib.stride * 0 + 0] = quad.x0;
    vbuf[attrib.offset + attrib.stride * 0 + 1] = quad.y0;
    vbuf[attrib.offset + attrib.stride * 1 + 0] = quad.x0;
    vbuf[attrib.offset + attrib.stride * 1 + 1] = quad.y1;
    vbuf[attrib.offset + attrib.stride * 2 + 0] = quad.x1;
    vbuf[attrib.offset + attrib.stride * 2 + 1] = quad.y1;
    vbuf[attrib.offset + attrib.stride * 3 + 0] = quad.x1;
    vbuf[attrib.offset + attrib.stride * 3 + 1] = quad.y0;
    *vcount += 8;
}

void font::internal::store_quad_uv(const stbtt_aligned_quad& quad, const font_mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
    vbuf[attrib.offset + attrib.stride * 0 + 0] = quad.s0;
    vbuf[attrib.offset + attrib.stride * 0 + 1] = quad.t0;
    vbuf[attrib.offset + attrib.stride * 1 + 0] = quad.s0;
    vbuf[attrib.offset + attrib.stride * 1 + 1] = quad.t1;
    vbuf[attrib.offset + attrib.stride * 2 + 0] = quad.s1;
    vbuf[attrib.offset + attrib.stride * 2 + 1] = quad.t1;
    vbuf[attrib.offset + attrib.stride * 3 + 0] = quad.s1;
    vbuf[attrib.offset + attrib.stride * 3 + 1] = quad.t0;
    *vcount += 8;
}

void font::internal::store_color(const float r, const float g, const float b, const font_mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
    vbuf[attrib.offset + attrib.stride * 0 + 0] = r;
    vbuf[attrib.offset + attrib.stride * 0 + 1] = g;
    vbuf[attrib.offset + attrib.stride * 0 + 2] = b;
    vbuf[attrib.offset + attrib.stride * 1 + 0] = r;
    vbuf[attrib.offset + attrib.stride * 1 + 1] = g;
    vbuf[attrib.offset + attrib.stride * 1 + 2] = b;
    vbuf[attrib.offset + attrib.stride * 2 + 0] = r;
    vbuf[attrib.offset + attrib.stride * 2 + 1] = g;
    vbuf[attrib.offset + attrib.stride * 2 + 2] = b;
    vbuf[attrib.offset + attrib.stride * 3 + 0] = r;
    vbuf[attrib.offset + attrib.stride * 3 + 1] = g;
    vbuf[attrib.offset + attrib.stride * 3 + 2] = b;
    *vcount += 12;
}

font::font_t font::init_font(font::FontType type, unsigned int tex_width, unsigned int tex_height) {
    for (font::font_t i = 0; i < font::fonts.size(); i++) {
        auto& font = font::fonts.at(i);
        if (type == font.type
            && tex_width == font.tex_width
            && tex_height == font.tex_height) {
            return i;
        }
    }
    font::font_ctx ctx(type, tex_width, tex_height);
    font::fonts.push_back(ctx);
    return font::fonts.size() - 1;
}

#include <cassert>
GLuint font::getFontTexture(const font_t handle) {
    assert(font::fonts.size() > handle); // invalid handle
    return font::fonts.at(handle).texture_id;
}

font_ctx& font::get_font_ctx(const font_t handle) {
    assert(font::fonts.size() > handle); // invalid handle
    return font::fonts.at(handle);
}

GLuint font::load_font(const font_t handle, unsigned char* buffer) {
    assert(fonts.size() > handle); // invalid handle
    auto& font = fonts.at(handle);
    unsigned char bitmap[font.tex_width * font.tex_height];
    stbtt_pack_context ctx;
    if (!stbtt_PackBegin(&ctx, bitmap, font.tex_width, font.tex_height, 0, 1, nullptr))
        printf("some kinda error happened with stbtt_PackBegin\n");
    stbtt_PackFontRange(&ctx, buffer, 0, font.line_size, 32, 95, font.chardata);
    stbtt_PackEnd(&ctx);

    stbtt_aligned_quad q;
    float xpos, ypos;
    stbtt_GetPackedQuad(font.chardata, font.tex_width, font.tex_height, 'W' - 32, &xpos, &ypos, &q, 0);

    // get vertical metrics
    stbtt_fontinfo info;
        stbtt_InitFont(&info, buffer, 0);
        int asc, dsc, gap;
        stbtt_GetFontVMetrics(&info, &asc, &dsc, &gap);
        float scale = stbtt_ScaleForPixelHeight(&info, font.line_size);
        font.asc = asc * scale;
    font.dsc = dsc * scale;
    font.gap = gap * scale;

    // flip vertically to comply with OpenGL texture coordinates
    stbi__vertical_flip(bitmap, font.tex_width, font.tex_height, 1);

    glGenTextures(1, (GLuint*) &font.texture_id);
    glBindTexture(GL_TEXTURE_2D, font.texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.tex_width, font.tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // stbi_write_png("/tmp/test.png", font.tex_width, font.tex_height, 1, bitmap, 0);

    return font.texture_id;
}

#endif // UINTA_FONT_IMPLEMENTATION
#endif // UINTA_FONT_H
