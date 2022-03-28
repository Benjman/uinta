#include <gtest/gtest.h>
#include <font.hpp>

TEST(font, renderableCount) {
    // ascii chars from 33 to 126
    char allchars[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    ASSERT_EQ(94, getRenderableCharCount(allchars, sizeof(allchars)));

    char boarder_chars[] = {32, 33, 126, 127};
    ASSERT_EQ(2, getRenderableCharCount(boarder_chars, sizeof(boarder_chars)));

    char outside_chars[] = {32, 127};
    ASSERT_EQ(0, getRenderableCharCount(outside_chars, sizeof(outside_chars)));
}
