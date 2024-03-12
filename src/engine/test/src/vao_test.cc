#include "uinta/vao.h"

#include <gtest/gtest.h>

#include "./mock/mock_gl.h"

namespace uinta {

TEST(Vao, construct_raii) {
  MockOpenGLApi api;
  GLuint expId = 13;
  api.onGenVertexArrays = [&](GLuint, GLuint* id) { *id = expId; };
  Vao vao(&api);
  ASSERT_EQ(expId, vao.id());
}

TEST(Vao, destructor) {
  MockOpenGLApi api;
  bool onDeleteVertexArraysCalled = false;
  api.onDeleteVertexArrays = [&](GLuint, GLuint*) {
    onDeleteVertexArraysCalled = true;
  };
  { Vao vao(&api); }
  ASSERT_TRUE(onDeleteVertexArraysCalled);
}

// TEST(Vao, linkAttribute_binding) {
//   MockOpenGLApi api;
//   bool bound = false;
//   api.onBindVertexArray = [&](GLuint) { bound = true; };
//   Vao vao(&api);
//   ASSERT_FALSE(bound);
//   vao.linkAttribute({});
//   ASSERT_TRUE(bound);
// }

}  // namespace uinta
