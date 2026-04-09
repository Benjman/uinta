#include "uinta/texture.h"

#include <gtest/gtest.h>

#include <vector>

#include "uinta/mock/mock_gl.h"
#include "uinta/mock/mock_stb_image_api.h"

namespace uinta {

// ============================================================================
// Texture tests
// ============================================================================

class TextureTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  MockImageApi imageApi;
};

// ----------------------------------------------------------------------------
// Constructor tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, ConstructorCallsGenTextures) {
  bool genTexturesCalled = false;
  GLuint expId = 42;

  gl.onGenTextures = [&](GLsizei count, GLuint* id) {
    EXPECT_EQ(1, count);
    *id = expId;
    genTexturesCalled = true;
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  EXPECT_TRUE(genTexturesCalled);
  EXPECT_EQ(expId, texture.id());
}

TEST_F(TextureTest, ConstructorBindsTextureOnCreation) {
  GLuint expId = 55;
  std::vector<std::pair<GLenum, GLuint>> bindCalls;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onBindTexture = [&](GLenum target, GLuint id) {
    bindCalls.emplace_back(target, id);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  // TextureGuard binds and unbinds in constructor
  ASSERT_EQ(2u, bindCalls.size());
  EXPECT_EQ(GL_TEXTURE_2D, bindCalls[0].first);
  EXPECT_EQ(expId, bindCalls[0].second);
  EXPECT_EQ(GL_TEXTURE_2D, bindCalls[1].first);
  EXPECT_EQ(0u, bindCalls[1].second);
}

TEST_F(TextureTest, ConstructorWithDimensionsCallsTexImage2D) {
  bool texImage2DCalled = false;
  i32 expWidth = 256;
  i32 expHeight = 128;

  gl.onTexImage2D = [&](GLenum target, GLint level, GLint internalformat,
                        GLsizei width, GLsizei height, GLint border,
                        GLenum format, GLenum type, const void* data) {
    texImage2DCalled = true;
    EXPECT_EQ(GL_TEXTURE_2D, target);
    EXPECT_EQ(0, level);
    EXPECT_EQ(GL_RGB, internalformat);
    EXPECT_EQ(expWidth, width);
    EXPECT_EQ(expHeight, height);
    EXPECT_EQ(0, border);
    EXPECT_EQ(GL_RGB, format);
    EXPECT_EQ(GL_UNSIGNED_BYTE, type);
    EXPECT_EQ(nullptr, data);
  };

  Texture texture(GL_TEXTURE_2D, expWidth, expHeight, 0, 0, 0, &gl);

  EXPECT_TRUE(texImage2DCalled);
  EXPECT_EQ(expWidth, texture.width());
  EXPECT_EQ(expHeight, texture.height());
}

TEST_F(TextureTest, ConstructorWithZeroDimensionsSkipsTexImage2D) {
  bool texImage2DCalled = false;

  gl.onTexImage2D = [&](GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
                        GLenum, const void*) { texImage2DCalled = true; };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  EXPECT_FALSE(texImage2DCalled);
}

TEST_F(TextureTest, ConstructorWithFilterCallsTexParameteri) {
  std::vector<std::tuple<GLenum, GLenum, GLint>> texParamCalls;
  GLint expFilter = GL_LINEAR;

  gl.onTexParameteri = [&](GLenum target, GLenum pname, GLint param) {
    texParamCalls.emplace_back(target, pname, param);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, expFilter, 0, &gl);

  // Should have MIN_FILTER and MAG_FILTER calls
  ASSERT_GE(texParamCalls.size(), 2u);

  bool foundMinFilter = false;
  bool foundMagFilter = false;
  for (const auto& call : texParamCalls) {
    if (std::get<1>(call) == GL_TEXTURE_MIN_FILTER) {
      foundMinFilter = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expFilter, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_MAG_FILTER) {
      foundMagFilter = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expFilter, std::get<2>(call));
    }
  }
  EXPECT_TRUE(foundMinFilter);
  EXPECT_TRUE(foundMagFilter);
}

