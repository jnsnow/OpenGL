#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef __APPLE__
#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGRemoteOperation.h>
#define glGenVertexArrays( X, Y ) glGenVertexArraysAPPLE( X, Y )
#define glBindVertexArray( X ) glBindVertexArrayAPPLE( X )
#define GLEW_INIT() ;
#else
#define GLEW_INIT();	      \
  glewExperimental = GL_TRUE; \
  glewInit();
#endif
#endif
