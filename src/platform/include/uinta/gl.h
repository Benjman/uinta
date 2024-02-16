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

class OpenGLApi {
 public:
  std::string getTypeString(GLenum) const noexcept;
  std::string getSourceString(GLenum) const noexcept;
  std::string getSeverityString(GLenum) const noexcept;

  /*! `glAttachShader` — Attaches a shader object to a program object
   *
   *  @brief In order to create a complete shader program, there must be a way
   * to specify the list of things that will be linked together.
   *
   *  @param `program` Specifies the program object to which a shader object
   * will be attached.
   *  @param `shader` Specifies the shader object that is to be attached.
   */
  virtual void attachShader(GLuint program, GLuint shader) const noexcept = 0;

  /*! `glBindBuffer` — bind a named buffer object
   *
   *  @brief `glBindBuffer` binds a buffer object to the specified buffer
   * binding point.
   *
   *  @param `target` Specifies the target to which the buffer object is bound.
   *  @param `buffer` Specifies the name of a buffer object.
   */
  virtual void bindBuffer(GLenum target, GLuint buffer) const noexcept = 0;

  /*! `glBufferData`, `glNamedBufferData` — creates and initializes a buffer
   * object's data store
   *
   *  @brief `glBufferData` and `glNamedBufferData` create a new data store for
   * a buffer object.
   *
   *  @param `target` Specifies the target to which the buffer object is bound
   * for `glBufferData`.
   *  @param `size` Specifies the size in bytes of the buffer object's new data
   * store.
   *  @param `data` Specifies a pointer to data that will be copied into the
   * data store for initialization, or NULL if no data is to be copied.
   *  @param `usage` Specifies the expected usage pattern of the data store. The
   * symbolic constant must be `GL_STREAM_DRAW`, `GL_STREAM_READ`,
   * `GL_STREAM_COPY`, `GL_STATIC_DRAW`, `GL_STATIC_READ`, `GL_STATIC_COPY`,
   * `GL_DYNAMIC_DRAW`, `GL_DYNAMIC_READ`, or `GL_DYNAMIC_COPY`.
   */
  virtual void bufferData(GLenum target, GLsizeiptr size, const void* data,
                          GLenum usage) const noexcept = 0;

