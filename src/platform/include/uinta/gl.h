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

  /*! `glActivateTexture` — Select active texture unit
   *
   *  @brief In order to create a complete shader program, there must be a way
   * to specify the list of things that will be linked together.
   *
   *  @param `texture` Specifies which texture unit to make active.
   */
  virtual void activeTexture(GLenum) const noexcept = 0;

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

  /*! `glBindTexture` — bind a named texture to a texturing target
   *
   *  @brief `glBindTexture` lets you create or use a named texture.
   *
   *  @param `target` Specifies the target to which the texture is bound. Must
   * be one of `GL_TEXTURE_1D`, `GL_TEXTURE_2D`, `GL_TEXTURE_3D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D_ARRAY`, `GL_TEXTURE_RECTANGLE`,
   * `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, `GL_TEXTURE_BUFFER`,
   * `GL_TEXTURE_2D_MULTISAMPLE` or `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`.
   *  @param `texture` Specifies the name of a texture.
   */
  virtual void bindTexture(GLenum target, GLuint texture) const noexcept = 0;

  /*! `glBindVertexArray` — bind a vertex array object
   *
   *  @brief `glBindVertexArray` binds the vertex array object with name array.
   *
   *  @param `array` Specifies the name of the vertex array to bind.
   */
  virtual void bindVertexArray(GLuint array) const noexcept = 0;

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

  /*! `glDeleteTextures` — delete named textures
   *
   *  @brief `glDeleteTextures` deletes n textures named by the elements of the
   * array textures.
   *
   *  @param `n` Specifies the number of textures to be deleted.
   *  @param `textures` Specifies an array of textures to be deleted.
   */
  virtual void deleteTextures(GLsizei n,
                              const GLuint* textures) const noexcept = 0;

  /*! glDeleteVertexArrays — delete vertex array objects
   *
   *  @brief `glDeleteVertexArrays` deletes n vertex array objects whose names
   * are stored in the array addressed by arrays.
   *
   *  @param `n` Specifies the number of vertex array object names to be
   * deleted.
   *  @param `arrays` Specifies the address of an array containing the n names
   * of the objects to be deleted.
   */
  virtual void deleteVertexArrays(GLsizei, GLuint*) const noexcept = 0;

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

  /*! `glEnableVertexAttribArray` — Enable or disable a generic vertex attribute
   * array
   *
   *  @brief `glEnableVertexAttribArray` and `glEnableVertexArrayAttrib` enable
   * the generic vertex attribute array specified by index.
   *
   *  @param `index` Specifies the index of the generic vertex attribute to be
   * enabled or disabled.
   */
  virtual void enableVertexAttribArray(GLuint index) const noexcept = 0;

  /*! `glEnableVertexAttribArray` — Enable or disable a generic vertex attribute
   * array
   *
   *  @brief `glEnableVertexAttribArray` and `glEnableVertexArrayAttrib` enable
   * the generic vertex attribute array specified by index.
   *
   *  @param `index` Specifies the index of the generic vertex attribute to be
   * enabled or disabled.
   *  @param `vaobj` Specifies the name of the vertex array object for
   * `glDisableVertexArrayAttrib` and `glEnableVertexArrayAttrib` functions
   */
  virtual void enableVertexArrayAttrib(GLuint vaobj,
                                       GLuint index) const noexcept = 0;

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

  /*! `glGenTextures` — generate texture names
   *
   *  @brief `glGenTextures` returns n texture names in textures.
   *
   *  @param `n` Specifies the number of texture names to be generated.
   *  @param `textures` Specifies an array in which the generated texture names
   * are stored.
   */
  virtual void genTextures(GLsizei n, GLuint* textures) const noexcept = 0;

  /*! `glGenVertexArrays` — generate vertex array object names
   *
   *  @brief `glGenVertexArrays` returns n vertex array object names in arrays.
   *
   *  @param `n` Specifies the number of vertex array object names to generate.
   *  @param `arrays` Specifies an array in which the generated vertex array
   * object names are stored.
   */
  virtual void genVertexArrays(GLsizei n, GLuint* arrays) const noexcept = 0;

  /*! `glGenerateMipmap`, `glGenerateTextureMipmap` — generate mipmaps for a
   * specified texture object
   *
   *  @brief `glGenerateMipmap` and `glGenerateTextureMipmap` generates mipmaps
   * for the specified texture object.
   *
   *  @param `target` Specifies the target to which the texture object is bound
   * for `glGenerateMipmap`. Must be one of `GL_TEXTURE_1D`, `GL_TEXTURE_2D`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_CUBE_MAP`, or `GL_TEXTURE_CUBE_MAP_ARRAY`.
   */
  virtual void generateMipmap(GLenum target) const noexcept = 0;

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

  /*! `glLineWidth` — Specify the width of rasterized lines
   *
   *  @brief `glLineWidth` specifies the rasterized width of both aliased and
   * antialiased lines.
   *
   *  @param `width` Specifies the width of rasterized lines. The initial value
   * is 1.
   */
  virtual void lineWidth(GLfloat width) const noexcept = 0;

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

  /*! `glPointSize` — specify the diameter of rasterized points
   *
   *  @brief `glPointSize` specifies the rasterized diameter of points.
   *
   *  @param `size` Specifies the diameter of rasterized points. The initial
   * value is 1.
   */
  virtual void pointSize(GLfloat size) const noexcept = 0;

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

  /*! `glTexImage2D` — specify a two-dimensional texture image
   *
   *  @brief Texturing allows elements of an image array to be read by shaders.
   *
   *  @param `target` Specifies the target texture. Must be `GL_TEXTURE_2D`,
   * `GL_PROXY_TEXTURE_2D`, `GL_TEXTURE_1D_ARRAY`, `GL_PROXY_TEXTURE_1D_ARRAY`,
   * `GL_TEXTURE_RECTANGLE`, `GL_PROXY_TEXTURE_RECTANGLE`,
   * `GL_TEXTURE_CUBE_MAP_POSITIVE_X`, `GL_TEXTURE_CUBE_MAP_NEGATIVE_X`,
   * `GL_TEXTURE_CUBE_MAP_POSITIVE_Y`, `GL_TEXTURE_CUBE_MAP_NEGATIVE_Y`,
   * `GL_TEXTURE_CUBE_MAP_POSITIVE_Z`, `GL_TEXTURE_CUBE_MAP_NEGATIVE_Z`, or
   * `GL_PROXY_TEXTURE_CUBE_MAP`.
   *  @param `level` Specifies the level-of-detail number. Level 0 is the base
   * image level. Level n is the nth mipmap reduction image. If target is
   * `GL_TEXTURE_RECTANGLE` or `GL_PROXY_TEXTURE_RECTANGLE`, level must be 0.
   *  @param `internalformat` Specifies the number of color components in the
   * texture. Must be one of base internal formats given in Table 1, one of the
   * sized internal formats given in Table 2, or one of the compressed internal
   * formats given in Table 3, below.
   *  @param `width` Specifies the width of the texture image. All
   * implementations support texture images that are at least 1024 texels wide.
   *  @param `height` Specifies the height of the texture image, or the number
   * of layers in a texture array, in the case of the `GL_TEXTURE_1D_ARRAY` and
   * `GL_PROXY_TEXTURE_1D_ARRAY` targets. All implementations support 2D texture
   * images that are at least 1024 texels high, and texture arrays that are at
   * least 256 layers deep.
   *  @param `border` This value must be 0.
   *  @param `format` Specifies the format of the pixel data. The following
   * symbolic values are accepted: `GL_RED`, `GL_RG`, `GL_RGB`, `GL_BGR`,
   * `GL_RGBA`, `GL_BGRA`, `GL_RED_INTEGER`, `GL_RG_INTEGER`, `GL_RGB_INTEGER`,
   * `GL_BGR_INTEGER`, `GL_RGBA_INTEGER`, `GL_BGRA_INTEGER`, `GL_STENCIL_INDEX`,
   * `GL_DEPTH_COMPONENT`, `GL_DEPTH_STENCIL`.
   *  @param `type` Specifies the data type of the pixel data. The following
   * symbolic values are accepted: `GL_UNSIGNED_BYTE`, `GL_BYTE`,
   * `GL_UNSIGNED_SHORT`, `GL_SHORT`, `GL_UNSIGNED_INT`, `GL_INT`,
   * `GL_HALF_FLOAT`, `GL_FLOAT`, `GL_UNSIGNED_BYTE_3_3_2`,
   * `GL_UNSIGNED_BYTE_2_3_3_REV`, `GL_UNSIGNED_SHORT_5_6_5`,
   * `GL_UNSIGNED_SHORT_5_6_5_REV`, `GL_UNSIGNED_SHORT_4_4_4_4`,
   * `GL_UNSIGNED_SHORT_4_4_4_4_REV`, `GL_UNSIGNED_SHORT_5_5_5_1`,
   * `GL_UNSIGNED_SHORT_1_5_5_5_REV`, `GL_UNSIGNED_INT_8_8_8_8`,
   * `GL_UNSIGNED_INT_8_8_8_8_REV`, `GL_UNSIGNED_INT_10_10_10_2`, and
   * `GL_UNSIGNED_INT_2_10_10_10_REV`.
   *  @param `data` Specifies a pointer to the image data in memory.
   */
  virtual void texImage2D(GLenum target, GLint level, GLint internalformat,
                          GLsizei width, GLsizei height, GLint border,
                          GLenum format, GLenum type,
                          const void* data) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void texParameterIiv(GLenum target, GLenum pname,
                               const GLint* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void texParameterIuiv(GLenum target, GLenum pname,
                                const GLuint* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `param` For the scalar commands, specifies the value of pname.
   */
  virtual void texParameterf(GLenum target, GLenum pname,
                             GLfloat param) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `param` For the scalar commands, specifies the value of pname.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void texParameterfv(GLenum target, GLenum pname,
                              const GLfloat* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `param` For the scalar commands, specifies the value of pname.
   */
  virtual void texParameteri(GLenum target, GLenum pname,
                             GLint param) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `target` Specifies the target to which the texture is bound for
   * `glTexParameter` functions. Must be one of `GL_TEXTURE_1D`,
   * `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`,
   * `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`,
   * `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or
   * `GL_TEXTURE_RECTANGLE`.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void texParameteriv(GLenum target, GLenum pname,
                              const GLint* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void textureParameterIiv(GLuint texture, GLenum pname,
                                   const GLint* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void textureParameterIuiv(GLuint texture, GLenum pname,
                                    const GLuint* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `param` For the scalar commands, specifies the value of pname.
   */
  virtual void textureParameterf(GLuint texture, GLenum pname,
                                 GLfloat param) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void textureParameterfv(GLuint texture, GLenum pname,
                                  const GLfloat* params) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `param` For the scalar commands, specifies the value of pname.
   */
  virtual void textureParameteri(GLuint texture, GLenum pname,
                                 GLint param) const noexcept = 0;

  /*! `glTexParameter`, `glTextureParameter` — set texture parameters
   *
   *  @brief `glTexParameter` and `glTextureParameter` assign the value or
   * values in params to the texture parameter specified as pname.
   *
   *  @param `texture` Specifies the texture object name for
   * `glTextureParameter` functions.
   *  @param `pname` Specifies the symbolic name of a sin`gle`-valued texture
   * parameter. pname can be one of the following:
   * `GL_DEPTH_STENCIL_TEXTURE_MODE`, `GL_TEXTURE_BASE_LEVEL`,
   * `GL_TEXTURE_COMPARE_FUNC`, `GL_TEXTURE_COMPARE_MODE`,
   * `GL_TEXTURE_LOD_BIAS`, `GL_TEXTURE_MIN_FILTER`, `GL_TEXTURE_MAG_FILTER`,
   * `GL_TEXTURE_MIN_LOD`, `GL_TEXTURE_MAX_LOD`, `GL_TEXTURE_MAX_LEVEL`,
   * `GL_TEXTURE_SWIZZLE_R`, `GL_TEXTURE_SWIZZLE_G`, `GL_TEXTURE_SWIZZLE_B`,
   * `GL_TEXTURE_SWIZZLE_A`, `GL_TEXTURE_WRAP_S`, `GL_TEXTURE_WRAP_T`, or
   * `GL_TEXTURE_WRAP_R`. For the vector commands (`glTexParameter`*v), pname
   * can also be one of `GL_TEXTURE_BORDER_COLOR` or `GL_TEXTURE_SWIZZLE_RGBA`.
   *  @param `params` For the vector commands, specifies a pointer to an array
   * where the value or values of pname are stored.
   */
  virtual void textureParameteriv(GLuint texture, GLenum pname,
                                  const GLint* params) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform1f(GLint location, GLfloat v0) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform2f(GLint location, GLfloat v0,
                         GLfloat v1) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform3f(GLint location, GLfloat v0, GLfloat v1,
                         GLfloat v2) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v3` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
                         GLfloat v3) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform1i(GLint location, GLint v0) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform2i(GLint location, GLint v0, GLint v1) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform3i(GLint location, GLint v0, GLint v1,
                         GLint v2) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v3` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform4i(GLint location, GLint v0, GLint v1, GLint v2,
                         GLint v3) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform1ui(GLint location, GLuint v0) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform2ui(GLint location, GLuint v0,
                          GLuint v1) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform3ui(GLint location, GLuint v0, GLuint v1,
                          GLuint v2) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `v0` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v1` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v2` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   *  @param `v3` For the scalar commands, specifies the new values to be used
   * for the specified uniform variable.
   */
  virtual void uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
                          GLuint v3) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform1fv(GLint location, GLsizei count,
                          const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform2fv(GLint location, GLsizei count,
                          const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform3fv(GLint location, GLsizei count,
                          const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform4fv(GLint location, GLsizei count,
                          const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform1iv(GLint location, GLsizei count,
                          const GLint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform2iv(GLint location, GLsizei count,
                          const GLint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform3iv(GLint location, GLsizei count,
                          const GLint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform4iv(GLint location, GLsizei count,
                          const GLint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform1uiv(GLint location, GLsizei count,
                           const GLuint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniform2uiv(GLint location, GLsizei count,
                           const GLuint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   */
  virtual void uniform3uiv(GLint location, GLsizei count,
                           const GLuint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   */
  virtual void uniform4uiv(GLint location, GLsizei count,
                           const GLuint* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix2fv(GLint location, GLsizei count,
                                GLboolean transpose,
                                const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix3fv(GLint location, GLsizei count,
                                GLboolean transpose,
                                const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix4fv(GLint location, GLsizei count,
                                GLboolean transpose,
                                const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix2x3fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix3x2fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix2x4fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix4x2fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix3x4fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

  /*! `glUniform` — Specify the value of a uniform variable for the current
   * program object
   *
   *  @brief `glUniform` modifies the value of a uniform variable or a uniform
   * variable array.
   *
   *  @param `location` Specifies the location of the uniform variable to be
   * modified.
   *  @param `count` For the vector (`glUniform`*v) commands, specifies the
   * number of elements that are to be modified. This should be 1 if the
   * targeted uniform variable is not an array, and 1 or more if it is an array.
   * For the matrix (`glUniformMatrix`*) commands, specifies the number of
   * matrices that are to be modified. This should be 1 if the targeted uniform
   * variable is not an array of matrices, and 1 or more if it is an array of
   * matrices.
   *  @param `transpose` For the matrix commands, specifies whether to transpose
   * the matrix as the values are loaded into the uniform variable.
   *  @param `value` For the vector and matrix commands, specifies a pointer to
   * an array of count values that will be used to update the specified uniform
   * variable.
   */
  virtual void uniformMatrix4x3fv(GLint location, GLsizei count,
                                  GLboolean transpose,
                                  const GLfloat* value) const noexcept = 0;

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

  /*! `glVertexAttribPointer` — define an array of generic vertex attribute data
   *
   *  @brief `glVertexAttribPointer`, `glVertexAttribIPointer` and
   * `glVertexAttribLPointer` specify the location and data format of the array
   * of generic vertex attributes at index index to use when rendering.
   *
   *  @param `index` Specifies the index of the generic vertex attribute to be
   * modified.
   *  @param `size` Specifies the number of components per generic vertex
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant
   * `GL_BGRA` is accepted by `glVertexAttribPointer`. The initial value is 4.
   *  @param `type` Specifies the data type of each component in the array. The
   * symbolic constants `GL_BYTE`, `GL_UNSIGNED_BYTE`, `GL_SHORT`,
   * `GL_UNSIGNED_SHORT`, `GL_INT`, and `GL_UNSIGNED_INT` are accepted by
   * `glVertexAttribPointer` and `glVertexAttribIPointer`. Additionally
   * `GL_HALF_FLOAT`, `GL_FLOAT`, `GL_DOUBLE`, `GL_FIXED`,
   * `GL_INT_2_10_10_10_REV`, `GL_UNSIGNED_INT_2_10_10_10_REV` and
   * `GL_UNSIGNED_INT_10F_11F_11F_REV` are accepted by `glVertexAttribPointer`.
   * `GL_DOUBLE` is also accepted by `glVertexAttribLPointer` and is the only
   * token accepted by the type parameter for that function. The initial value
   * is `GL_FLOAT`.
   *  @param `normalized` For `glVertexAttribPointer`, specifies whether
   * fixed-point data values should be normalized (`GL_TRUE`) or converted
   * directly as fixed-point values (`GL_FALSE`) when they are accessed.
   *  @param `stride` Specifies the byte offset between consecutive generic
   * vertex attributes. If stride is 0, the generic vertex attributes are
   * understood to be tightly packed in the array. The initial value is 0.
   *  @param `pointer` Specifies a offset of the first component of the first
   * generic vertex attribute in the array in the data store of the buffer
   * currently bound to the `GL_ARRAY_BUFFER` target. The initial value is 00.
   */
  virtual void vertexAttribPointer(GLuint index, GLint size, GLenum type,
                                   GLboolean normalized, GLsizei stride,
                                   const void* pointer) const noexcept = 0;

  /*! `glVertexAttribPointer` — define an array of generic vertex attribute data
   *
   *  @brief `glVertexAttribPointer`, `glVertexAttribIPointer` and
   * `glVertexAttribLPointer` specify the location and data format of the array
   * of generic vertex attributes at index index to use when rendering.
   *
   *  @param `index` Specifies the index of the generic vertex attribute to be
   * modified.
   *  @param `size` Specifies the number of components per generic vertex
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant
   * `GL_BGRA` is accepted by `glVertexAttribPointer`. The initial value is 4.
   *  @param `type` Specifies the data type of each component in the array. The
   * symbolic constants `GL_BYTE`, `GL_UNSIGNED_BYTE`, `GL_SHORT`,
   * `GL_UNSIGNED_SHORT`, `GL_INT`, and `GL_UNSIGNED_INT` are accepted by
   * `glVertexAttribPointer` and `glVertexAttribIPointer`. Additionally
   * `GL_HALF_FLOAT`, `GL_FLOAT`, `GL_DOUBLE`, `GL_FIXED`,
   * `GL_INT_2_10_10_10_REV`, `GL_UNSIGNED_INT_2_10_10_10_REV` and
   * `GL_UNSIGNED_INT_10F_11F_11F_REV` are accepted by `glVertexAttribPointer`.
   * `GL_DOUBLE` is also accepted by `glVertexAttribLPointer` and is the only
   * token accepted by the type parameter for that function. The initial value
   * is `GL_FLOAT`.
   *  @param `stride` Specifies the byte offset between consecutive generic
   * vertex attributes. If stride is 0, the generic vertex attributes are
   * understood to be tightly packed in the array. The initial value is 0.
   *  @param `pointer` Specifies a offset of the first component of the first
   * generic vertex attribute in the array in the data store of the buffer
   * currently bound to the `GL_ARRAY_BUFFER` target. The initial value is 00.
   */
  virtual void vertexAttribIPointer(GLuint index, GLint size, GLenum type,
                                    GLsizei stride,
                                    const void* pointer) const noexcept = 0;

  /*! `glVertexAttribPointer` — define an array of generic vertex attribute data
   *
   *  @brief `glVertexAttribPointer`, `glVertexAttribIPointer` and
   * `glVertexAttribLPointer` specify the location and data format of the array
   * of generic vertex attributes at index index to use when rendering.
   *
   *  @param `index` Specifies the index of the generic vertex attribute to be
   * modified.
   *  @param `size` Specifies the number of components per generic vertex
   * attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant
   * `GL_BGRA` is accepted by `glVertexAttribPointer`. The initial value is 4.
   *  @param `type` Specifies the data type of each component in the array. The
   * symbolic constants `GL_BYTE`, `GL_UNSIGNED_BYTE`, `GL_SHORT`,
   * `GL_UNSIGNED_SHORT`, `GL_INT`, and `GL_UNSIGNED_INT` are accepted by
   * `glVertexAttribPointer` and `glVertexAttribIPointer`. Additionally
   * `GL_HALF_FLOAT`, `GL_FLOAT`, `GL_DOUBLE`, `GL_FIXED`,
   * `GL_INT_2_10_10_10_REV`, `GL_UNSIGNED_INT_2_10_10_10_REV` and
   * `GL_UNSIGNED_INT_10F_11F_11F_REV` are accepted by `glVertexAttribPointer`.
   * `GL_DOUBLE` is also accepted by `glVertexAttribLPointer` and is the only
   * token accepted by the type parameter for that function. The initial value
   * is `GL_FLOAT`.
   *  @param `stride` Specifies the byte offset between consecutive generic
   * vertex attributes. If stride is 0, the generic vertex attributes are
   * understood to be tightly packed in the array. The initial value is 0.
   *  @param `pointer` Specifies a offset of the first component of the first
   * generic vertex attribute in the array in the data store of the buffer
   * currently bound to the `GL_ARRAY_BUFFER` target. The initial value is 00.
   */
  virtual void vertexAttribLPointer(GLuint index, GLint size, GLenum type,
                                    GLsizei stride,
                                    const void* pointer) const noexcept = 0;

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

  inline void activeTexture(GLenum texture) const noexcept override {
    glActiveTexture(texture);
  }

  inline void attachShader(GLuint program,
                           GLuint shader) const noexcept override {
    glAttachShader(program, shader);
  }

  inline void bindBuffer(GLenum target, GLuint id) const noexcept override {
    glBindBuffer(target, id);
  }

  inline void bindTexture(GLenum target, GLuint id) const noexcept override {
    glBindTexture(target, id);
  }

  inline void bindVertexArray(GLuint id) const noexcept override {
    glBindVertexArray(id);
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

  inline void deleteTextures(GLsizei count,
                             const GLuint* ptr) const noexcept override {
    glDeleteTextures(count, ptr);
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

  inline void deleteVertexArrays(GLsizei count,
                                 GLuint* ptr) const noexcept override {
    glDeleteVertexArrays(count, ptr);
  }

  inline void enableVertexAttribArray(GLuint index) const noexcept override {
    glEnableVertexAttribArray(index);
  }

  inline void enableVertexArrayAttrib(GLuint vaobj,
                                      GLuint index) const noexcept override {
    glEnableVertexArrayAttrib(vaobj, index);
  }

  inline void genBuffers(GLsizei count, GLuint* ptr) const noexcept override {
    glGenBuffers(count, ptr);
  }

  inline void generateMipmap(GLenum target) const noexcept override {
    glGenerateMipmap(target);
  }

  inline void genTextures(GLsizei count, GLuint* ptr) const noexcept override {
    glGenTextures(count, ptr);
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

  inline void lineWidth(GLfloat width) const noexcept override {
    glLineWidth(width);
  }

  inline void genVertexArrays(GLsizei count,
                              GLuint* ptr) const noexcept override {
    glGenVertexArrays(count, ptr);
  }

  inline void linkProgram(GLuint program) const noexcept override {
    glLinkProgram(program);
  }

  inline void namedBufferSubData(GLuint buffer, GLintptr offset,
                                 GLsizeiptr size,
                                 const void* data) const noexcept override {
    glNamedBufferSubData(buffer, offset, size, data);
  }

  inline void pointSize(GLfloat size) const noexcept override {
    glPointSize(size);
  }

  inline void shaderSource(GLuint shader, GLsizei count, const GLchar** source,
                           const GLint* length) const noexcept override {
    glShaderSource(shader, count, source, length);
  }

  inline void texImage2D(GLenum target, GLint level, GLint internalformat,
                         GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type,
                         const void* data) const noexcept override {
    glTexImage2D(target, level, internalformat, width, height, border, format,
                 type, data);
  }

  inline void texParameterf(GLenum target, GLenum pname,
                            GLfloat param) const noexcept override {
    glTexParameterf(target, pname, param);
  }

  inline void texParameteri(GLenum target, GLenum pname,
                            GLint param) const noexcept override {
    glTexParameteri(target, pname, param);
  }

  inline void textureParameterf(GLuint texture, GLenum pname,
                                GLfloat param) const noexcept override {
    glTextureParameterf(texture, pname, param);
  }

  inline void textureParameteri(GLuint texture, GLenum pname,
                                GLint param) const noexcept override {
    glTextureParameteri(texture, pname, param);
  }

  inline void texParameterfv(GLenum target, GLenum pname,
                             const GLfloat* params) const noexcept override {
    glTexParameterfv(target, pname, params);
  }

  inline void texParameteriv(GLenum target, GLenum pname,
                             const GLint* params) const noexcept override {
    glTexParameteriv(target, pname, params);
  }

  inline void texParameterIiv(GLenum target, GLenum pname,
                              const GLint* params) const noexcept override {
    glTexParameterIiv(target, pname, params);
  }

  inline void texParameterIuiv(GLenum target, GLenum pname,
                               const GLuint* params) const noexcept override {
    glTexParameterIuiv(target, pname, params);
  }

  inline void textureParameterfv(
      GLuint texture, GLenum pname,
      const GLfloat* params) const noexcept override {
    glTextureParameterfv(texture, pname, params);
  }

  inline void textureParameteriv(GLuint texture, GLenum pname,
                                 const GLint* params) const noexcept override {
    glTextureParameteriv(texture, pname, params);
  }

  inline void textureParameterIiv(GLuint texture, GLenum pname,
                                  const GLint* params) const noexcept override {
    glTextureParameterIiv(texture, pname, params);
  }

  inline void textureParameterIuiv(
      GLuint texture, GLenum pname,
      const GLuint* params) const noexcept override {
    glTextureParameterIuiv(texture, pname, params);
  }

  inline void uniform1f(GLint location, GLfloat v0) const noexcept override {
    glUniform1f(location, v0);
  }

  inline void uniform1fv(GLint location, GLsizei count,
                         const GLfloat* value) const noexcept override {
    glUniform1fv(location, count, value);
  }

  inline void uniform1i(GLint location, GLint v0) const noexcept override {
    glUniform1i(location, v0);
  }

  inline void uniform1iv(GLint location, GLsizei count,
                         const GLint* value) const noexcept override {
    glUniform1iv(location, count, value);
  }

  inline void uniform1ui(GLint location, GLuint v0) const noexcept override {
    glUniform1ui(location, v0);
  }

  inline void uniform1uiv(GLint location, GLsizei count,
                          const GLuint* value) const noexcept override {
    glUniform1uiv(location, count, value);
  }

  inline void uniform2f(GLint location, GLfloat v0,
                        GLfloat v1) const noexcept override {
    glUniform2f(location, v0, v1);
  }

  inline void uniform2fv(GLint location, GLsizei count,
                         const GLfloat* value) const noexcept override {
    glUniform2fv(location, count, value);
  }

  inline void uniform2i(GLint location, GLint v0,
                        GLint v1) const noexcept override {
    glUniform2i(location, v0, v1);
  }

  inline void uniform2iv(GLint location, GLsizei count,
                         const GLint* value) const noexcept override {
    glUniform2iv(location, count, value);
  }

  inline void uniform2ui(GLint location, GLuint v0,
                         GLuint v1) const noexcept override {
    glUniform2ui(location, v0, v1);
  }

  inline void uniform2uiv(GLint location, GLsizei count,
                          const GLuint* value) const noexcept override {
    glUniform2uiv(location, count, value);
  }

  inline void uniform3f(GLint location, GLfloat v0, GLfloat v1,
                        GLfloat v2) const noexcept override {
    glUniform3f(location, v0, v1, v2);
  }

  inline void uniform3fv(GLint location, GLsizei count,
                         const GLfloat* value) const noexcept override {
    glUniform3fv(location, count, value);
  }

  inline void uniform3i(GLint location, GLint v0, GLint v1,
                        GLint v2) const noexcept override {
    glUniform3i(location, v0, v1, v2);
  }

  inline void uniform3iv(GLint location, GLsizei count,
                         const GLint* value) const noexcept override {
    glUniform3iv(location, count, value);
  }

  inline void uniform3ui(GLint location, GLuint v0, GLuint v1,
                         GLuint v2) const noexcept override {
    glUniform3ui(location, v0, v1, v2);
  }

  inline void uniform3uiv(GLint location, GLsizei count,
                          const GLuint* value) const noexcept override {
    glUniform3uiv(location, count, value);
  }

  inline void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
                        GLfloat v3) const noexcept override {
    glUniform4f(location, v0, v1, v2, v3);
  }

  inline void uniform4fv(GLint location, GLsizei count,
                         const GLfloat* value) const noexcept override {
    glUniform4fv(location, count, value);
  }

  inline void uniform4i(GLint location, GLint v0, GLint v1, GLint v2,
                        GLint v3) const noexcept override {
    glUniform4i(location, v0, v1, v2, v3);
  }

  inline void uniform4iv(GLint location, GLsizei count,
                         const GLint* value) const noexcept override {
    glUniform4iv(location, count, value);
  }

  inline void uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2,
                         GLuint v3) const noexcept override {
    glUniform4ui(location, v0, v1, v2, v3);
  }

  inline void uniform4uiv(GLint location, GLsizei count,
                          const GLuint* value) const noexcept override {
    glUniform4uiv(location, count, value);
  }

  inline void uniformMatrix2fv(GLint location, GLsizei count,
                               GLboolean transpose,
                               const GLfloat* value) const noexcept override {
    glUniformMatrix2fv(location, count, transpose, value);
  }

  inline void uniformMatrix2x3fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix2x3fv(location, count, transpose, value);
  }

  inline void uniformMatrix2x4fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix2x4fv(location, count, transpose, value);
  }

  inline void uniformMatrix3fv(GLint location, GLsizei count,
                               GLboolean transpose,
                               const GLfloat* value) const noexcept override {
    glUniformMatrix3fv(location, count, transpose, value);
  }

  inline void uniformMatrix3x2fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix3x2fv(location, count, transpose, value);
  }

  inline void uniformMatrix3x4fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix3x4fv(location, count, transpose, value);
  }

  inline void uniformMatrix4fv(GLint location, GLsizei count,
                               GLboolean transpose,
                               const GLfloat* value) const noexcept override {
    glUniformMatrix4fv(location, count, transpose, value);
  }

  inline void uniformMatrix4x2fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix4x2fv(location, count, transpose, value);
  }

  inline void uniformMatrix4x3fv(GLint location, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat* value) const noexcept override {
    glUniformMatrix4x3fv(location, count, transpose, value);
  }

  inline void useProgram(GLuint program) const noexcept override {
    glUseProgram(program);
  }

  inline void vertexAttribPointer(GLuint index, GLint size, GLenum type,
                                  GLboolean normalized, GLsizei stride,
                                  const void* pointer) const noexcept override {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  }

  inline void vertexAttribIPointer(
      GLuint index, GLint size, GLenum type, GLsizei stride,
      const void* pointer) const noexcept override {
    glVertexAttribIPointer(index, size, type, stride, pointer);
  }

  inline void vertexAttribLPointer(
      GLuint index, GLint size, GLenum type, GLsizei stride,
      const void* pointer) const noexcept override {
    glVertexAttribLPointer(index, size, type, stride, pointer);
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

class CapabilityGuard {
 public:
  CapabilityGuard(GLenum capability, bool isActive = true,
                  const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : gl_(gl),
        capability_(capability),
        wasEnabled_(gl_->isEnabled(capability_)) {
    if (isActive && !wasEnabled_) activate();
  }

  virtual ~CapabilityGuard() noexcept {
    if (isActive_ && !wasEnabled_) gl_->disable(capability_);
  }

  virtual void activate() noexcept {
    if (!isActive_) {
      gl_->enable(capability_);
      isActive_ = true;
    }
  }

 protected:
  const OpenGLApi* gl_;
  const GLenum capability_;
  const GLboolean wasEnabled_;
  bool isActive_ = false;
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_GL_H_
