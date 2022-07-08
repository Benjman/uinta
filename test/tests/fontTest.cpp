#include <uinta/font.hpp>

#include <gtest/gtest.h>

using namespace font;

TEST(font, renderableCount) {
  char renderableChars[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
  ASSERT_EQ(94, getRenderableCharCount(renderableChars));
}

TEST(font, renderableCount_boarders) {
  char boarder_chars[] = {'!' - 1, '!', '~', '~' + 1};
  ASSERT_EQ(2, getRenderableCharCount(boarder_chars));
}