  /*! `glBufferSubData`, `glNamedBufferSubData` — updates a subset of a buffer
   * object's data store
   *
   *  @brief `glBufferSubData` and `glNamedBufferSubData` redefine some or all
   * of the data store for the specified buffer object.
   *
   *  @param `target` Specifies the target to which the buffer object is bound
   * for `glBufferSubData`
   *  @param `offset` Specifies the offset into the buffer object's data store
   * where data replacement will begin, measured in bytes.
   *  @param `size` Specifies the size in bytes of the data store region being
   * replaced.
   *  @param `data` Specifies a pointer to the new data that will be copied into
   * the data store.
   */
  virtual void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                             const void* data) const noexcept = 0;

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

  /*! `glCompileShader` — Compiles a shader object
   *
   *  @brief `glCompileShader` compiles the source code strings that have been
   * stored in the shader object specified by shader.
   *
   *  @param `shader` Specifies the shader object to be compiled.
   */
  virtual void compileShader(GLuint shader) const noexcept = 0;

  /*! `glCopyBufferSubData`, `glCopyNamedBufferSubData` — copy all or part of
   * the data store of a buffer object to the data store of another buffer
   * object
   *
   *  @brief `glCopyBufferSubData` and `glCopyNamedBufferSubData` copy part of
   * the data store attached to a source buffer object to the data store
   * attached to a destination buffer object.
   *
   *  @param `readTarget` Specifies the target to which the source buffer object
   * is bound for `glCopyBufferSubData`
   *  @param `writeTarget` Specifies the target to which the destination buffer
   * object is bound for `glCopyBufferSubData`.
   *  @param `readOffset` Specifies the offset, in basic machine units, within
   * the data store of the source buffer object at which data will be read.
   *  @param `writeOffset` Specifies the offset, in basic machine units, within
   * the data store of the destination buffer object at which data will be
   * written.
   *  @param `size` Specifies the size, in basic machine units, of the data to
   * be copied from the source buffer object to the destination buffer object.
   */
  virtual void copyBufferSubData(GLenum readTarget, GLenum writeTarget,
                                 GLintptr readOffset, GLintptr writeOffset,
                                 GLsizeiptr size) const noexcept = 0;

  /*! `glCopyBufferSubData`, `glCopyNamedBufferSubData` — copy all or part of
   * the data store of a buffer object to the data store of another buffer
   * object
   *
   *  @brief `glCopyBufferSubData` and `glCopyNamedBufferSubData` copy part of
   * the data store attached to a source buffer object to the data store
   * attached to a destination buffer object.
   *
   *  @param `readBuffer` Specifies the name of the source buffer object for
   * `glCopyNamedBufferSubData`.
   *  @param `writeBuffer` Specifies the name of the destination buffer object
   * for `glCopyNamedBufferSubData`.
   *  @param `readOffset` Specifies the offset, in basic machine units, within
   * the data store of the source buffer object at which data will be read.
   *  @param `writeOffset` Specifies the offset, in basic machine units, within
   * the data store of the destination buffer object at which data will be
   * written.
   *  @param `size` Specifies the size, in basic machine units, of the data to
   * be copied from the source buffer object to the destination buffer object.
   */
  virtual void copyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer,
                                      GLintptr readOffset, GLintptr writeOffset,
                                      GLsizeiptr size) const noexcept = 0;

  /*! `glCreateProgram` — Creates a program object
   *
   *  @brief `glCreateProgram` creates an empty program object and returns a
   * non-zero value by which it can be referenced.
   *
   *  @return A non-zero value by which it can be referenced.
   */
  virtual GLuint createProgram() const noexcept = 0;

  /*! `glCreateShader` — Creates a shader object
   *
   *  @brief `glCreateShader` creates an empty shader object and returns a
   * non-zero value by which it can be referenced.
   *
   *  @param `shaderType` Specifies the type of shader to be created. Must be
   * one of `GL_COMPUTE_SHADER`, `GL_VERTEX_SHADER`, `GL_TESS_CONTROL_SHADER`,
   * `GL_TESS_EVALUATION_SHADER`, `GL_GEOMETRY_SHADER`, or `GL_FRAGMENT_SHADER`.
   *
   *  @return A non-zero value by which it can be referenced.
   */
  virtual GLuint createShader(GLuint shaderType) const noexcept = 0;

  /*! `glDeleteBuffers` — delete named buffer objects
   *
   *  @brief `glDeleteBuffers` deletes n buffer objects named by the elements of
   * the array buffers.
   *
   *  @param `n` Specifies the number of buffer objects to be deleted.
   *  @param `buffers` Specifies an array of buffer objects to be deleted.
   */
  virtual void deleteBuffers(GLsizei n, GLuint* buffers) const noexcept = 0;

  /*! `glDeleteProgram` — Deletes a program object
   *
   *  @brief `glDeleteProgram` frees the memory and invalidates the name
   * associated with the program object specified by program.
   *
   *  @param `program` Specifies the program object to be deleted
   */
  virtual void deleteProgram(GLuint program) const noexcept = 0;

  /*! `glDeleteShader` — Deletes a shader object
   *
   *  @brief `glDeleteShader` frees the memory and invalidates the name
   * associated with the shader object specified by shader.
   *
   *  @param `shader` Species the shader object to be deleted.
   */
  virtual void deleteShader(GLuint) const noexcept = 0;

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

  /*! `glGenBuffers` — generate buffer object names
   *
   *  @brief `glGenBuffers` returns n buffer object names in buffers.
   *
   *  @param `n` Specifies the number of buffer object names to be generated.
   *  @param `buffers` Specifies an array in which the generated buffer object
   * names are stored.
   */
  virtual void genBuffers(GLsizei n, GLuint* buffers) const noexcept = 0;

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

  /*! `glGetProgramInfoLog` — Returns the information log for a program object
   *
   *  @brief `glGetProgramInfoLog` returns the information log for the specified
   * program object.
   *
   *  @param `program` Specifies the program object whose information log is to
   * be queried.
   *  @param `maxLength` Specifies the size of the character buffer for storing
   * the returned information log.
   *  @param `length` Returns the length of the string returned in infoLog
   * (excluding the null terminator).
   *  @param `infoLog` Specifies an array of characters that is used to return
   * the information log.
   */
  virtual void getProgramInfoLog(GLuint, GLsizei, GLsizei*,
                                 GLchar*) const noexcept = 0;

  /*! `glGetProgramiv` — Returns a parameter from a program object
   *
   *  @brief `glGetProgram` returns in params the value of a parameter for a
   * specific program object.
   *
   *  @param `program` Specifies the program object to be queried.
   *  @param `pname` Specifies the object parameter. Accepted symbolic names are
   * `GL_DELETE_STATUS`, `GL_LINK_STATUS`, `GL_VALIDATE_STATUS`,
   * `GL_INFO_LOG_LENGTH`, `GL_ATTACHED_SHADERS`,
   * `GL_ACTIVE_ATOMIC_COUNTER_BUFFERS`, `GL_ACTIVE_ATTRIBUTES`,
   * `GL_ACTIVE_ATTRIBUTE_MAX_LENGTH`, `GL_ACTIVE_UNIFORMS`,
   * `GL_ACTIVE_UNIFORM_BLOCKS`, `GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH`,
   * `GL_ACTIVE_UNIFORM_MAX_LENGTH`, `GL_COMPUTE_WORK_GROUP_SIZE`,
   * `GL_PROGRAM_BINARY_LENGTH`, `GL_TRANSFORM_FEEDBACK_BUFFER_MODE`,
   * `GL_TRANSFORM_FEEDBACK_VARYINGS`,
   * `GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH`, `GL_GEOMETRY_VERTICES_OUT`,
   * `GL_GEOMETRY_INPUT_TYPE`, and `GL_GEOMETRY_OUTPUT_TYPE`.
   *  @param `params` Returns the requested object parameter.
   */
  virtual void getProgramiv(GLuint program, GLenum pname,
                            GLint* params) const noexcept = 0;

  /*! `glGetShaderInfoLog` — Returns the information log for a shader object
   *
   *  @brief `glGetShaderInfoLog` returns the information log for the specified
   * shader object.
   *
   *  @param `shader` Specifies the shader object whose information log is to be
   * queried.
   *  @param `maxLength` Specifies the size of the character buffer for storing
   * the returned information log.
   *  @param `length` Returns the length of the string returned in infoLog
   * (excluding the null terminator).
   *  @param `infoLog` Specifies an array of characters that is used to return
   * the information log.
   */
  virtual void getShaderInfoLog(GLuint shader, GLsizei maxLength,
                                GLsizei* length,
                                GLchar* infoLog) const noexcept = 0;

  /*! `glGetShaderiv` — Returns a parameter from a shader object
   *
   *  @brief `glGetShader` returns in params the value of a parameter for a
   * specific shader object.
   *
   *  @param `shader` Specifies the shader object to be queried.
   *  @param `pname` Specifies the object parameter. Accepted symbolic names are
   * `GL_SHADER_TYPE`, `GL_DELETE_STATUS`, `GL_COMPILE_STATUS`,
   * `GL_INFO_LOG_LENGTH`, `GL_SHADER_SOURCE_LENGTH`.
   *  @param `params` Returns the requested object parameter.
   */
  virtual void getShaderiv(GLuint shader, GLenum pname,
                           GLint* params) const noexcept = 0;

  /*! `glGetUniformLocation` — Returns the location of a uniform variable
   *
   *  @brief `glGetUniformLocation` returns an integer that represents the
   * location of a specific uniform variable within a program object.
   *
   *  @param `program` Specifies the program object to be queried.
   *  @param `name` Points to a null terminated string containing the name of
   * the uniform variable whose location is to be queried.
   *
   *  @returns An integer that represents the location of a specific uniform
   * variable within a program object.
   */
  virtual GLint getUniformLocation(GLuint, const GLchar*) const noexcept = 0;

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

  /*! `glLinkProgram` — Links a program object
   *
   *  @brief `glLinkProgram` links the program object specified by program.
   *
   *  @param `program` Specifies the handle of the program object to be linked.
   */
  virtual void linkProgram(GLuint program) const noexcept = 0;

  /*! `glBufferSubData`, `glNamedBufferSubData` — updates a subset of a buffer
   * object's data store
   *
   *  @brief `glBufferSubData` and `glNamedBufferSubData` redefine some or all
   * of the data store for the specified buffer object.
   *
   *  @param `buffer` Specifies the name of the buffer object for
   * `glNamedBufferSubData`.
   *  @param `offset` Specifies the offset into the buffer object's data store
   * where data replacement will begin, measured in bytes.
   *  @param `size` Specifies the size in bytes of the data store region being
   * replaced.
   *  @param `data` Specifies a pointer to the new data that will be copied into
   * the data store.
   */
  virtual void namedBufferSubData(GLuint buffer, GLintptr offset,
                                  GLsizeiptr size,
                                  const void* data) const noexcept = 0;

  /*! `glShaderSource` — Replaces the source code in a shader object
   *
   *  @brief `glShaderSource` sets the source code in shader to the source code
   * in the array of strings specified by string.
   *
   *  @param `shader` Specifies the handle of the shader object whose source
   * code is to be replaced.
   *  @param `count` Specifies the number of elements in the string and length
   * arrays.
   *  @param `string` Specifies an array of pointers to strings containing the
   * source code to be loaded into the shader.
   *  @param `length` Specifies an array of string lengths.
   */
  virtual void shaderSource(GLuint shader, GLsizei count, const GLchar** source,
                            const GLint* length) const noexcept = 0;

  /*! `glUseProgram` — Installs a program object as part of current rendering
   * state
   *
   *  @brief `glUseProgram` installs the program object specified by program as
   * part of current rendering state.
   *
   *  @param `program` Specifies the handle of the program object whose
   * executables are to be used as part of current rendering state.
   */
  virtual void useProgram(GLuint program) const noexcept = 0;

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

