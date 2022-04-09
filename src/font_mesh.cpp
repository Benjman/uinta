#include <font.hpp>
#include <regex>

using namespace font;
using namespace font::internal;

unsigned int font::getRenderableCharCount(const char* s, const unsigned int size) noexcept {
    const std::regex expression("[\x21-\x7E]");
    return std::ptrdiff_t(std::distance(
        std::cregex_iterator(s, &s[size], expression),
        std::cregex_iterator()));
}

unsigned int font::getVertBufferSize(const char* s, const unsigned int size, const std::unordered_map<MeshAttribType, mesh_attrib> attribs) noexcept {
    unsigned int bufsize = 0;
    if (const mesh_attrib* attrib = find_attrib(MeshAttribType_Position, &attribs))
        bufsize += 8;
    if (const mesh_attrib* attrib = find_attrib(MeshAttribType_UV, &attribs))
        bufsize += 8;
    if (const mesh_attrib* attrib = find_attrib(MeshAttribType_Color, &attribs))
        bufsize += 12;
    const int count = getRenderableCharCount(s, size);
    return bufsize * count;
}

unsigned int font::getIndexBufferSize(const char* s, const unsigned int size) noexcept {
    const int count = getRenderableCharCount(s, size);
    return 6 * count;
}

void font::generate_mesh(const text* root, const float frame_width, const float frame_height, const std::unordered_map<MeshAttribType, mesh_attrib> attribs, float* vbuf, unsigned int* vcount, unsigned int* ibuf, unsigned int* icount, unsigned int* ioffset) {
    mesh_ctx ctx;
    ctx.root = root;
    ctx.frame_width = frame_width;
    ctx.frame_height = frame_height;
    ctx.attribs = attribs;
    ctx.vbuf = vbuf;
    ctx.vcount = vcount;
    ctx.ibuf = ibuf;
    ctx.icount = icount;
    ctx.ioffset = ioffset;
    ctx.text_scale = (float) root->line_size / (float) root->font->line_size;
    generate_mesh(ctx);
}

void font::internal::generate_mesh(mesh_ctx ctx) {
    if (ctx.root->value.empty())
        return;
    if (ctx.attribs.size() == 0) {
        printf("[WARN] No attributes were provided to generate a text mesh.");
        return;
    }
    const unsigned int vertexStride = ctx.attribs.begin()->second.stride * 4;
    std::vector<line> lines;
    stbtt_aligned_quad quad;
    generate_structure(ctx, &lines, quad);
    ctx.ycursor = ctx.root->font->asc;
    float xcursor = 0.0;
    for (const auto& line : lines) {
        ctx.xcursor = find_xstart(ctx.root, line.width);
        for (auto& word : line.words) {
            for (unsigned int i = 0u, len = word.value.length(); i < len; i++) {
                const char c = word.value.at(i);
                xcursor = 0.0;
                getCharQuad(c, *ctx.root->font, &quad, &xcursor, &ctx.ycursor);

                // scale from font to text size
                quad.x0 *= ctx.text_scale;
                quad.x1 *= ctx.text_scale;
                quad.y0 *= ctx.text_scale;
                quad.y1 *= ctx.text_scale;

                // place according to text position
                quad.x0 += ctx.root->pos.x + ctx.xcursor;
                quad.x1 += ctx.root->pos.x + ctx.xcursor;
                quad.y0 += ctx.root->pos.y;
                quad.y1 += ctx.root->pos.y;

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
                if (const mesh_attrib* attrib = find_attrib(MeshAttribType_Position, &ctx.attribs))
                    store_quad_position(quad, *attrib, ctx.vbuf, ctx.vcount);
                if (const mesh_attrib* attrib = find_attrib(MeshAttribType_UV, &ctx.attribs))
                    store_quad_uv(quad, *attrib, ctx.vbuf, ctx.vcount);
                if (const mesh_attrib* attrib = find_attrib(MeshAttribType_Color, &ctx.attribs))
                    store_color(ctx.root->color, *attrib, ctx.vbuf, ctx.vcount);

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
            getCharQuad(' ', *ctx.root->font, &quad, &xcursor, &ctx.ycursor);
            xcursor *= ctx.text_scale;
            ctx.xcursor += xcursor;
        }
        ctx.ycursor += ctx.root->line_size;
    }
}

void font::internal::generate_structure(mesh_ctx ctx, std::vector<line>* lines, stbtt_aligned_quad& quad) {
    line line(ctx.root->dimensions.x);
    word word;

    float space_width = 0.0, xpos = 0.0, ypos = 0.0;
    getCharQuad(' ', *ctx.root->font, &quad, &space_width, &ypos);
    space_width *= ctx.text_scale;

    for (unsigned int i = 0, len = ctx.root->value.size(); i < len; i++) {
        char c = ctx.root->value.at(i);
        if (c == ' ') {
            if (!try_add_word(line, word)) {
                line.width -= space_width;
                lines->emplace_back(font::internal::line(line));
                line = font::internal::line(ctx.root->dimensions.x); // TODO test the assignment operator. i'm worried it's operating like a copy.
                try_add_word(line, word);
            }
            word = font::internal::word();
            line.width += space_width;
            continue;
        }

        xpos = 0.0;
        c = ctx.root->value.at(i);
        getCharQuad(c, *ctx.root->font, &quad, &xpos, &ypos);
        xpos *= ctx.text_scale;
        add_char(word, c, xpos);
    }

    if (!try_add_word(line, word)) {
        lines->emplace_back(font::internal::line(line));
        line = font::internal::line(ctx.root->dimensions.x);
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

const mesh_attrib* font::internal::find_attrib(MeshAttribType type, const std::unordered_map<MeshAttribType, mesh_attrib>* attribs) {
    if (attribs->find(type) == attribs->end())
        return nullptr;
    return &attribs->at(type);
}

void font::internal::store_quad_position(const stbtt_aligned_quad& quad, const mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
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

void font::internal::store_quad_uv(const stbtt_aligned_quad& quad, const mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
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

void font::internal::store_color(const vec3& color, const mesh_attrib& attrib, float* vbuf, unsigned int* vcount) {
    vbuf[attrib.offset + attrib.stride * 0 + 0] = color.x;
    vbuf[attrib.offset + attrib.stride * 0 + 1] = color.y;
    vbuf[attrib.offset + attrib.stride * 0 + 2] = color.z;
    vbuf[attrib.offset + attrib.stride * 1 + 0] = color.x;
    vbuf[attrib.offset + attrib.stride * 1 + 1] = color.y;
    vbuf[attrib.offset + attrib.stride * 1 + 2] = color.z;
    vbuf[attrib.offset + attrib.stride * 2 + 0] = color.x;
    vbuf[attrib.offset + attrib.stride * 2 + 1] = color.y;
    vbuf[attrib.offset + attrib.stride * 2 + 2] = color.z;
    vbuf[attrib.offset + attrib.stride * 3 + 0] = color.x;
    vbuf[attrib.offset + attrib.stride * 3 + 1] = color.y;
    vbuf[attrib.offset + attrib.stride * 3 + 2] = color.z;
    *vcount += 12;
}

