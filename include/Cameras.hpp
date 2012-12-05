#ifndef __CAMERAS_HPP
#define __CAMERAS_HPP

#include <vector>
#include "Camera.hpp"
using std::vector;

/**
   @author John Huston, jhuston@cs.uml.edu
   @since 28 Nov 2012

   @brief The Cameras class represents a group of logical cameras
   for a model view. Each camera posesses its own current viewing
   angle, and an absolute position in space.

   Each Camera posesses its own CTM which can be resent to the GPU at will.

**/

class Cameras {

public:

  Cameras( void );
  Cameras( const size_t &numCameras = 1 );
  ~Cameras( void );

  size_t addCamera( void );
  size_t addCamera( Camera const &newCamera );
  void delCamera( size_t n );
  void popCamera( void );

  Camera &getCamera( size_t n );
  Camera &operator[]( size_t n );

  Camera *iter( size_t n );

  void IdleMotion( void );
  void LinkAll( const GLuint &program, 
		const Camera::glsl_var &which, const string &glslVarName ); 

  size_t ActiveN( void );
  Camera &Active( void );
  Camera &Active( size_t n );

  void Resize( int width, int height );
  void CalculateViewports( void );
  void Draw(void (*draw_func)(void));

private:
  
  void commonInit( void );
  vector< Camera > camList;
  size_t activeCamera;
  size_t Width;
  size_t Height;

};

#endif


#define CAMERAS_MSG( LST, MSG )						\
  {									\
    Camera *__PTR = NULL;						\
    for (size_t __N = 0; (__PTR = LST.iter(__N)) != NULL; ++__N) {	\
      (__PTR)->MSG;							\
    }									\
  }