class OpenGLApiImpl : public OpenGLApi {
 public:
  static const OpenGLApiImpl* Instance() noexcept {
    static OpenGLApiImpl instance;
    return &instance;
  }

  OpenGLApiImpl(const OpenGLApiImpl&) noexcept = delete;
  OpenGLApiImpl& operator=(const OpenGLApiImpl&) noexcept = delete;
  OpenGLApiImpl(OpenGLApiImpl&&) noexcept = delete;
  OpenGLApiImpl& operator=(OpenGLApiImpl&&) noexcept = delete;

  inline void attachShader(GLuint program,
                           GLuint shader) const noexcept override {
    glAttachShader(program, shader);
  }

  inline void bindBuffer(GLenum target, GLuint id) const noexcept override {
    glBindBuffer(target, id);
  }

  inline void bufferData(GLenum target, GLsizeiptr size, const void* data,
                         GLenum usage) const noexcept override {
    glBufferData(target, size, data, usage);
  }

  inline void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                            const void* data) const noexcept override {
    glBufferSubData(target, offset, size, data);
  }

  inline void copyBufferSubData(GLenum readTarget, GLenum writeTarget,
                                GLintptr readOffset, GLintptr writeOffset,
                                GLsizeiptr size) const noexcept override {
    glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
  }

  inline void copyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer,
                                     GLintptr readOffset, GLintptr writeOffset,
                                     GLsizeiptr size) const noexcept override {
    glCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset,
                             size);
  }

  inline void clear(GLbitfield mask) const noexcept override { glClear(mask); }

  inline void clearColor(GLfloat r, GLfloat g, GLfloat b,
                         GLfloat a) const noexcept override {
    glClearColor(r, g, b, a);
  }

  inline void compileShader(GLuint shader) const noexcept override {
    glCompileShader(shader);
  }

  inline GLuint createProgram() const noexcept override {
    return glCreateProgram();
  }

  inline GLuint createShader(GLenum stage) const noexcept override {
    return glCreateShader(stage);
  }

  inline void deleteBuffers(GLsizei count,
                            GLuint* ptr) const noexcept override {
    glDeleteBuffers(count, ptr);
  }

  inline void deleteProgram(GLuint program) const noexcept override {
    glDeleteProgram(program);
  }

  inline void deleteShader(GLuint program) const noexcept override {
    glDeleteShader(program);
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

  inline void genBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    glGenBuffers(count, ptr);
  }

  inline void getShaderiv(GLuint shader, GLenum pname,
                          GLint* params) const noexcept override {
    glGetShaderiv(shader, pname, params);
  }

  inline void getProgramiv(GLuint program, GLenum pname,
                           GLint* params) const noexcept override {
    glGetProgramiv(program, pname, params);
  }

  inline void getProgramInfoLog(GLuint program, GLsizei maxLength,
                                GLsizei* length,
                                GLchar* infoLog) const noexcept override {
    glGetProgramInfoLog(program, maxLength, length, infoLog);
  }

  inline void getShaderInfoLog(GLuint shader, GLsizei maxLength,
                               GLsizei* length,
                               GLchar* infoLog) const noexcept override {
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
  }

  inline GLint getUniformLocation(GLuint program,
                                  const GLchar* name) const noexcept override {
    return glGetUniformLocation(program, name);
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

  inline void linkProgram(GLuint program) const noexcept override {
    glLinkProgram(program);
  }

  inline void namedBufferSubData(GLuint buffer, GLintptr offset,
                                 GLsizeiptr size,
                                 const void* data) const noexcept override {
    glNamedBufferSubData(buffer, offset, size, data);
  }

  inline void shaderSource(GLuint shader, GLsizei count, const GLchar** source,
                           const GLint* length) const noexcept override {
    glShaderSource(shader, count, source, length);
  }

  inline void useProgram(GLuint program) const noexcept override {
    glUseProgram(program);
  }

  inline void viewport(GLint x, GLint y, GLsizei width,
                       GLsizei height) const noexcept override {
    glViewport(x, y, width, height);
  }

 private:
  OpenGLApiImpl() noexcept = default;
};

struct BufferSegment {
  GLsizeiptr offset = 0;
  GLsizeiptr size = 0;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_GL_H_
