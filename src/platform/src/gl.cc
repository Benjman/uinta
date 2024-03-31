#include "uinta/gl.h"

#include "uinta/lib/absl/strings.h"

namespace uinta {

std::string OpenGLApi::getSourceString(GLenum source) const noexcept {
  switch (source) {
    case GL_DEBUG_SOURCE_API:
      return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:
      return "Application";
    case GL_DEBUG_SOURCE_OTHER:
      return "Other";
    default:
      return StrCat("Unknown source: ", source);
  }
}

std::string OpenGLApi::getTypeString(GLenum type) const noexcept {
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      return "Deprecated Behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      return "Undefined Behavior";
    case GL_DEBUG_TYPE_PORTABILITY:
      return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
      return "Performance";
    case GL_DEBUG_TYPE_OTHER:
      return "Other";
    case GL_DEBUG_TYPE_MARKER:
      return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:
      return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP:
      return "Pop Group";
    default:
      return StrCat("Unknown type: ", type);
  }
}

std::string OpenGLApi::getSeverityString(GLenum severity) const noexcept {
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      return "High";
    case GL_DEBUG_SEVERITY_MEDIUM:
      return "Medium";
    case GL_DEBUG_SEVERITY_LOW:
      return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      return "Notification";
    default:
      return StrCat("Unknown severity: ", severity);
  }
}

}  // namespace uinta
