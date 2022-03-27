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

void load_font(const unsigned char* ttfdata, stbtt_packedchar* chardata, GLuint *texId) {
    const unsigned int width = 256,
                       height = 256;
    unsigned char bitmap[width * height];

    stbtt_pack_context ctx;
    if (!stbtt_PackBegin(&ctx, bitmap, width, height, 0, 1, nullptr))
        printf("some kinda error happened with stbtt_PackBegin\n");

    stbtt_packedchar chars[96];
    stbtt_PackFontRange(&ctx, ttfdata, 0, 32.0, 32, 95, chars);
    stbtt_PackEnd(&ctx);

    // TODO invert bitmap so we don't have to do vertex trickery

    glGenTextures(1, texId);
    glBindTexture(GL_TEXTURE_2D, *texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_write_png("/tmp/test.png", width, height, 1, bitmap, 0);
}
