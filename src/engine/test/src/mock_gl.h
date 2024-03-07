#ifndef SRC_ENGINE_TEST_SRC_MOCK_GL_H_
#define SRC_ENGINE_TEST_SRC_MOCK_GL_H_

#include <functional>

#include "uinta/gl.h"

namespace uinta {

class MockOpenGLApi : public OpenGLApi {
 public:
  std::function<void(GLuint, GLuint)> onAttachShader;
  void attachShader(GLuint program, GLuint shader) const noexcept override {
    if (onAttachShader) onAttachShader(program, shader);
  }

  std::function<void(GLenum, GLuint)> onBindBuffer;
  void bindBuffer(GLenum target, GLuint id) const noexcept override {
    if (onBindBuffer) onBindBuffer(target, id);
  }

  std::function<void(GLenum, GLuint)> onBindFramebuffer;
  void bindFramebuffer(GLenum target, GLuint id) const noexcept override {
    if (onBindFramebuffer) onBindFramebuffer(target, id);
  }

  std::function<void(GLenum, GLuint)> onBindRenderbuffer;
  void bindRenderbuffer(GLenum target, GLuint id) const noexcept override {
    if (onBindRenderbuffer) onBindRenderbuffer(target, id);
  }

  std::function<void(GLenum, GLuint)> onBindTexture;
  void bindTexture(GLenum target, GLuint id) const noexcept override {
    if (onBindTexture) onBindTexture(target, id);
  }

  std::function<void(GLuint)> onBindVertexArray;
  void bindVertexArray(GLuint id) const noexcept override {
    if (onBindVertexArray) onBindVertexArray(id);
  }

  std::function<void(GLfloat, GLfloat, GLfloat, GLfloat)> onBlendColor;
  void blendColor(GLfloat red, GLfloat green, GLfloat blue,
                  GLfloat alpha) const noexcept override {
    if (onBlendColor) onBlendColor(red, green, blue, alpha);
  }

  std::function<void(GLenum)> onBlendEquation;
  void blendEquation(GLenum mode) const noexcept override {
    if (onBlendEquation) onBlendEquation(mode);
  }

  std::function<void(GLuint, GLenum)> onBlendEquationi;
  void blendEquationi(GLuint buf, GLenum mode) const noexcept override {
    if (onBlendEquationi) onBlendEquationi(buf, mode);
  }

  std::function<void(GLenum, GLenum)> onBlendFunc;
  void blendFunc(GLenum sfactor, GLenum dfactor) const noexcept override {
    if (onBlendFunc) onBlendFunc(sfactor, dfactor);
  }

  std::function<void(GLenum, GLenum, GLenum, GLenum)> onBlendFuncSeparate;
  void blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
                         GLenum dstAlpha) const noexcept override {
    if (onBlendFuncSeparate)
      onBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
  }

