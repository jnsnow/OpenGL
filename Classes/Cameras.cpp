#include <vector>
#include "Camera.hpp"
#include "Cameras.hpp"
using std::vector;

Cameras::Cameras( void ) {
  /* Nothing, one supposes? 
     Hello, though!
     If you are reading this, I love you. */
}

Cameras::Cameras( const size_t &numCameras ) {
  for ( size_t i = 0; i < numCameras; ++i ) {
    addCamera( Camera() );
  }
}

Cameras::~Cameras( void ) {
  /* Nothing special here, either! */
}
     
size_t Cameras::addCamera( void ) {
  return this->addCamera( Camera() );
}

size_t Cameras::addCamera( Camera const &newCamera ) {
  this->camList.push_back( newCamera );
  return (this->camList.size()) - 1;
}

void Cameras::delCamera( size_t n ) {
  this->camList.erase(camList.begin()+n);
}

Camera &Cameras::getCamera( size_t n ) {
  return this->camList.at( n );
}

Camera &Cameras::operator[]( size_t n ) {
  return getCamera( n );
}
  
void Cameras::IdleMotion( void ) {
  vector<Camera>::iterator it;
  for (it = camList.begin(); it != camList.end(); ++it) {
    it->Idle();
  }
}

void Cameras::LinkAll( const GLuint &program, 
		       const Camera::glsl_var &which, const string &glslVarName ) {
  vector<Camera>::iterator it;
  for (it = camList.begin(); it != camList.end(); ++it) {
    it->link( program, which, glslVarName );
  }
}

Camera *Cameras::iter( size_t n ) {
  
  vector<Camera>::iterator it;
  if (n < camList.size()) {
    return &camList[n];
  } else {
    return NULL;
  }

}

Camera &Cameras::Active( void ) {
  return camList[activeCamera];
}

Camera &Cameras::Active( size_t n ) {
  if (n < camList.size()) {
    activeCamera = n;
  } 
  return Active();
}
