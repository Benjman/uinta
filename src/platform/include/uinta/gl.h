#ifndef SRC_PLATFORM_INCLUDE_UINTA_GL_H_
#define SRC_PLATFORM_INCLUDE_UINTA_GL_H_

#ifdef __ANDROID__
#include <GL/gl.h>
#elif __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else  // desktop
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif

#endif  // SRC_PLATFORM_INCLUDE_UINTA_GL_H_
