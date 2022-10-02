#ifndef UINTA_GL_ERRORS_HPP
#define UINTA_GL_ERRORS_HPP

#define UINTA_glGetError(m)

#ifdef UINTA_DEBUG

#include <uinta/logging.hpp>

#define UINTA_glGetError_CASE(method, error)                                                                                   \
  case error:                                                                                                                  \
    SPDLOG_ERROR("{} generated error {}. See https://registry.khronos.org/OpenGL-Refpages/gl4/html/{}.xhtml", #method, #error, \
                 method);                                                                                                      \
    break;

#undef UINTA_glGetError
#define UINTA_glGetError(method)                                              \
  {                                                                           \
    if (auto error = glGetError()) {                                          \
      switch (error) {                                                        \
        UINTA_glGetError_CASE(#method, GL_CONTEXT_LOST);                      \
        UINTA_glGetError_CASE(#method, GL_INVALID_ENUM);                      \
        UINTA_glGetError_CASE(#method, GL_INVALID_FRAMEBUFFER_OPERATION);     \
        UINTA_glGetError_CASE(#method, GL_INVALID_OPERATION);                 \
        UINTA_glGetError_CASE(#method, GL_INVALID_VALUE);                     \
        UINTA_glGetError_CASE(#method, GL_OUT_OF_MEMORY);                     \
        UINTA_glGetError_CASE(#method, GL_STACK_OVERFLOW);                    \
        UINTA_glGetError_CASE(#method, GL_STACK_UNDERFLOW);                   \
        default:                                                              \
          SPDLOG_WARN("{} generated unknown error code {}.", #method, error); \
          break;                                                              \
      }                                                                       \
    }                                                                         \
  }

#endif  // UINTA_DEBUG

#endif  // UINTA_GL_ERRORS_HPP
