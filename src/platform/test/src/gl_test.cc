#include "uinta/gl.h"

#include <gtest/gtest.h>

namespace uinta {

class OpenGLApiTestF : public ::testing::Test {
 protected:
};

// GetTypeString tests

TEST_F(OpenGLApiTestF, GetTypeStringError) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_ERROR), "Error");
}

TEST_F(OpenGLApiTestF, GetTypeStringDeprecatedBehavior) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR),
            "Deprecated Behavior");
}

TEST_F(OpenGLApiTestF, GetTypeStringUndefinedBehavior) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR),
            "Undefined Behavior");
}

TEST_F(OpenGLApiTestF, GetTypeStringPortability) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_PORTABILITY), "Portability");
}

TEST_F(OpenGLApiTestF, GetTypeStringPerformance) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_PERFORMANCE), "Performance");
}

TEST_F(OpenGLApiTestF, GetTypeStringOther) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_OTHER), "Other");
}

TEST_F(OpenGLApiTestF, GetTypeStringMarker) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_MARKER), "Marker");
}

TEST_F(OpenGLApiTestF, GetTypeStringPushGroup) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_PUSH_GROUP), "Push Group");
}

TEST_F(OpenGLApiTestF, GetTypeStringPopGroup) {
  EXPECT_EQ(OpenGLApi::GetTypeString(GL_DEBUG_TYPE_POP_GROUP), "Pop Group");
}

TEST_F(OpenGLApiTestF, GetTypeStringUnknown) {
  constexpr GLenum unknownType = 0xFFFF;
  std::string result = OpenGLApi::GetTypeString(unknownType);
  EXPECT_TRUE(result.find("Unknown type:") != std::string::npos);
}

// GetSourceString tests

TEST_F(OpenGLApiTestF, GetSourceStringApi) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_API), "API");
}

TEST_F(OpenGLApiTestF, GetSourceStringWindowSystem) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_WINDOW_SYSTEM),
            "Window System");
}

TEST_F(OpenGLApiTestF, GetSourceStringShaderCompiler) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_SHADER_COMPILER),
            "Shader Compiler");
}

TEST_F(OpenGLApiTestF, GetSourceStringThirdParty) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_THIRD_PARTY),
            "Third Party");
}

TEST_F(OpenGLApiTestF, GetSourceStringApplication) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_APPLICATION),
            "Application");
}

TEST_F(OpenGLApiTestF, GetSourceStringOther) {
  EXPECT_EQ(OpenGLApi::GetSourceString(GL_DEBUG_SOURCE_OTHER), "Other");
}

TEST_F(OpenGLApiTestF, GetSourceStringUnknown) {
  constexpr GLenum unknownSource = 0xFFFF;
  std::string result = OpenGLApi::GetSourceString(unknownSource);
  EXPECT_TRUE(result.find("Unknown source:") != std::string::npos);
}

// GetSeverityString tests

TEST_F(OpenGLApiTestF, GetSeverityStringHigh) {
  EXPECT_EQ(OpenGLApi::GetSeverityString(GL_DEBUG_SEVERITY_HIGH), "High");
}

TEST_F(OpenGLApiTestF, GetSeverityStringMedium) {
  EXPECT_EQ(OpenGLApi::GetSeverityString(GL_DEBUG_SEVERITY_MEDIUM), "Medium");
}

TEST_F(OpenGLApiTestF, GetSeverityStringLow) {
  EXPECT_EQ(OpenGLApi::GetSeverityString(GL_DEBUG_SEVERITY_LOW), "Low");
}

TEST_F(OpenGLApiTestF, GetSeverityStringNotification) {
  EXPECT_EQ(OpenGLApi::GetSeverityString(GL_DEBUG_SEVERITY_NOTIFICATION),
            "Notification");
}

TEST_F(OpenGLApiTestF, GetSeverityStringUnknown) {
  constexpr GLenum unknownSeverity = 0xFFFF;
  std::string result = OpenGLApi::GetSeverityString(unknownSeverity);
  EXPECT_TRUE(result.find("Unknown severity:") != std::string::npos);
}

}  // namespace uinta
