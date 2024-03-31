#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_

#include <functional>

#include "uinta/gl.h"

namespace uinta {

struct MockOpenGLApi : OpenGLApi {
  std::function<void(GLbitfield)> onClear = [](auto) {};
  void clear(GLbitfield mask) const noexcept override { onClear(mask); }

  std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> onClearColor =
      [](auto, auto, auto, auto) {};
  void clearColor(GLfloat r, GLfloat g, GLfloat b,
                  GLfloat a) const noexcept override {
    onClearColor(r, g, b, a);
  }

  std::function<void(GLenum)> onDisable = [](auto) {};
  void disable(GLenum cap) const noexcept override { onDisable(cap); }

  std::function<void(GLenum, GLuint)> onDisablei = [](auto, auto) {};
  void disablei(GLenum cap, GLuint index) const noexcept override {
    onDisablei(cap, index);
  }

  std::function<void(GLenum, GLint, GLsizei)> onDrawArrays = [](auto, auto,
                                                                auto) {};
  void drawArrays(GLenum mode, GLint first,
                  GLsizei count) const noexcept override {
    onDrawArrays(mode, first, count);
  }

  std::function<void(GLenum, GLint, GLsizei, GLsizei)> onDrawArraysInstanced =
      [](auto, auto, auto, auto) {};
  void drawArraysInstanced(GLenum mode, GLint first, GLsizei count,
                           GLsizei instancecount) const noexcept override {
    onDrawArraysInstanced(mode, first, count, instancecount);
  }

  std::function<void(GLenum, GLsizei, GLenum, const void*)> onDrawElements =
      [](auto, auto, auto, const auto*) {};
  void drawElements(GLenum mode, GLsizei count, GLenum type,
                    const void* indices) const noexcept override {
    onDrawElements(mode, count, type, indices);
  }

  std::function<void(GLenum, GLuint, GLuint, GLsizei, GLenum, const void*)>
      onDrawRangeElements = [](auto, auto, auto, auto, auto, const auto*) {};
  void drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count,
                         GLenum type,
                         const void* indices) const noexcept override {
    onDrawRangeElements(mode, start, end, count, type, indices);
  }

  std::function<void(GLenum)> onEnable = [](auto) {};
  void enable(GLenum cap) const noexcept override { onEnable(cap); }

  std::function<void(GLenum, GLuint)> onEnablei = [](auto, auto) {};
  void enablei(GLenum cap, GLuint index) const noexcept override {
    onEnablei(cap, index);
  }

  std::function<const GLubyte*(GLenum)> onErrorString =
      [](auto) -> const GLubyte* {
    constexpr const char* message = "";
    return reinterpret_cast<const GLubyte*>(message);
  };
  const GLubyte* errorString(GLenum error) const noexcept override {
    return onErrorString(error);
  }

  std::function<void(GLenum, GLboolean*)> onGetBooleanv = [](auto, auto*) {};
  void getBooleanv(GLenum pname, GLboolean* data) const noexcept override {
    onGetBooleanv(pname, data);
  }

  std::function<void(GLenum, GLdouble*)> onGetDoublev = [](auto, auto*) {};
  void getDoublev(GLenum pname, GLdouble* data) const noexcept override {
    onGetDoublev(pname, data);
  }

  std::function<void(GLenum, GLfloat*)> onGetFloatv = [](auto, auto*) {};
  void getFloatv(GLenum pname, GLfloat* data) const noexcept override {
    onGetFloatv(pname, data);
  }

  std::function<void(GLenum, GLuint, GLint64*)> onGetInteger64i_v =
      [](auto, auto, auto*) {};
  void getInteger64i_v(GLenum target, GLuint index,
                       GLint64* data) const noexcept override {
    onGetInteger64i_v(target, index, data);
  }

  std::function<void(GLenum, GLint64*)> onGetInteger64v = [](auto, auto*) {};
  void getInteger64v(GLenum pname, GLint64* data) const noexcept override {
    onGetInteger64v(pname, data);
  }

  std::function<void(GLenum, GLuint, GLint*)> onGetIntegeri_v = [](auto, auto,
                                                                   auto*) {};
  void getIntegeri_v(GLenum target, GLuint index,
                     GLint* data) const noexcept override {
    onGetIntegeri_v(target, index, data);
  }

  std::function<void(GLenum, GLint*)> onGetIntegerv = [](auto, auto*) {};
  void getIntegerv(GLenum pname, GLint* data) const noexcept override {
    onGetIntegerv(pname, data);
  }

  std::function<bool(GLenum)> onIsEnabled = [](auto) -> bool { return true; };
  bool isEnabled(GLenum cap) const noexcept override {
    return onIsEnabled(cap);
  }

  std::function<bool(GLenum, GLuint)> onIsEnabledi = [](auto, auto) -> bool {
    return true;
  };
  bool isEnabledi(GLenum cap, GLuint index) const noexcept override {
    return onIsEnabledi(cap, index);
    return false;
  }

  std::function<void(GLfloat)> onLineWidth = [](auto) {};
  void lineWidth(GLfloat width) const noexcept override { onLineWidth(width); }

  std::function<void(GLfloat)> onPointSize = [](auto) {};
  void pointSize(GLfloat size) const noexcept override { onPointSize(size); }

  std::function<void(GLuint, GLuint, GLuint, GLuint)> onViewport =
      [](auto, auto, auto, auto) {};
  void viewport(GLint x, GLint y, GLsizei width,
                GLsizei height) const noexcept override {
    onViewport(x, y, width, height);
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_
