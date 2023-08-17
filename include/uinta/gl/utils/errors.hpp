#ifndef UINTA_GL_ERRORS_HPP
#define UINTA_GL_ERRORS_HPP

#define UINTA_glGetError(m) (void)0

#ifdef UINTA_DEBUG

#include <uinta/flags.h>

#include <uinta/gl/utils/type_utils.hpp>
#include <uinta/logging.hpp>

namespace uinta {

static constexpr flag_t UINTA_GL_EXIT_ON_ERROR = 1 << 0;
static constexpr flags_t UINTA_GL_FLAGS = UINTA_GL_EXIT_ON_ERROR;

#define UINTA_glGetError_CASE(method, error)                                                                                   \
  case error:                                                                                                                  \
    SPDLOG_ERROR("{} generated error {}. See https://registry.khronos.org/OpenGL-Refpages/gl4/html/{}.xhtml", #method, #error, \
                 method);                                                                                                      \
    break;

#undef UINTA_glGetError
#define UINTA_glGetError(method)                                              \
  {                                                                           \
    auto error = glGetError();                                                \
    if (error) {                                                              \
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
      if (UINTA_GL_FLAGS & UINTA_GL_EXIT_ON_ERROR) {                          \
        std::string message = #method;                                        \
        message += " error: " + getGlEnumName(error);                         \
        throw std::runtime_error(message);                                    \
      }                                                                       \
    }                                                                         \
  }

}  // namespace uinta

#endif  // UINTA_DEBUG

#endif  // UINTA_GL_ERRORS_HPP
