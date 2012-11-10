#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef __APPLE__
#define glGenVertexArrays( X, Y ) glGenVertexArraysAPPLE( X, Y )
#endif

#endif