  std::function<void(GLuint, GLenum, GLenum, GLenum, GLenum)>
      onBlendFuncSeparatei;
  void blendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB,
                          GLenum srcAlpha,
                          GLenum dstAlpha) const noexcept override {
    if (onBlendFuncSeparatei)
      onBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
  }

  std::function<void(GLuint, GLenum, GLenum)> onBlendFunci;
  void blendFunci(GLuint buf, GLenum sfactor,
                  GLenum dfactor) const noexcept override {
    if (onBlendFunci) onBlendFunci(buf, sfactor, dfactor);
  }

  std::function<void(GLenum, GLsizeiptr, const void*, GLenum)> onBufferData;
  void bufferData(GLenum target, GLsizeiptr size, const void* data,
                  GLenum usage) const noexcept override {
    if (onBufferData) onBufferData(target, size, data, usage);
  }

  std::function<void(GLenum, GLintptr, GLsizeiptr, const void*)>
      onBufferSubData;
  void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                     const void* data) const noexcept override {
    if (onBufferSubData) onBufferSubData(target, offset, size, data);
  }

  std::function<GLenum(GLenum)> onCheckFramebufferStatus;
  GLenum checkFramebufferStatus(GLenum target) const noexcept override {
    return onCheckFramebufferStatus ? onCheckFramebufferStatus(target) : GL_ONE;
  }

  std::function<GLenum(GLuint, GLenum)> onCheckNamedFramebufferStatus;
  GLenum checkNamedFramebufferStatus(GLuint framebuffer,
                                     GLenum target) const noexcept override {
    return onCheckNamedFramebufferStatus
               ? onCheckNamedFramebufferStatus(framebuffer, target)
               : GL_ONE;
  }

  std::function<void(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr)>
      onCopyBufferSubData;
  void copyBufferSubData(GLenum readTarget, GLenum writeTarget,
                         GLintptr readOffset, GLintptr writeOffset,
                         GLsizeiptr size) const noexcept override {
    if (onCopyBufferSubData)
      onCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset,
                          size);
  }

  std::function<void(GLuint, GLuint, GLintptr, GLintptr, GLsizeiptr)>
      onCopyNamedBufferSubData;
  void copyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer,
                              GLintptr readOffset, GLintptr writeOffset,
                              GLsizeiptr size) const noexcept override {
    if (onCopyNamedBufferSubData)
      onCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset,
                               size);
  }

  std::function<void(GLuint)> onCompileShader;
  void compileShader(GLuint shader) const noexcept override {
    if (onCompileShader) onCompileShader(shader);
  }

  std::function<GLuint()> onCreateProgram;
  GLuint createProgram() const noexcept override {
    return onCreateProgram ? onCreateProgram() : 1;
  }

  std::function<GLuint(GLenum)> onCreateShader;
  GLuint createShader(GLenum stage) const noexcept override {
    return onCreateShader ? onCreateShader(stage) : 1;
  }

  std::function<void(GLenum)> onCullFace;
  void cullFace(GLenum mode) const noexcept override {
    if (onCullFace) onCullFace(mode);
  }

  std::function<void(GLsizei, GLuint*)> onDeleteBuffers;
  void deleteBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onDeleteBuffers) onDeleteBuffers(count, ptr);
  }

  std::function<void(GLsizei, GLuint*)> onDeleteFramebuffers;
  void deleteFramebuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onDeleteFramebuffers) onDeleteFramebuffers(count, ptr);
  }

  std::function<void(GLuint)> onDeleteProgram;
  void deleteProgram(GLuint program) const noexcept override {
    if (onDeleteProgram) onDeleteProgram(program);
  }

  std::function<void(GLsizei, GLuint*)> onDeleteRenderbuffers;
  void deleteRenderbuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onDeleteRenderbuffers) onDeleteRenderbuffers(count, ptr);
  }

  std::function<void(GLuint)> onDeleteShader;
  void deleteShader(GLuint program) const noexcept override {
    if (onDeleteShader) onDeleteShader(program);
  }

  std::function<void(GLsizei, const GLuint*)> onDeleteTextures;
  void deleteTextures(GLsizei count,
                      const GLuint* ptr) const noexcept override {
    if (onDeleteTextures) onDeleteTextures(count, ptr);
  }

  std::function<void(GLenum)> onDisable;
  void disable(GLenum cap) const noexcept override {
    if (onDisable) onDisable(cap);
  }

  std::function<void(GLenum, GLuint)> onDisablei;
  void disablei(GLenum cap, GLuint index) const noexcept override {
    if (onDisablei) onDisablei(cap, index);
  }

  std::function<void(GLenum, GLint, GLsizei)> onDrawArrays;
  void drawArrays(GLenum mode, GLint first,
                  GLsizei count) const noexcept override {
    if (onDrawArrays) onDrawArrays(mode, first, count);
  }

  std::function<void(GLenum, GLint, GLsizei, GLsizei)> onDrawArraysInstanced;
  void drawArraysInstanced(GLenum mode, GLint first, GLsizei count,
                           GLsizei instancecount) const noexcept override {
    if (onDrawArraysInstanced)
      onDrawArraysInstanced(mode, first, count, instancecount);
  }

  std::function<void(GLenum, GLsizei, GLenum, const void*)> onDrawElements;
  void drawElements(GLenum mode, GLsizei count, GLenum type,
                    const void* indices) const noexcept override {
    if (onDrawElements) onDrawElements(mode, count, type, indices);
  }

  std::function<void(GLenum, GLuint, GLuint, GLsizei, GLenum, const void*)>
      onDrawRangeElements;
  void drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count,
                         GLenum type,
                         const void* indices) const noexcept override {
    if (onDrawRangeElements)
      onDrawRangeElements(mode, start, end, count, type, indices);
  }

  std::function<void(GLenum)> onEnable;
  void enable(GLenum cap) const noexcept override {
    if (onEnable) onEnable(cap);
  }

  std::function<void(GLuint, GLuint)> onEnableVertexArrayAttrib;
  void enableVertexArrayAttrib(GLuint vaobj,
                               GLuint index) const noexcept override {
    if (onEnableVertexArrayAttrib) onEnableVertexArrayAttrib(vaobj, index);
  }

  std::function<void(GLenum, GLuint)> onEnablei;
  void enablei(GLenum cap, GLuint index) const noexcept override {
    if (onEnablei) onEnablei(cap, index);
  }

  std::function<void(GLsizei, GLuint*)> onDeleteVertexArrays;
  void deleteVertexArrays(GLsizei count, GLuint* ptr) const noexcept override {
    if (onDeleteVertexArrays) onDeleteVertexArrays(count, ptr);
  }

  std::function<void(GLuint)> onEnableVertexAttribArray;
  void enableVertexAttribArray(GLuint index) const noexcept override {
    if (onEnableVertexAttribArray) onEnableVertexAttribArray(index);
  }

  std::function<void(GLenum, GLenum, GLuint, GLint)> onFramebufferTexture;
  void framebufferTexture(GLenum target, GLenum attachment, GLuint texture,
                          GLint level) const noexcept override {
    if (onFramebufferTexture)
      onFramebufferTexture(target, attachment, texture, level);
  }

  std::function<void(GLenum, GLenum, GLenum, GLuint, GLint)>
      onFramebufferTexture1D;
  void framebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget,
                            GLuint texture,
                            GLint level) const noexcept override {
    if (onFramebufferTexture1D)
      onFramebufferTexture1D(target, attachment, textarget, texture, level);
  }

  std::function<void(GLenum, GLenum, GLenum, GLuint, GLint)>
      onFramebufferTexture2D;
  void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
                            GLuint texture,
                            GLint level) const noexcept override {
    if (onFramebufferTexture2D)
      onFramebufferTexture2D(target, attachment, textarget, texture, level);
  }

  std::function<void(GLenum, GLenum, GLenum, GLuint, GLint, GLint)>
      onFramebufferTexture3D;
  void framebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget,
                            GLuint texture, GLint level,
                            GLint layer) const noexcept override {
    if (onFramebufferTexture3D)
      onFramebufferTexture3D(target, attachment, textarget, texture, level,
                             layer);
  }

  std::function<void(GLenum, GLenum, GLenum, GLuint)> onFramebufferRenderbuffer;
  void framebufferRenderbuffer(GLenum target, GLenum attachment,
                               GLenum renderbuffertarget,
                               GLuint renderbuffer) const noexcept override {
    if (onFramebufferRenderbuffer)
      onFramebufferRenderbuffer(target, attachment, renderbuffertarget,
                                renderbuffer);
  }

  std::function<void(GLsizei, GLuint*)> onGenBuffers;
  void genBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onGenBuffers) onGenBuffers(count, ptr);
  }

  std::function<void(GLenum)> onGenerateMipmap;
  void generateMipmap(GLenum target) const noexcept override {
    if (onGenerateMipmap) onGenerateMipmap(target);
  }

  std::function<void(GLsizei, GLuint*)> onGenFramebuffers;
  void genFramebuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onGenFramebuffers) onGenFramebuffers(count, ptr);
  }

  std::function<void(GLsizei, GLuint*)> onGenRenderbuffers;
  void genRenderbuffers(GLsizei count, GLuint* ptr) const noexcept override {
    if (onGenRenderbuffers) onGenRenderbuffers(count, ptr);
  }

  std::function<void(GLsizei, GLuint*)> onGenTextures;
  void genTextures(GLsizei count, GLuint* ptr) const noexcept override {
    if (onGenTextures) onGenTextures(count, ptr);
  }

  std::function<void(GLuint, GLenum, GLint*)> onGetShaderiv;
  void getShaderiv(GLuint shader, GLenum pname,
                   GLint* params) const noexcept override {
    if (onGetShaderiv) onGetShaderiv(shader, pname, params);
  }

  std::function<void(GLuint, GLenum, GLint*)> onGetProgramiv;
  void getProgramiv(GLuint program, GLenum pname,
                    GLint* params) const noexcept override {
    if (onGetProgramiv) onGetProgramiv(program, pname, params);
  }

  std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> onGetProgramInfoLog;
  void getProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length,
                         GLchar* infoLog) const noexcept override {
    if (onGetProgramInfoLog)
      onGetProgramInfoLog(program, maxLength, length, infoLog);
  }

  std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> onGetShaderInfoLog;
  void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length,
                        GLchar* infoLog) const noexcept override {
    if (onGetShaderInfoLog)
      onGetShaderInfoLog(shader, maxLength, length, infoLog);
  }

  std::function<GLint(GLuint, const GLchar* name)> onGetUniformLocation;
  GLint getUniformLocation(GLuint program,
                           const GLchar* name) const noexcept override {
    return onGetUniformLocation ? onGetUniformLocation(program, name) : 1;
  }

  std::function<void(GLenum, GLboolean*)> onGetBooleanv;
  void getBooleanv(GLenum pname, GLboolean* data) const noexcept override {
    if (onGetBooleanv) onGetBooleanv(pname, data);
  }

  std::function<void(GLenum, GLdouble*)> onGetDoublev;
  void getDoublev(GLenum pname, GLdouble* data) const noexcept override {
    if (onGetDoublev) onGetDoublev(pname, data);
  }

  std::function<void(GLenum, GLfloat*)> onGetFloatv;
  void getFloatv(GLenum pname, GLfloat* data) const noexcept override {
    if (onGetFloatv) onGetFloatv(pname, data);
  }

  std::function<void(GLenum, GLuint, GLint64*)> onGetInteger64i_v;
  void getInteger64i_v(GLenum target, GLuint index,
                       GLint64* data) const noexcept override {
    if (onGetInteger64i_v) onGetInteger64i_v(target, index, data);
  }

  std::function<void(GLenum, GLint64*)> onGetInteger64v;
  void getInteger64v(GLenum pname, GLint64* data) const noexcept override {
    if (onGetInteger64v) onGetInteger64v(pname, data);
  }

  std::function<void(GLenum, GLuint, GLint*)> onGetIntegeri_v;
  void getIntegeri_v(GLenum target, GLuint index,
                     GLint* data) const noexcept override {
    if (onGetIntegeri_v) onGetIntegeri_v(target, index, data);
  }

  std::function<void(GLenum, GLint*)> onGetIntegerv;
  void getIntegerv(GLenum pname, GLint* data) const noexcept override {
    if (onGetIntegerv) onGetIntegerv(pname, data);
  }

  std::function<bool(GLenum)> onIsEnabled;
  bool isEnabled(GLenum cap) const noexcept override {
    return onIsEnabled ? onIsEnabled(cap) : true;
  }

  std::function<bool(GLenum, GLuint)> onIsEnabledi;
  bool isEnabledi(GLenum cap, GLuint index) const noexcept override {
    if (onIsEnabledi) return onIsEnabledi(cap, index);
    return false;
  }

  std::function<void(GLsizei, GLuint*)> onGenVertexArrays;
  void genVertexArrays(GLsizei count, GLuint* ptr) const noexcept override {
    if (onGenVertexArrays) onGenVertexArrays(count, ptr);
  }

  std::function<void(GLuint)> onLinkProgram;
  void linkProgram(GLuint program) const noexcept override {
    if (onLinkProgram) onLinkProgram(program);
  }

  std::function<void(GLuint, GLintptr, GLsizeiptr, const void*)>
      onNamedBufferSubData;
  void namedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size,
                          const void* data) const noexcept override {
    if (onNamedBufferSubData) onNamedBufferSubData(buffer, offset, size, data);
  }

  std::function<void(GLuint, GLenum, GLuint, GLint)> onNamedFramebufferTexture;
  void namedFramebufferTexture(GLuint framebuffer, GLenum attachment,
                               GLuint texture,
                               GLint level) const noexcept override {
    if (onNamedFramebufferTexture)
      onNamedFramebufferTexture(framebuffer, attachment, texture, level);
  }

  std::function<void(GLuint, GLenum, GLenum, GLuint)>
      onNamedFramebufferRenderbuffer;
  void namedFramebufferRenderbuffer(
      GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget,
      GLuint renderbuffer) const noexcept override {
    if (onNamedFramebufferRenderbuffer)
      onNamedFramebufferRenderbuffer(framebuffer, attachment,
                                     renderbuffertarget, renderbuffer);
  }

  std::function<void(GLuint, GLenum, GLsizei, GLsizei)>
      onNamedRenderbufferStorage;
  void namedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat,
                                GLsizei width,
                                GLsizei height) const noexcept override {
    if (onNamedRenderbufferStorage)
      onNamedRenderbufferStorage(renderbuffer, internalformat, width, height);
  }

  std::function<void(GLenum, GLenum)> onPolygonMode;
  void polygonMode(GLenum face, GLenum mode) const noexcept override {
    if (onPolygonMode) onPolygonMode(face, mode);
  }

  std::function<void(GLenum, GLenum, GLsizei, GLsizei)> onRenderbufferStorage;
  void renderbufferStorage(GLenum target, GLenum internalformat, GLsizei width,
                           GLsizei height) const noexcept override {
    if (onRenderbufferStorage)
      onRenderbufferStorage(target, internalformat, width, height);
  }

  std::function<void(GLuint, GLsizei, const GLchar**, const GLint*)>
      onShaderSource;
  void shaderSource(GLuint shader, GLsizei count, const GLchar** source,
                    const GLint* length) const noexcept override {
    if (onShaderSource) onShaderSource(shader, count, source, length);
  }

  std::function<void(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
                     GLenum, const void*)>
      onTexImage2D;
  void texImage2D(GLenum target, GLint level, GLint internalformat,
                  GLsizei width, GLsizei height, GLint border, GLenum format,
                  GLenum type, const void* data) const noexcept override {
    if (onTexImage2D)
      onTexImage2D(target, level, internalformat, width, height, border, format,
                   type, data);
  }

  std::function<void(GLenum, GLenum, GLfloat)> onTexParameterf;
  void texParameterf(GLenum target, GLenum pname,
                     GLfloat param) const noexcept override {
    if (onTexParameterf) onTexParameterf(target, pname, param);
  }

  std::function<void(GLenum, GLenum, GLint)> onTexParameteri;
  void texParameteri(GLenum target, GLenum pname,
                     GLint param) const noexcept override {
    if (onTexParameteri) onTexParameteri(target, pname, param);
  }

  std::function<void(GLuint, GLenum, GLfloat)> onTextureParameterf;
  void textureParameterf(GLuint texture, GLenum pname,
                         GLfloat param) const noexcept override {
    if (onTextureParameterf) onTextureParameterf(texture, pname, param);
  }

  std::function<void(GLuint, GLenum, GLint)> onTextureParameteri;
  void textureParameteri(GLuint texture, GLenum pname,
                         GLint param) const noexcept override {
    if (onTextureParameteri) onTextureParameteri(texture, pname, param);
  }

  std::function<void(GLenum, GLenum, const GLfloat*)> onTexParameterfv;
  void texParameterfv(GLenum target, GLenum pname,
                      const GLfloat* params) const noexcept override {
    if (onTexParameterfv) onTexParameterfv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLint*)> onTexParameteriv;
  void texParameteriv(GLenum target, GLenum pname,
                      const GLint* params) const noexcept override {
    if (onTexParameteriv) onTexParameteriv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLint*)> onTexParameterIiv;
  void texParameterIiv(GLenum target, GLenum pname,
                       const GLint* params) const noexcept override {
    if (onTexParameterIiv) onTexParameterIiv(target, pname, params);
  }

  std::function<void(GLenum, GLenum, const GLuint*)> onTexParameterIuiv;
  void texParameterIuiv(GLenum target, GLenum pname,
                        const GLuint* params) const noexcept override {
    if (onTexParameterIuiv) onTexParameterIuiv(target, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLfloat*)> onTextureParameterfv;
  void textureParameterfv(GLuint texture, GLenum pname,
                          const GLfloat* params) const noexcept override {
    if (onTextureParameterfv) onTextureParameterfv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLint*)> onTextureParameteriv;
  void textureParameteriv(GLuint texture, GLenum pname,
                          const GLint* params) const noexcept override {
    if (onTextureParameteriv) onTextureParameteriv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLint*)> onTextureParameterIiv;
  void textureParameterIiv(GLuint texture, GLenum pname,
                           const GLint* params) const noexcept override {
    if (onTextureParameterIiv) onTextureParameterIiv(texture, pname, params);
  }

  std::function<void(GLuint, GLenum, const GLuint*)> onTextureParameterIuiv;
  void textureParameterIuiv(GLuint texture, GLenum pname,
                            const GLuint* params) const noexcept override {
    if (onTextureParameterIuiv) onTextureParameterIuiv(texture, pname, params);
  }

  std::function<void(GLint, GLfloat)> onUniform1f;
  void uniform1f(GLint location, GLfloat v0) const noexcept override {
    if (onUniform1f) onUniform1f(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform1fv;
  void uniform1fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    if (onUniform1fv) onUniform1fv(location, count, value);
  }

  std::function<void(GLint, GLint)> onUniform1i;
  void uniform1i(GLint location, GLint v0) const noexcept override {
    if (onUniform1i) onUniform1i(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform1iv;
  void uniform1iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    if (onUniform1iv) onUniform1iv(location, count, value);
  }

  std::function<void(GLint, GLuint)> onUniform1ui;
  void uniform1ui(GLint location, GLuint v0) const noexcept override {
    if (onUniform1ui) onUniform1ui(location, v0);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform1uiv;
  void uniform1uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    if (onUniform1uiv) onUniform1uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat)> onUniform2f;
  void uniform2f(GLint location, GLfloat v0,
                 GLfloat v1) const noexcept override {
    if (onUniform2f) onUniform2f(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform2fv;
  void uniform2fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    if (onUniform2fv) onUniform2fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint)> onUniform2i;
  void uniform2i(GLint location, GLint v0, GLint v1) const noexcept override {
    if (onUniform2i) onUniform2i(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform2iv;
  void uniform2iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    if (onUniform2iv) onUniform2iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint)> onUniform2ui;
  void uniform2ui(GLint location, GLuint v0,
                  GLuint v1) const noexcept override {
    if (onUniform2ui) onUniform2ui(location, v0, v1);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform2uiv;
  void uniform2uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    if (onUniform2uiv) onUniform2uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat, GLfloat)> onUniform3f;
  void uniform3f(GLint location, GLfloat v0, GLfloat v1,
                 GLfloat v2) const noexcept override {
    if (onUniform3f) onUniform3f(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform3fv;
  void uniform3fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    if (onUniform3fv) onUniform3fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint, GLint)> onUniform3i;
  void uniform3i(GLint location, GLint v0, GLint v1,
                 GLint v2) const noexcept override {
    if (onUniform3i) onUniform3i(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform3iv;
  void uniform3iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    if (onUniform3iv) onUniform3iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint, GLuint)> onUniform3ui;
  void uniform3ui(GLint location, GLuint v0, GLuint v1,
                  GLuint v2) const noexcept override {
    if (onUniform3ui) onUniform3ui(location, v0, v1, v2);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform3uiv;
  void uniform3uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    if (onUniform3uiv) onUniform3uiv(location, count, value);
  }

  std::function<void(GLint, GLfloat, GLfloat, GLfloat, GLfloat)> onUniform4f;
  void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
                 GLfloat v3) const noexcept override {
    if (onUniform4f) onUniform4f(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLfloat*)> onUniform4fv;
  void uniform4fv(GLint location, GLsizei count,
                  const GLfloat* value) const noexcept override {
    if (onUniform4fv) onUniform4fv(location, count, value);
  }

  std::function<void(GLint, GLint, GLint, GLint, GLint)> onUniform4i;
  void uniform4i(GLint location, GLint v0, GLint v1, GLint v2,
                 GLint v3) const noexcept override {
    if (onUniform4i) onUniform4i(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLint*)> onUniform4iv;
  void uniform4iv(GLint location, GLsizei count,
                  const GLint* value) const noexcept override {
    if (onUniform4iv) onUniform4iv(location, count, value);
  }

  std::function<void(GLint, GLuint, GLuint, GLuint, GLuint)> onUniform4ui;
  void uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
                  GLuint v3) const noexcept override {
    if (onUniform4ui) onUniform4ui(location, v0, v1, v2, v3);
  }

  std::function<void(GLint, GLsizei, const GLuint*)> onUniform4uiv;
  void uniform4uiv(GLint location, GLsizei count,
                   const GLuint* value) const noexcept override {
    if (onUniform4uiv) onUniform4uiv(location, count, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2fv;
  void uniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    if (onUniformMatrix2fv)
      onUniformMatrix2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2x3fv;
  void uniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix2x3fv)
      onUniformMatrix2x3fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix2x4fv;
  void uniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix2x4fv)
      onUniformMatrix2x4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3fv;
  void uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    if (onUniformMatrix3fv)
      onUniformMatrix3fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3x2fv;
  void uniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix3x2fv)
      onUniformMatrix3x2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix3x4fv;
  void uniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix3x4fv)
      onUniformMatrix3x4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4fv;
  void uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
                        const GLfloat* value) const noexcept override {
    if (onUniformMatrix4fv)
      onUniformMatrix4fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4x2fv;
  void uniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix4x2fv)
      onUniformMatrix4x2fv(location, count, transpose, value);
  }

  std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>
      onUniformMatrix4x3fv;
  void uniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,
                          const GLfloat* value) const noexcept override {
    if (onUniformMatrix4x3fv)
      onUniformMatrix4x3fv(location, count, transpose, value);
  }

  std::function<void(GLuint)> onUseProgram;
  void useProgram(GLuint program) const noexcept override {
    if (onUseProgram) onUseProgram(program);
  }

  std::function<void(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)>
      onVertexAttribPointer;
  void vertexAttribPointer(GLuint index, GLint size, GLenum type,
                           GLboolean normalized, GLsizei stride,
                           const void* pointer) const noexcept override {
    if (onVertexAttribPointer)
      onVertexAttribPointer(index, size, type, normalized, stride, pointer);
  }

  std::function<void(GLuint, GLint, GLenum, GLsizei, const void*)>
      onVertexAttribIPointer;
  void vertexAttribIPointer(GLuint index, GLint size, GLenum type,
                            GLsizei stride,
                            const void* pointer) const noexcept override {
    if (onVertexAttribIPointer)
      onVertexAttribIPointer(index, size, type, stride, pointer);
  }

  std::function<void(GLuint, GLint, GLenum, GLsizei, const void*)>
      onVertexAttribLPointer;
  void vertexAttribLPointer(GLuint index, GLint size, GLenum type,
                            GLsizei stride,
                            const void* pointer) const noexcept override {
    if (onVertexAttribLPointer)
      onVertexAttribLPointer(index, size, type, stride, pointer);
  }

  std::function<void(GLuint, GLuint, GLuint, GLuint)> onViewport;
  void viewport(GLint x, GLint y, GLsizei width,
                GLsizei height) const noexcept override {
    if (onViewport) onViewport(x, y, width, height);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_MOCK_GL_H_
