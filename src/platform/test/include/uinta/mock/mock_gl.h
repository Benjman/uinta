#ifndef SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_
#define SRC_PLATFORM_TEST_INCLUDE_UINTA_MOCK_MOCK_GL_H_

#include <functional>

#include "uinta/gl.h"

namespace uinta {

struct MockOpenGLApi : OpenGLApi {
  std::function<void(GLenum)> onActiveTexture = [](auto) {};
  void activeTexture(GLenum texture) const noexcept override {
    onActiveTexture(texture);
  }

  std::function<void(GLuint, GLuint)> onAttachShader = [](auto, auto) {};
  void attachShader(GLuint program, GLuint shader) const noexcept override {
    onAttachShader(program, shader);
  }

  std::function<void(GLenum, GLuint)> onBindBuffer = [](auto, auto) {};
  void bindBuffer(GLenum target, GLuint id) const noexcept override {
    onBindBuffer(target, id);
  }

  std::function<void(GLenum, GLuint)> onBindTexture = [](auto, auto) {};
  void bindTexture(GLenum target, GLuint id) const noexcept override {
    onBindTexture(target, id);
  }

  std::function<void(GLuint)> onBindVertexArray = [](auto) {};
  void bindVertexArray(GLuint id) const noexcept override {
    onBindVertexArray(id);
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

  std::function<void(GLsizei, const GLuint*)> onDeleteTextures =
      [](auto, const auto*) {};
  void deleteTextures(GLsizei count,
                      const GLuint* ptr) const noexcept override {
    onDeleteTextures(count, ptr);
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

  std::function<void(GLuint, GLuint)> onEnableVertexArrayAttrib = [](auto,
                                                                     auto) {};
  void enableVertexArrayAttrib(GLuint vaobj,
                               GLuint index) const noexcept override {
    onEnableVertexArrayAttrib(vaobj, index);
  }

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

  std::function<void(GLsizei, GLuint*)> onDeleteVertexArrays = [](auto, auto*) {
  };
  void deleteVertexArrays(GLsizei count, GLuint* ptr) const noexcept override {
    onDeleteVertexArrays(count, ptr);
  }

  std::function<void(GLuint)> onEnableVertexAttribArray = [](auto) {};
  void enableVertexAttribArray(GLuint index) const noexcept override {
    onEnableVertexAttribArray(index);
  }

  std::function<void(GLsizei, GLuint*)> onGenBuffers = [](auto count,
                                                          auto* idPtrs) {
    for (auto i = 0; i < count; i++) *idPtrs = i + 1;
  };
  void genBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    onGenBuffers(count, ptr);
  }

  std::function<void(GLenum)> onGenerateMipmap = [](auto) {};
  void generateMipmap(GLenum target) const noexcept override {
    onGenerateMipmap(target);
  }

  std::function<void(GLsizei, GLuint*)> onGenTextures = [](auto count,
                                                           auto* idPtrs) {
    for (auto i = 0; i < count; i++) *idPtrs = i + 1;
  };
  void genTextures(GLsizei count, GLuint* ptr) const noexcept override {
    onGenTextures(count, ptr);
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

  std::function<void(GLsizei, GLuint*)> onGenVertexArrays = [](auto count,
                                                               auto* idPtrs) {
    for (auto i = 0; i < count; i++) *idPtrs = i + 1;
  };
  void genVertexArrays(GLsizei count, GLuint* ptr) const noexcept override {
    onGenVertexArrays(count, ptr);
  }

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

  std::function<void(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
                     GLenum, const void*)>
      onTexImage2D = [](GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
                        GLenum, const void*) {};
  void texImage2D(GLenum target, GLint level, GLint internalformat,
                  GLsizei width, GLsizei height, GLint border, GLenum format,
                  GLenum type, const void* data) const noexcept override {
    onTexImage2D(target, level, internalformat, width, height, border, format,
                 type, data);
  }

  std::function<void(GLenum, GLenum, GLfloat)> onTexParameterf = [](auto, auto,
                                                                    auto) {};
  void texParameterf(GLenum target, GLenum pname,
                     GLfloat param) const noexcept override {
    onTexParameterf(target, pname, param);
  }

  std::function<void(GLenum, GLenum, GLint)> onTexParameteri = [](auto, auto,
                                                                  auto) {};
  void texParameteri(GLenum target, GLenum pname,
                     GLint param) const noexcept override {
    onTexParameteri(target, pname, param);
  }

  std::function<void(GLuint, GLenum, GLfloat)> onTextureParameterf =
      [](auto, auto, auto) {};
  void textureParameterf(GLuint texture, GLenum pname,
                         GLfloat param) const noexcept override {
    onTextureParameterf(texture, pname, param);
  }

  std::function<void(GLuint, GLenum, GLint)> onTextureParameteri =
      [](auto, auto, auto) {};
  void textureParameteri(GLuint texture, GLenum pname,
                         GLint param) const noexcept override {
    onTextureParameteri(texture, pname, param);
  }

  std::function<void(GLenum, GLenum, const GLfloat*)> onTexParameterfv =
      [](auto, auto, const auto*) {};
  void texParameterfv(GLenum target, GLenum pname,
                      const GLfloat* params) const noexcept override {
    onTexParameterfv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLint*)> onTexParameteriv =
      [](auto, auto, const auto*) {};
  void texParameteriv(GLenum target, GLenum pname,
                      const GLint* params) const noexcept override {
    onTexParameteriv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLint*)> onTexParameterIiv =
      [](auto, auto, const auto*) {};
  void texParameterIiv(GLenum target, GLenum pname,
                       const GLint* params) const noexcept override {
    onTexParameterIiv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLuint*)> onTexParameterIuiv =
      [](auto, auto, const auto*) {};
  void texParameterIuiv(GLenum target, GLenum pname,
                        const GLuint* params) const noexcept override {
    onTexParameterIuiv(target, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLfloat*)> onTextureParameterfv =
      [](auto, auto, const auto*) {};
  void textureParameterfv(GLuint texture, GLenum pname,
                          const GLfloat* params) const noexcept override {
    onTextureParameterfv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLint*)> onTextureParameteriv =
      [](auto, auto, const auto*) {};
  void textureParameteriv(GLuint texture, GLenum pname,
                          const GLint* params) const noexcept override {
    onTextureParameteriv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLint*)> onTextureParameterIiv =
      [](auto, auto, const auto*) {};
  void textureParameterIiv(GLuint texture, GLenum pname,
                           const GLint* params) const noexcept override {
    onTextureParameterIiv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLuint*)> onTextureParameterIuiv =
      [](auto, auto, const auto*) {};
  void textureParameterIuiv(GLuint texture, GLenum pname,
                            const GLuint* params) const noexcept override {
    onTextureParameterIuiv(texture, pname, params);
  }

  std::function<void(GLint, GLfloat)> onUniform1f = [](auto, auto) {};
  void uniform1f(GLint location, GLfloat v0) const noexcept override {
    onUniform1f(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform1fv =
      [](auto, auto, const auto*) {};
  void uniform1fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    onUniform1fv(location, count, value);
  }

  std::function<void(GLint, GLint)> onUniform1i = [](auto, auto) {};
  void uniform1i(GLint location, GLint v0) const noexcept override {
    onUniform1i(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform1iv =
      [](auto, auto, const auto*) {};
  void uniform1iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    onUniform1iv(location, count, value);
  }

  std::function<void(GLint, GLuint)> onUniform1ui = [](auto, auto) {};
  void uniform1ui(GLint location, GLuint v0) const noexcept override {
    onUniform1ui(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform1uiv =
      [](auto, auto, const auto*) {};
  void uniform1uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    onUniform1uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat)> onUniform2f = [](auto, auto,
                                                                auto) {};
  void uniform2f(GLint location, GLfloat v0,
                 GLfloat v1) const noexcept override {
    onUniform2f(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform2fv =
      [](auto, auto, const auto*) {};
  void uniform2fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    onUniform2fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint)> onUniform2i = [](auto, auto, auto) {
  };
  void uniform2i(GLint location, GLint v0, GLint v1) const noexcept override {
    onUniform2i(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform2iv =
      [](auto, auto, const auto*) {};
  void uniform2iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    onUniform2iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint)> onUniform2ui = [](auto, auto,
                                                               auto) {};
  void uniform2ui(GLint location, GLuint v0,
                  GLuint v1) const noexcept override {
    onUniform2ui(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform2uiv =
      [](auto, auto, const auto*) {};
  void uniform2uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    onUniform2uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat, GLfloat)> onUniform3f =
      [](auto, auto, auto, auto) {};
  void uniform3f(GLint location, GLfloat v0, GLfloat v1,
                 GLfloat v2) const noexcept override {
    onUniform3f(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform3fv =
      [](auto, auto, const auto*) {};
  void uniform3fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    onUniform3fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint, GLint)> onUniform3i =
      [](auto, auto, auto, auto) {};
  void uniform3i(GLint location, GLint v0, GLint v1,
                 GLint v2) const noexcept override {
    onUniform3i(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform3iv =
      [](auto, auto, const auto*) {};
  void uniform3iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    onUniform3iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint, GLuint)> onUniform3ui =
      [](auto, auto, auto, auto) {};
  void uniform3ui(GLint location, GLuint v0, GLuint v1,
                  GLuint v2) const noexcept override {
    onUniform3ui(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform3uiv =
      [](auto, auto, const auto*) {};
  void uniform3uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    onUniform3uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat, GLfloat, GLfloat)> onUniform4f =
      [](auto, auto, auto, auto, auto) {};
  void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
                 GLfloat v3) const noexcept override {
    onUniform4f(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform4fv =
      [](auto, auto, const auto*) {};
  void uniform4fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    onUniform4fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint, GLint, GLint)> onUniform4i =
      [](auto, auto, auto, auto, auto) {};
  void uniform4i(GLint location, GLint v0, GLint v1, GLint v2,
                 GLint v3) const noexcept override {
    onUniform4i(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform4iv =
      [](auto, auto, const auto*) {};
  void uniform4iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    onUniform4iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint, GLuint, GLuint)> onUniform4ui =
      [](auto, auto, auto, auto, auto) {};
  void uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
                  GLuint v3) const noexcept override {
    onUniform4ui(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform4uiv =
      [](auto, auto, const auto*) {};
  void uniform4uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    onUniform4uiv(location, count, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    onUniformMatrix2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2x3fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix2x3fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2x4fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix2x4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    onUniformMatrix3fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3x2fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix3x2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3x4fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix3x4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    onUniformMatrix4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4x2fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix4x2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4x3fv = [](auto, auto, auto, const auto*) {};
  void uniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    onUniformMatrix4x3fv(location, count, transpose, value);
  }

  std::function<void(GLuint)> onUseProgram = [](auto) {};
  void useProgram(GLuint program) const noexcept override {
    onUseProgram(program);
  }

  std::function<void(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)>
      onVertexAttribPointer = [](auto, auto, auto, auto, auto, const auto*) {};
  void vertexAttribPointer(GLuint index, GLint size, GLenum type,
                           GLboolean normalized, GLsizei stride,
                           const void* pointer) const noexcept override {
    onVertexAttribPointer(index, size, type, normalized, stride, pointer);
  }

  std::function<void(GLuint, GLint, GLenum, GLsizei, const void*)>
      onVertexAttribIPointer = [](auto, auto, auto, auto, const auto*) {};
  void vertexAttribIPointer(GLuint index, GLint size, GLenum type,
                            GLsizei stride,
                            const void* pointer) const noexcept override {
    onVertexAttribIPointer(index, size, type, stride, pointer);
  }

  std::function<void(GLuint, GLint, GLenum, GLsizei, const void*)>
      onVertexAttribLPointer = [](auto, auto, auto, auto, const auto*) {};
  void vertexAttribLPointer(GLuint index, GLint size, GLenum type,
                            GLsizei stride,
                            const void* pointer) const noexcept override {
    onVertexAttribLPointer(index, size, type, stride, pointer);
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
