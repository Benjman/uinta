#include <glad/glad.h>

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

void load_font(const unsigned char* ttfdata, GLuint *textureid,
               const unsigned int texture_width, const unsigned int texture_height,
               stbtt_pack_context &ctx, stbtt_packedchar* chardata) {
    unsigned char bitmap[texture_width * texture_height];

    if (!stbtt_PackBegin(&ctx, bitmap, texture_width, texture_height, 0, 1, nullptr))
        printf("some kinda error happened with stbtt_PackBegin\n");

    stbtt_PackFontRange(&ctx, ttfdata, 0, 32.0, 32, 95, chardata);
    stbtt_PackEnd(&ctx);

    // flip vertically to comply with OpenGL texture coordinates
    stbi__vertical_flip(bitmap, texture_width, texture_height, 1);

    glGenTextures(1, textureid);
    glBindTexture(GL_TEXTURE_2D, *textureid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_width, texture_height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_write_png("/tmp/test.png", texture_width, texture_height, 1, bitmap, 0);
}
