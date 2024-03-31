#ifndef SRC_PLATFORM_INCLUDE_UINTA_GL_H_
#define SRC_PLATFORM_INCLUDE_UINTA_GL_H_

#include <string>

#ifdef __ANDROID__
#include <GL/gl.h>
#include <GL/glu.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else  // desktop
#include <glad/glad.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#endif

namespace uinta {

struct OpenGLApi {
  static std::string GetTypeString(GLenum type) noexcept {
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
        return "Unknown type: " + std::to_string(type);
    }
  }

  static std::string GetSourceString(GLenum source) noexcept {
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
        return "Unknown source: " + std::to_string(source);
    }
  }

  static std::string GetSeverityString(GLenum severity) noexcept {
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
        return "Unknown severity: " + std::to_string(severity);
    }
  }

  /*! `glClear` — clear buffers to preset values
   *
   *  @brief `glClear `sets the bitplane area of the window to values previously
   * selected by glClearColor, glClearDepth, and glClearStencil. Multiple color
   * buffers can be cleared simultaneously by selecting more than one buffer at
   * a time using glDrawBuffer.
   *
   *  @param `mask` Bitwise OR of masks that indicate the buffers to be cleared.
   * The three masks are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, and
   * GL_STENCIL_BUFFER_BIT.
   */
  virtual void clear(GLbitfield) const noexcept = 0;

  /*! `glClearColor` — specify clear values for the color buffers
   *
   *  @brief `glClearColor` specifies the red, green, blue, and alpha values
   * used by glClear to clear the color buffers. Values specified by
   * glClearColor are clamped to the range [0,1].
   *
   *  @param `r` Specify the red value used when the color buffers are cleared.
   * The initial values are all 0.
   *  @param `g` Specify the green value used when the color buffers are
   * cleared. The initial values are all 0.
   *  @param `b` Specify the blue value used when the color buffers are cleared.
   * The initial values are all 0.
   *  @param `a` Specify the alpha value used when the color buffers are
   * cleared. The initial values are all 0.
   */
  virtual void clearColor(GLfloat r, GLfloat g, GLfloat b,
                          GLfloat a) const noexcept = 0;

  /*! `glDisable` — disable server-side GL capabilities
   *
   *  @brief `glDisable` disable various capabilities.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   */
  virtual void disable(GLenum cap) const noexcept = 0;

  /*! `glDisablei` — disable server-side GL capabilities
   *
   *  @brief `glDisable` disable various capabilities.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   *  @param `index` Specifies the index of the switch to disable (for
   * `glEnablei` and `glDisablei` only).
   */
  virtual void disablei(GLenum cap, GLuint index) const noexcept = 0;

  /*! `glDrawArrays` — render primitives from array data
   *
   *  @brief `glDrawArrays` specifies multiple geometric primitives with very
   * few subroutine calls.
   *
   *  @param `mode` Specifies what kind of primitives to render. Symbolic
   * constants `GL_POINTS`, `GL_LINE_STRIP`, `GL_LINE_LOOP`, `GL_LINES`,
   * `GL_LINE_STRIP_ADJACENCY`, `GL_LINES_ADJACENCY`, `GL_TRIANGLE_STRIP`,
   * `GL_TRIANGLE_FAN`, `GL_TRIANGLES`, `GL_TRIANGLE_STRIP_ADJACENCY`,
   * `GL_TRIANGLES_ADJACENCY` and `GL_PATCHES` are accepted.
   *  @param `first` Specifies the starting index in the enabled arrays.
   *  @param `count` Specifies the number of indices to be rendered.
   */
  virtual void drawArrays(GLenum mode, GLint first,
                          GLsizei count) const noexcept = 0;

  /*! `glDrawArraysInstanced` — draw multiple instances of a range of elements
   *
   *  @brief `glDrawArraysInstanced` behaves identically to `glDrawArrays`
   * except that instancecount instances of the range of elements are executed
   * and the value of the internal counter instanceID advances for each
   * iteration.
   *
   *  @param `mode` Specifies what kind of primitives to render. Symbolic
   * constants `GL_POINTS`, `GL_LINE_STRIP`, `GL_LINE_LOOP`, `GL_LINES`,
   * `GL_TRIANGLE_STRIP`, `GL_TRIANGLE_FAN`, `GL_TRIANGLES`
   * `GL_LINES_ADJACENCY`, `GL_LINE_STRIP_ADJACENCY`, `GL_TRIANGLES_ADJACENCY`,
   * `GL_TRIANGLE_STRIP_ADJACENCY` and `GL_PATCHES` are accepted.
   *  @param `first` Specifies the starting index in the enabled arrays.
   *  @param `count` Specifies the number of indices to be rendered.
   *  @param `instancecount` Specifies the number of instances of the specified
   * range of indices to be rendered.
   */
  virtual void drawArraysInstanced(GLenum mode, GLint first, GLsizei count,
                                   GLsizei instancecount) const noexcept = 0;

  /*! `glDrawElements` — render primitives from array data
   *
   *  @brief `glDrawElements` specifies multiple geometric primitives with very
   * few subroutine calls.
   *
   *  @param `mode` Specifies what kind of primitives to render. Symbolic
   * constants `GL_POINTS`, `GL_LINE_STRIP`, `GL_LINE_LOOP`, `GL_LINES`,
   * `GL_LINE_STRIP_ADJACENCY`, `GL_LINES_ADJACENCY`, `GL_TRIANGLE_STRIP`,
   * `GL_TRIANGLE_FAN`, `GL_TRIANGLES`, `GL_TRIANGLE_STRIP_ADJACENCY`,
   * `GL_TRIANGLES_ADJACENCY` and `GL_PATCHES` are accepted.
   *  @param `count` Specifies the number of elements to be rendered.
   *  @param `type` Specifies the type of the values in indices. Must be one of
   * `GL_UNSIGNED_BYTE`, `GL_UNSIGNED_SHORT`, or `GL_UNSIGNED_INT`.
   *  @param `indices` Specifies a pointer to the location where the indices are
   * stored.
   */
  virtual void drawElements(GLenum mode, GLsizei count, GLenum type,
                            const void* indices) const noexcept = 0;

  /*! `glDrawRangeElements` — render primitives from array data
   *
   *  @brief `glDrawRangeElements` is a restricted form of `glDrawElements`.
   *
   *  @param `mode` Specifies what kind of primitives to render. Symbolic
   * constants `GL_POINTS`, `GL_LINE_STRIP`, `GL_LINE_LOOP`, `GL_LINES`,
   * `GL_LINE_STRIP_ADJACENCY`, `GL_LINES_ADJACENCY`, `GL_TRIANGLE_STRIP`,
   * `GL_TRIANGLE_FAN`, `GL_TRIANGLES`, `GL_TRIANGLE_STRIP_ADJACENCY`,
   * `GL_TRIANGLES_ADJACENCY` and `GL_PATCHES` are accepted.
   *  @param `start` Specifies the minimum array index contained in indices.
   *  @param `end` Specifies the maximum array index contained in indices.
   *  @param `count` Specifies the number of elements to be rendered.
   *  @param `type` Specifies the type of the values in indices. Must be one of
   * `GL_UNSIGNED_BYTE`, `GL_UNSIGNED_SHORT`, or `GL_UNSIGNED_INT`.
   *  @param `indices` Specifies a pointer to the location where the indices are
   * stored.
   */
  virtual void drawRangeElements(GLenum mode, GLuint start, GLuint end,
                                 GLsizei count, GLenum type,
                                 const void* indices) const noexcept = 0;

  /*! `glEnable` — enable server-side GL capabilities
   *
   *  @brief `glEnable` enable and disable various capabilities.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   */
  virtual void enable(GLenum cap) const noexcept = 0;

  /*! `glEnablei` — enable or disable server-side GL capabilities
   *
   *  @brief `glDisable` enable and disable various capabilities.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   *  @param `index` Specifies the index of the switch to disable (for
   * `glEnablei` and `glDisablei` only).
   */
  virtual void enablei(GLenum cap, GLuint index) const noexcept = 0;

  /*! `gluErrorString` — produce an error string from a GL or GLU error code
   *
   *  @brief `gluErrorString` produces an error string from a GL or GLU error
   * code.
   *
   *  @param `error` Specifies a GL or GLU error code
   *
   *  @returns The string out of memory.
   */
  virtual const GLubyte* errorString(GLenum error) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`. The symbolic constants in the list below
   * are accepted.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getBooleanv(GLenum pname, GLboolean* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`. The symbolic constants in the list below
   * are accepted.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getDoublev(GLenum pname, GLdouble* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`. The symbolic constants in the list below
   * are accepted.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getFloatv(GLenum pname, GLfloat* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`.
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `target` Specifies the parameter value to be returned for indexed
   * versions of glGet. The symbolic constants in the list below are accepted.
   *  @param `index` Specifies the index of the particular element being
   * queried.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getInteger64i_v(GLenum target, GLuint index,
                               GLint64* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`. The symbolic constants in the list below
   * are accepted.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getInteger64v(GLenum pname, GLint64* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `target` Specifies the parameter value to be returned for indexed
   * versions of glGet. The symbolic constants in the list below are accepted.
   *  @param `index` Specifies the index of the particular element being
   * queried.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getIntegeri_v(GLenum target, GLuint index,
                             GLint* data) const noexcept = 0;

  /*! `glGet` — return the value or values of a selected parameter
   *
   *  @brief These commands return values for simple state variables in GL.
   *
   *  @param `pname` Specifies the parameter value to be returned for
   * non-indexed versions of `glGet`. The symbolic constants in the list below
   * are accepted.
   *  @param `data` Returns the value or values of the specified parameter.
   */
  virtual void getIntegerv(GLenum pname, GLint* data) const noexcept = 0;

  /*! `glIsEnabled`, `glIsEnabledi` — test whether a capability is enabled
   *
   *  @brief `glIsEnabled` returns `GL_TRUE` if cap is an enabled capability and
   * returns `GL_FALSE` otherwise.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   *
   *  @returns `GL_TRUE` if cap is an enabled capability and returns `GL_FALSE`
   * otherwise.
   */
  virtual bool isEnabled(GLenum cap) const noexcept = 0;

  /*! `glIsEnabled`, `glIsEnabledi` — test whether a capability is enabled
   *
   *  @brief `glIsEnabled` returns `GL_TRUE` if cap is an enabled capability and
   * returns `GL_FALSE` otherwise.
   *
   *  @param `cap` Specifies a symbolic constant indicating a GL capability.
   *  @param `index` Specifies the index of the capability.
   *
   *  @returns `GL_TRUE` if cap is an enabled capability and returns `GL_FALSE`
   * otherwise.
   */
  virtual bool isEnabledi(GLenum cap, GLuint index) const noexcept = 0;

  /*! `glLineWidth` — Specify the width of rasterized lines
   *
   *  @brief `glLineWidth` specifies the rasterized width of both aliased and
   * antialiased lines.
   *
   *  @param `width` Specifies the width of rasterized lines. The initial value
   * is 1.
   */
  virtual void lineWidth(GLfloat width) const noexcept = 0;

  /*! `glPointSize` — specify the diameter of rasterized points
   *
   *  @brief `glPointSize` specifies the rasterized diameter of points.
   *
   *  @param `size` Specifies the diameter of rasterized points. The initial
   * value is 1.
   */
  virtual void pointSize(GLfloat size) const noexcept = 0;

  /*! glViewport - set the viewport
   *
   *  @brief glViewport specifies the affine transformation of x and y from
   * normalized device coordinates to window coordinates.
   *
   *  @param x Specify the left side of the viewport rectangle, in pixels.
   *  @param y Specify the bottom side of the viewport rectangle, in pixels.
   *  @param width Specify the width of the viewport.
   *  @param height Specify the height of the viewport.
   *
   *  @returns
   */
  virtual void viewport(GLint x, GLint y, GLsizei width,
                        GLsizei height) const noexcept = 0;
};

struct OpenGLApiImpl : OpenGLApi {
  static const OpenGLApiImpl* Instance() noexcept {
    static OpenGLApiImpl instance;
    return &instance;
  }

  OpenGLApiImpl(const OpenGLApiImpl&) noexcept = delete;
  OpenGLApiImpl& operator=(const OpenGLApiImpl&) noexcept = delete;
  OpenGLApiImpl(OpenGLApiImpl&&) noexcept = delete;
  OpenGLApiImpl& operator=(OpenGLApiImpl&&) noexcept = delete;

  inline void clear(GLbitfield mask) const noexcept override { glClear(mask); }

  inline void clearColor(GLfloat r, GLfloat g, GLfloat b,
                         GLfloat a) const noexcept override {
    glClearColor(r, g, b, a);
  }

  inline void disable(GLenum cap) const noexcept override { glDisable(cap); }

  inline void disablei(GLenum cap, GLuint index) const noexcept override {
    glDisablei(cap, index);
  }

  inline void drawArrays(GLenum mode, GLint first,
                         GLsizei count) const noexcept override {
    glDrawArrays(mode, first, count);
  }

  inline void drawArraysInstanced(
      GLenum mode, GLint first, GLsizei count,
      GLsizei instancecount) const noexcept override {
    glDrawArraysInstanced(mode, first, count, instancecount);
  }

  inline void drawElements(GLenum mode, GLsizei count, GLenum type,
                           const void* indices) const noexcept override {
    glDrawElements(mode, count, type, indices);
  }

  inline void drawRangeElements(GLenum mode, GLuint start, GLuint end,
                                GLsizei count, GLenum type,
                                const void* indices) const noexcept override {
    glDrawRangeElements(mode, start, end, count, type, indices);
  }

  inline void enable(GLenum cap) const noexcept override { glEnable(cap); }

  inline void enablei(GLenum cap, GLuint index) const noexcept override {
    glEnablei(cap, index);
  }

  inline const GLubyte* errorString(GLenum error) const noexcept override {
    auto str = gluErrorString(error);
    return str;
  }

  inline void getBooleanv(GLenum pname,
                          GLboolean* data) const noexcept override {
    glGetBooleanv(pname, data);
  }

  inline void getDoublev(GLenum pname, GLdouble* data) const noexcept override {
    glGetDoublev(pname, data);
  }

  inline void getFloatv(GLenum pname, GLfloat* data) const noexcept override {
    glGetFloatv(pname, data);
  }

  inline void getInteger64i_v(GLenum target, GLuint index,
                              GLint64* data) const noexcept override {
    glGetInteger64i_v(target, index, data);
  }

  inline void getInteger64v(GLenum pname,
                            GLint64* data) const noexcept override {
    glGetInteger64v(pname, data);
  }

  inline void getIntegeri_v(GLenum target, GLuint index,
                            GLint* data) const noexcept override {
    glGetIntegeri_v(target, index, data);
  }

  inline void getIntegerv(GLenum pname, GLint* data) const noexcept override {
    glGetIntegerv(pname, data);
  }

  inline bool isEnabledi(GLenum cap, GLuint index) const noexcept override {
    return glIsEnabledi(cap, index);
  }

  inline bool isEnabled(GLenum cap) const noexcept override {
    return glIsEnabled(cap);
  }

  inline void lineWidth(GLfloat width) const noexcept override {
    glLineWidth(width);
  }

  inline void pointSize(GLfloat size) const noexcept override {
    glPointSize(size);
  }

  inline void viewport(GLint x, GLint y, GLsizei width,
                       GLsizei height) const noexcept override {
    glViewport(x, y, width, height);
  }

 private:
  OpenGLApiImpl() noexcept = default;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_GL_H_