TEST_F(TextureTest, ConstructorWithZeroFilterSkipsFiltering) {
  std::vector<GLenum> texParamPnames;

  gl.onTexParameteri = [&](GLenum, GLenum pname, GLint) {
    texParamPnames.push_back(pname);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  for (const auto& pname : texParamPnames) {
    EXPECT_NE(GL_TEXTURE_MIN_FILTER, pname);
    EXPECT_NE(GL_TEXTURE_MAG_FILTER, pname);
  }
}

TEST_F(TextureTest, ConstructorWithWrapCallsTexParameteri) {
  std::vector<std::tuple<GLenum, GLenum, GLint>> texParamCalls;
  GLint expWrap = GL_REPEAT;

  gl.onTexParameteri = [&](GLenum target, GLenum pname, GLint param) {
    texParamCalls.emplace_back(target, pname, param);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, expWrap, &gl);

  bool foundWrapS = false;
  bool foundWrapT = false;
  for (const auto& call : texParamCalls) {
    if (std::get<1>(call) == GL_TEXTURE_WRAP_S) {
      foundWrapS = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_WRAP_T) {
      foundWrapT = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
  }
  EXPECT_TRUE(foundWrapS);
  EXPECT_TRUE(foundWrapT);
}

TEST_F(TextureTest, ConstructorWithZeroWrapSkipsWrapping) {
  std::vector<GLenum> texParamPnames;

  gl.onTexParameteri = [&](GLenum, GLenum pname, GLint) {
    texParamPnames.push_back(pname);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  for (const auto& pname : texParamPnames) {
    EXPECT_NE(GL_TEXTURE_WRAP_S, pname);
    EXPECT_NE(GL_TEXTURE_WRAP_T, pname);
    EXPECT_NE(GL_TEXTURE_WRAP_R, pname);
  }
}

TEST_F(TextureTest, ConstructorStoresTarget) {
  Texture texture2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  EXPECT_EQ(GL_TEXTURE_2D, texture2D.target());

  Texture texture3D(GL_TEXTURE_3D, 0, 0, 0, 0, 0, &gl);
  EXPECT_EQ(GL_TEXTURE_3D, texture3D.target());
}

TEST_F(TextureTest, ConstructorStoresChannels) {
  i32 expChannels = 4;
  Texture texture(GL_TEXTURE_2D, 0, 0, expChannels, 0, 0, &gl);
  EXPECT_EQ(expChannels, texture.channels());
}

// ----------------------------------------------------------------------------
// Destructor tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, DestructorCallsDeleteTextures) {
  bool deleteTexturesCalled = false;
  GLuint expId = 77;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onDeleteTextures = [&](GLsizei count, const GLuint* id) {
    deleteTexturesCalled = true;
    EXPECT_EQ(1, count);
    EXPECT_EQ(expId, *id);
  };

  {
    Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
    EXPECT_EQ(expId, texture.id());
  }

  EXPECT_TRUE(deleteTexturesCalled);
}

// ----------------------------------------------------------------------------
// fromFile tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, FromFileReturnsNotFoundForNonExistent) {
  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  auto status = texture.fromFile("/nonexistent/texture.png", &imageApi);

  EXPECT_FALSE(status.ok());
  EXPECT_TRUE(IsNotFound(status));
}

// Note: Testing fromFile success path requires filesystem setup which is
// covered in integration tests. The mock pattern here focuses on unit behavior.

// ----------------------------------------------------------------------------
// filtering() method tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, FilteringCallsTexParameteriForMinAndMag) {
  std::vector<std::tuple<GLenum, GLenum, GLint>> texParamCalls;
  GLint expMin = GL_NEAREST;
  GLint expMag = GL_LINEAR;

  gl.onTexParameteri = [&](GLenum target, GLenum pname, GLint param) {
    texParamCalls.emplace_back(target, pname, param);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  texParamCalls.clear();

  texture.filtering(expMin, expMag);

  ASSERT_EQ(2u, texParamCalls.size());

  // Find MIN_FILTER call
  bool foundMin = false;
  bool foundMag = false;
  for (const auto& call : texParamCalls) {
    if (std::get<1>(call) == GL_TEXTURE_MIN_FILTER) {
      foundMin = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expMin, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_MAG_FILTER) {
      foundMag = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expMag, std::get<2>(call));
    }
  }
  EXPECT_TRUE(foundMin);
  EXPECT_TRUE(foundMag);
}

TEST_F(TextureTest, FilteringUpdatesGetters) {
  GLint expMin = GL_NEAREST_MIPMAP_LINEAR;
  GLint expMag = GL_LINEAR;

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  texture.filtering(expMin, expMag);

  EXPECT_EQ(expMin, texture.filterMin());
  EXPECT_EQ(expMag, texture.filterMag());
}

