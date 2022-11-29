#ifndef UINTA_GL_FLAGS_H
#define UINTA_GL_FLAGS_H

#include <uinta/utils/flags.h>

static const flag_t UINTA_GL_EXIT_ON_ERROR = 1 << 0;
static flags_t UINTA_GL_FLAGS = UINTA_GL_EXIT_ON_ERROR;

#endif  // UINTA_GL_FLAGS_H
