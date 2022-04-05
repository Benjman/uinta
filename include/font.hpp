#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <GL/gl.h>
#include <stb_truetype.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <math.hpp>
#include <mesh.hpp>

namespace font {

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


/********************
    FONT LOADING
********************/
struct font_ctx final {
    FontType type;
    stbtt_packedchar *chardata = nullptr;
    GLuint textureId = GL_ZERO;
    unsigned int tex_width, tex_height;
    float asc = 0.0, dsc = 0.0, gap = 0.0, line_size = 32.0;

    font_ctx(const FontType, const float tex_width, const float tex_height) noexcept;

    font_ctx(const font_ctx& other) noexcept {
        *this = other;
    }

    font_ctx& operator=(const font_ctx& other) noexcept {
        type = other.type;
        chardata = other.chardata;
        textureId = other.textureId;
        tex_width = other.tex_width;
        tex_height = other.tex_height;
        asc = other.asc;
        dsc = other.dsc;
        gap = other.gap;
        line_size = other.line_size;
        return *this;
    }

    ~font_ctx() noexcept;
};
void load_font(font_ctx&);
void getCharQuad(const char c, const font_ctx& ctx, stbtt_aligned_quad* quad); // TODO can we noexcept these?
void getCharQuad(const char c, const font_ctx& ctx, stbtt_aligned_quad* quad, float* xpos, float* ypos);

/*********************
    TEXT ELEMENT
********************/
struct text final {
    const font_ctx* font;
    std::string value;
    vec3 color;
    vec2 pos;
    vec2 dimensions;
    float line_size;

    text(const font_ctx* font, const std::string value, const float line_size, const vec3 color = vec3(0.0), const vec2 pos = vec2(0.0), const vec2 dimensions = vec2(0)) noexcept 
    : font(font), value(value), line_size(line_size), color(color), pos(pos), dimensions(dimensions) { }

    text(const text& other) noexcept {
        *this = other;
    }

    text& operator=(const text& other) noexcept {
        dimensions = other.dimensions;
        pos = other.pos;
        color = other.color;
        font = other.font;
        value = other.value;
        line_size = other.line_size;
        return *this;
    }

};


/*********************
    MESH GENERATION
********************/
int getRenderableCharCount(const char* s, const unsigned int size) noexcept;

void generate_mesh(const text* root, const float frame_width, const float frame_height, const std::unordered_map<MeshAttribType, mesh_attrib> attribs, float* vbuf, unsigned int* vcount, unsigned int* ibuf, unsigned int* icount, unsigned int* ioffset);


/*********************
    INTERNAL ONLY
********************/
namespace internal {
struct mesh_ctx final {
    const text* root;
    float frame_width;
    float frame_height;
    std::unordered_map<MeshAttribType, mesh_attrib> attribs;
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

    line() noexcept {
        width = 0.0;
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
const mesh_attrib* find_attrib(MeshAttribType, const std::unordered_map<MeshAttribType, mesh_attrib>*); // TODO maybe this should be in mesh.hpp?
void store_quad_position(const stbtt_aligned_quad&, const mesh_attrib&, float*);
void store_quad_uv(const stbtt_aligned_quad&, const mesh_attrib&, float*);
void store_color(const vec3&, const mesh_attrib&, float*);

}}

#endif // UINTA_FONT_H
