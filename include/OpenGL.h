#ifndef __OPENGL_H
#define __OPENGL_H

#ifdef __APPLE__  // include Mac OS X verions of headers                   
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems                                     
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__   

#endif
