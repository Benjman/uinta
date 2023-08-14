#ifndef UINTA_GL_API_H
#define UINTA_GL_API_H

#ifdef __ANDROID__
#include <GL/gl.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else  // desktop
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#endif

#endif  // UINTA_GL_API_H
