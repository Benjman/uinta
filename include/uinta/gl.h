#ifndef UINTA_GL_H
#define UINTA_GL_H

#ifdef __APPLE__
#define GL_SILENCe_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__

#endif // UINTA_GL_H
