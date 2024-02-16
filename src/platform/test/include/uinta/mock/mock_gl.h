#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_

#include <functional>

#include "uinta/gl.h"

namespace uinta {

struct MockOpenGLApi : OpenGLApi {
  std::function<void(GLuint, GLuint)> onAttachShader = [](auto, auto) {};
  void attachShader(GLuint program, GLuint shader) const noexcept override {
    onAttachShader(program, shader);
  }

  std::function<void(GLenum, GLuint)> onBindBuffer = [](auto, auto) {};
  void bindBuffer(GLenum target, GLuint id) const noexcept override {
    onBindBuffer(target, id);
  }

  std::function<void(GLenum, GLsizeiptr, const void*, GLenum)> onBufferData =
      [](auto, auto, const auto*, auto) {};
  void bufferData(GLenum target, GLsizeiptr size, const void* data,
                  GLenum usage) const noexcept override {
    onBufferData(target, size, data, usage);
  }

  std::function<void(GLenum, GLintptr, GLsizeiptr, const void*)>
      onBufferSubData = [](auto, auto, auto, const auto*) {};
  void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                     const void* data) const noexcept override {
    onBufferSubData(target, offset, size, data);
  }

  std::function<void(GLbitfield)> onClear = [](auto) {};
  void clear(GLbitfield mask) const noexcept override { onClear(mask); }

  std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> onClearColor =
      [](auto, auto, auto, auto) {};
  void clearColor(GLfloat r, GLfloat g, GLfloat b,
                  GLfloat a) const noexcept override {
    onClearColor(r, g, b, a);
  }

  std::function<void(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr)>
      onCopyBufferSubData = [](auto, auto, auto, auto, auto) {};
  void copyBufferSubData(GLenum readTarget, GLenum writeTarget,
                         GLintptr readOffset, GLintptr writeOffset,
                         GLsizeiptr size) const noexcept override {
    onCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
  }

  std::function<void(GLuint, GLuint, GLintptr, GLintptr, GLsizeiptr)>
      onCopyNamedBufferSubData = [](auto, auto, auto, auto, auto) {};
  void copyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer,
                              GLintptr readOffset, GLintptr writeOffset,
                              GLsizeiptr size) const noexcept override {
    onCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset,
                             size);
  }

  std::function<void(GLuint)> onCompileShader = [](auto) {};
  void compileShader(GLuint shader) const noexcept override {
    onCompileShader(shader);
  }

  std::function<GLuint()> onCreateProgram = []() -> GLuint { return 1; };
  GLuint createProgram() const noexcept override { return onCreateProgram(); }

  std::function<GLuint(GLenum)> onCreateShader = [](auto) -> GLuint {
    return GL_ONE;
  };
  GLuint createShader(GLenum stage) const noexcept override {
    return onCreateShader(stage);
  }

  std::function<void(GLsizei, GLuint*)> onDeleteBuffers = [](auto, auto*) {};
  void deleteBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    onDeleteBuffers(count, ptr);
  }

  std::function<void(GLuint)> onDeleteProgram = [](auto) {};
  void deleteProgram(GLuint program) const noexcept override {
    onDeleteProgram(program);
  }

  std::function<void(GLuint)> onDeleteShader = [](auto) {};
  void deleteShader(GLuint program) const noexcept override {
    onDeleteShader(program);
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

  std::function<void(GLsizei, GLuint*)> onGenBuffers = [](auto count,
                                                          auto* idPtrs) {
    for (auto i = 0; i < count; i++) *idPtrs = i + 1;
  };
  void genBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    onGenBuffers(count, ptr);
  }

  std::function<void(GLuint, GLenum, GLint*)> onGetShaderiv =
      [](auto, auto, GLint* params) { *params = 1; };
  void getShaderiv(GLuint shader, GLenum pname,
                   GLint* params) const noexcept override {
    onGetShaderiv(shader, pname, params);
  }

  std::function<void(GLuint, GLenum, GLint*)> onGetProgramiv =
      [](auto, auto, auto* params) { *params = 1; };
  void getProgramiv(GLuint program, GLenum pname,
                    GLint* params) const noexcept override {
    onGetProgramiv(program, pname, params);
  }

  std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> onGetProgramInfoLog =
      [](auto, auto, auto*, auto*) {};
  void getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length,
                         GLchar* infoLog) const noexcept override {
    onGetProgramInfoLog(program, maxLength, length, infoLog);
  }

  std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> onGetShaderInfoLog =
      [](auto, auto, auto*, auto*) {};
  void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length,
                        GLchar* infoLog) const noexcept override {
    onGetShaderInfoLog(shader, maxLength, length, infoLog);
  }

  std::function<GLint(GLuint, const GLchar*)> onGetUniformLocation =
      [](auto, const auto*) -> GLint { return 1; };
  GLint getUniformLocation(GLuint program,
                           const GLchar* name) const noexcept override {
    return onGetUniformLocation(program, name);
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

  std::function<void(GLuint)> onLinkProgram = [](auto) {};
  void linkProgram(GLuint program) const noexcept override {
    onLinkProgram(program);
  }

  std::function<void(GLuint, GLintptr, GLsizeiptr, const void*)>
      onNamedBufferSubData = [](auto, auto, auto, const auto*) {};
  void namedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size,
                          const void* data) const noexcept override {
    onNamedBufferSubData(buffer, offset, size, data);
  }

  std::function<void(GLfloat)> onPointSize = [](auto) {};
  void pointSize(GLfloat size) const noexcept override { onPointSize(size); }

  std::function<void(GLuint, GLsizei, const GLchar**, const GLint*)>
      onShaderSource = [](auto, auto, const auto**, const auto*) {};
  void shaderSource(GLuint shader, GLsizei count, const GLchar** source,
                    const GLint* length) const noexcept override {
    onShaderSource(shader, count, source, length);
  }

  std::function<void(GLuint)> onUseProgram = [](auto) {};
  void useProgram(GLuint program) const noexcept override {
    onUseProgram(program);
  }

  std::function<void(GLuint, GLuint, GLuint, GLuint)> onViewport =
      [](auto, auto, auto, auto) {};
  void viewport(GLint x, GLint y, GLsizei width,
                GLsizei height) const noexcept override {
    onViewport(x, y, width, height);
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_
