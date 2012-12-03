#include <cmath>
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

size_t Cameras::ActiveN( void ) {
  return activeCamera;
}

Camera &Cameras::Active( void ) {
  return camList[activeCamera];
}

Camera &Cameras::Active( size_t n ) {
  if (n < camList.size()) {
    activeCamera = n;
  } 
  camList[activeCamera].send( Camera::CTM );
  return Active();
}

void Cameras::Draw(void (*draw_func)(void)) {

  vector<Camera>::iterator it;
  for (it = camList.begin();
       it != camList.end();
       ++it) {
    it->Draw();
    (*draw_func)();
  }
}

void Cameras::Resize( int width, int height ) {
  
  // How many cameras do we have?
  size_t numCameras = camList.size();

  // How many virtual rows will we need to display this many?
  size_t numRows = ceil(sqrt(numCameras));

  // How many columns will we need (average) in general to display this many?
  double numCols = (double)numCameras/(double)numRows;
  size_t numMaxCols = ceil(numCols);
  size_t numMinCols = floor(numCols);

  // How many rows do we need to draw with MinCols?
  size_t drawMinRows = (numCameras % numRows) ?
    ((numCameras - (numRows * numMaxCols)) / (numMinCols - numMaxCols)) :
    0;

  vector<Camera>::iterator it = camList.begin();
  size_t row = 0;

  for (size_t allocHeight = 0; row < numRows; ++row) {
    size_t myWidth;
    size_t myHeight;
    size_t allocWidth = 0;
    size_t colsThisRow = (row < drawMinRows) ? numMinCols : numMaxCols;

    for (size_t col = 0; col < colsThisRow; ++col, ++it) {
      // Is this the last column? Use the remaining width.
      if (col + 1 == colsThisRow) myWidth = width - allocWidth;
      else myWidth = width/colsThisRow;
      // Is this the last row? Use the remaining height.
      if (row + 1 == numRows) myHeight = height - allocHeight;
      else myHeight = height/numRows;
      // Tell this camera his new viewport.
      it->viewport( allocWidth, allocHeight, myWidth, myHeight );
	
      // Increment our allocated width counter.
      allocWidth += myWidth;
    }

    // Increment our allocated height counter.
    allocHeight += (height/numRows);
  }

}
