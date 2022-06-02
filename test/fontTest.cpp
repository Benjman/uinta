#include <uinta/font.hpp>

#include <gtest/gtest.h>

TEST(font, renderableCount) {
    // ascii chars from 33 to 126
    char renderableChars[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    ASSERT_EQ(94, font::getRenderableCharCount(renderableChars, sizeof(renderableChars)));

    char boarder_chars[] = {32, 33, 126, 127};
    ASSERT_EQ(2, font::getRenderableCharCount(boarder_chars, sizeof(boarder_chars)));

    char outside_chars[] = {32, 127};
    ASSERT_EQ(0, font::getRenderableCharCount(outside_chars, sizeof(outside_chars)));
}