// ----------------------------------------------------------------------------
// wrapping() method tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, WrappingCallsTexParameteriForSAndT) {
  std::vector<std::tuple<GLenum, GLenum, GLint>> texParamCalls;
  GLint expWrap = GL_CLAMP_TO_EDGE;

  gl.onTexParameteri = [&](GLenum target, GLenum pname, GLint param) {
    texParamCalls.emplace_back(target, pname, param);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  texParamCalls.clear();

  texture.wrapping(expWrap);

  ASSERT_EQ(2u, texParamCalls.size());

  bool foundWrapS = false;
  bool foundWrapT = false;
  for (const auto& call : texParamCalls) {
    if (std::get<1>(call) == GL_TEXTURE_WRAP_S) {
      foundWrapS = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_WRAP_T) {
      foundWrapT = true;
      EXPECT_EQ(GL_TEXTURE_2D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
  }
  EXPECT_TRUE(foundWrapS);
  EXPECT_TRUE(foundWrapT);
}

TEST_F(TextureTest, Wrapping3DCallsTexParameteriForSAndTAndR) {
  std::vector<std::tuple<GLenum, GLenum, GLint>> texParamCalls;
  GLint expWrap = GL_MIRRORED_REPEAT;

  gl.onTexParameteri = [&](GLenum target, GLenum pname, GLint param) {
    texParamCalls.emplace_back(target, pname, param);
  };

  Texture texture(GL_TEXTURE_3D, 0, 0, 0, 0, 0, &gl);
  texParamCalls.clear();

  texture.wrapping(expWrap);

  ASSERT_EQ(3u, texParamCalls.size());

  bool foundWrapS = false;
  bool foundWrapT = false;
  bool foundWrapR = false;
  for (const auto& call : texParamCalls) {
    if (std::get<1>(call) == GL_TEXTURE_WRAP_S) {
      foundWrapS = true;
      EXPECT_EQ(GL_TEXTURE_3D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_WRAP_T) {
      foundWrapT = true;
      EXPECT_EQ(GL_TEXTURE_3D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
    if (std::get<1>(call) == GL_TEXTURE_WRAP_R) {
      foundWrapR = true;
      EXPECT_EQ(GL_TEXTURE_3D, std::get<0>(call));
      EXPECT_EQ(expWrap, std::get<2>(call));
    }
  }
  EXPECT_TRUE(foundWrapS);
  EXPECT_TRUE(foundWrapT);
  EXPECT_TRUE(foundWrapR);
}

TEST_F(TextureTest, WrappingUpdatesGetter) {
  GLint expWrap = GL_REPEAT;

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  texture.wrapping(expWrap);

  EXPECT_EQ(expWrap, texture.wrap());
}

// ----------------------------------------------------------------------------
// Getter tests
// ----------------------------------------------------------------------------

TEST_F(TextureTest, IdGetter) {
  GLuint expId = 123;
  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);

  EXPECT_EQ(expId, texture.id());
}

TEST_F(TextureTest, TargetGetter) {
  Texture texture(GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0, 0, &gl);
  EXPECT_EQ(GL_TEXTURE_CUBE_MAP, texture.target());
}

TEST_F(TextureTest, DimensionGetters) {
  i32 expWidth = 512;
  i32 expHeight = 256;

  Texture texture(GL_TEXTURE_2D, expWidth, expHeight, 0, 0, 0, &gl);

  EXPECT_EQ(expWidth, texture.width());
  EXPECT_EQ(expHeight, texture.height());
}

TEST_F(TextureTest, ChannelsGetter) {
  i32 expChannels = 3;

  Texture texture(GL_TEXTURE_2D, 0, 0, expChannels, 0, 0, &gl);

  EXPECT_EQ(expChannels, texture.channels());
}

TEST_F(TextureTest, FilterGettersAfterConstruction) {
  GLint expFilter = GL_LINEAR_MIPMAP_LINEAR;

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, expFilter, 0, &gl);

  EXPECT_EQ(expFilter, texture.filterMin());
  EXPECT_EQ(expFilter, texture.filterMag());
}

TEST_F(TextureTest, WrapGetterAfterConstruction) {
  GLint expWrap = GL_CLAMP_TO_BORDER;

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, expWrap, &gl);

  EXPECT_EQ(expWrap, texture.wrap());
}

// ============================================================================
// ActiveTextureGuard tests
// ============================================================================

class ActiveTextureGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(ActiveTextureGuardTest, ConstructorGetsAndSetsActiveTexture) {
  bool getIntegervCalled = false;
  bool activeTextureCalled = false;
  GLint prevUnit = GL_TEXTURE3;
  GLenum newUnit = GL_TEXTURE5;
  GLenum capturedUnit = 0;

  gl.onGetIntegerv = [&](GLenum pname, GLint* data) {
    if (pname == GL_ACTIVE_TEXTURE) {
      getIntegervCalled = true;
      *data = prevUnit;
    }
  };
  gl.onActiveTexture = [&](GLenum texture) {
    activeTextureCalled = true;
    capturedUnit = texture;
  };

  ActiveTextureGuard guard(newUnit, &gl);

  EXPECT_TRUE(getIntegervCalled);
  EXPECT_TRUE(activeTextureCalled);
  EXPECT_EQ(newUnit, capturedUnit);
}

TEST_F(ActiveTextureGuardTest, DestructorRestoresPreviousActiveTexture) {
  std::vector<GLenum> activeTextureCalls;
  GLint prevUnit = GL_TEXTURE2;
  GLenum newUnit = GL_TEXTURE7;

  gl.onGetIntegerv = [&](GLenum pname, GLint* data) {
    if (pname == GL_ACTIVE_TEXTURE) {
      *data = prevUnit;
    }
  };
  gl.onActiveTexture = [&](GLenum texture) {
    activeTextureCalls.push_back(texture);
  };

  {
    ActiveTextureGuard guard(newUnit, &gl);
    ASSERT_EQ(1u, activeTextureCalls.size());
    EXPECT_EQ(newUnit, activeTextureCalls[0]);
  }

  ASSERT_EQ(2u, activeTextureCalls.size());
  EXPECT_EQ(static_cast<GLenum>(prevUnit), activeTextureCalls[1]);
}

TEST_F(ActiveTextureGuardTest, DefaultTextureIsTexture0) {
  GLenum capturedUnit = GL_NONE;

  gl.onActiveTexture = [&](GLenum texture) { capturedUnit = texture; };

  ActiveTextureGuard guard(GL_TEXTURE0, &gl);

  EXPECT_EQ(GL_TEXTURE0, capturedUnit);
}

// ============================================================================
// TextureGuard tests
// ============================================================================

class TextureGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(TextureGuardTest, ConstructorBindsTexture) {
  GLuint expId = 88;
  std::vector<std::pair<GLenum, GLuint>> bindCalls;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onBindTexture = [&](GLenum target, GLuint id) {
    bindCalls.emplace_back(target, id);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  bindCalls.clear();

  TextureGuard guard(&texture);

  ASSERT_EQ(1u, bindCalls.size());
  EXPECT_EQ(GL_TEXTURE_2D, bindCalls[0].first);
  EXPECT_EQ(expId, bindCalls[0].second);
}

TEST_F(TextureGuardTest, DestructorUnbindsTexture) {
  GLuint expId = 99;
  std::vector<std::pair<GLenum, GLuint>> bindCalls;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onBindTexture = [&](GLenum target, GLuint id) {
    bindCalls.emplace_back(target, id);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  bindCalls.clear();

  {
    TextureGuard guard(&texture);
    ASSERT_EQ(1u, bindCalls.size());
  }

  ASSERT_EQ(2u, bindCalls.size());
  EXPECT_EQ(GL_TEXTURE_2D, bindCalls[1].first);
  EXPECT_EQ(0u, bindCalls[1].second);
}

TEST_F(TextureGuardTest, ScopedUsagePattern) {
  GLuint expId = 111;
  std::vector<std::pair<GLenum, GLuint>> bindCalls;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onBindTexture = [&](GLenum target, GLuint id) {
    bindCalls.emplace_back(target, id);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  bindCalls.clear();

  {
    TextureGuard guard(&texture);
    // Texture should be bound here
  }
  // Texture should be unbound here

  ASSERT_EQ(2u, bindCalls.size());
  EXPECT_EQ(expId, bindCalls[0].second);
  EXPECT_EQ(0u, bindCalls[1].second);
}

TEST_F(TextureGuardTest, UsesTextureTarget) {
  std::vector<GLenum> boundTargets;

  gl.onBindTexture = [&](GLenum target, GLuint) {
    boundTargets.push_back(target);
  };

  Texture texture3D(GL_TEXTURE_3D, 0, 0, 0, 0, 0, &gl);
  boundTargets.clear();

  {
    TextureGuard guard(&texture3D);
  }

  ASSERT_EQ(2u, boundTargets.size());
  EXPECT_EQ(GL_TEXTURE_3D, boundTargets[0]);
  EXPECT_EQ(GL_TEXTURE_3D, boundTargets[1]);
}

TEST_F(TextureGuardTest, MultipleGuardsInSequence) {
  GLuint expId = 222;
  std::vector<std::pair<GLenum, GLuint>> bindCalls;

  gl.onGenTextures = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onBindTexture = [&](GLenum target, GLuint id) {
    bindCalls.emplace_back(target, id);
  };

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, &gl);
  bindCalls.clear();

  {
    TextureGuard guard1(&texture);
  }
  {
    TextureGuard guard2(&texture);
  }

  ASSERT_EQ(4u, bindCalls.size());
  EXPECT_EQ(expId, bindCalls[0].second);
  EXPECT_EQ(0u, bindCalls[1].second);
  EXPECT_EQ(expId, bindCalls[2].second);
  EXPECT_EQ(0u, bindCalls[3].second);
}

}  // namespace uinta
