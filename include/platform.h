#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef __APPLE__
/* All Hail Steve Jobs */
#include <Carbon/Carbon.h> // will need to define some new functions.


#define glGenVertexArrays( X, Y ) glGenVertexArraysAPPLE( X, Y )
#define glBindVertexArray( X ) glBindVertexArrayAPPLE( X )
#define GLEW_INIT() ;
#else
/* Peasant Code */
#define GLEW_INIT() \
  glewExperimental = GL_TRUE; \
  glewInit();
#endif
#endif
