//#include "OpenGL.h" /* For window size calculations */
#include <cmath>
#include <vector>
#include "Camera.hpp"
#include "Cameras.hpp"
using std::vector;


Cameras::Cameras( void ) {
  //commonInit();
  /* Nothing, one supposes? 
     Hello, though!
     If you are reading this, I love you. */
}

Cameras::Cameras( const size_t &numCameras ) {
  //commonInit();
  for ( size_t i = 0; i < numCameras; ++i ) {
    addCamera( Camera() );
  }
}

/*
void Cameras::commonInit( void ) {
  GLint size[4];
  glGetIntegerv( GL_VIEWPORT, size );
  Resize( size[2], size[3] );
}
*/

Cameras::~Cameras( void ) {
  /* Nothing special here, either! */
}
     
size_t Cameras::addCamera( void ) {
  return this->addCamera( Camera() );
}

size_t Cameras::addCamera( Camera const &newCamera ) {
  // Add the new camera.
  this->camList.push_back( newCamera );
  //Recalculate our splitscreen viewports.
  CalculateViewports();
  return (this->camList.size()) - 1;
}

void Cameras::delCamera( size_t n ) {
  this->camList.erase(camList.begin()+n);
  //Recalculate our splitscreen viewports.
  CalculateViewports();
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
  
  this->Width = width;
  this->Height = height;
  std::cerr << "Setting Cameras WxH: " << Width << "x" << Height << "\n";
  CalculateViewports();

}

void Cameras::CalculateViewports( void ) {

  /* Let's not try to distribute Viewports if the window system
     hasn't been initialized yet, i.e, if Resize() has not yet
     been called. */
  if (!Height || !Width) return;
  
  // How many cameras do we have?
  size_t numCameras = camList.size();
  // Let's not try to resize zero cameras.
  if (numCameras == 0) return;

  // How many virtual rows will we need to display this many?
  size_t numRows = ceil(sqrt(numCameras));

  // How many columns will we need (average) in general to display this many?
  double numCols = (double)numCameras/(double)numRows;
  size_t numMaxCols = ceil(numCols);
  size_t numMinCols = floor(numCols);

  // How many rows do we need to draw with MinCols?
  // (By extension: drawMaxRows = (numRows-drawMinRows))
  size_t drawMinRows = (numCameras % numRows) ?
    ((int)(numCameras - (numRows * numMaxCols)) / (int)(numMinCols - numMaxCols)) :
    0;

  vector<Camera>::iterator it = camList.begin();
  /* This is confusing as hell. Good luck! */
  for (size_t allocHeight = 0, row = 0;
       (row < numRows) || (it != camList.end()); //Terminate on either cond. Just in case.
       ++row) {
    size_t myWidth;
    size_t myHeight;
    size_t allocWidth = 0;
    size_t colsThisRow = (row < drawMinRows) ? numMinCols : numMaxCols;
    fprintf( stderr, "Row: %lu; Columns this row: %lu\n", row, colsThisRow );

    for (size_t col = 0; col < colsThisRow; ++col, ++it) {
      // Is this the last column? Use the remaining width.
      if (col + 1 == colsThisRow) myWidth = (this->Width) - allocWidth;
      else myWidth = (this->Width)/colsThisRow;
      // Is this the last row? Use the remaining height.
      if (row + 1 == numRows) myHeight = (this->Height) - allocHeight;
      else myHeight = (this->Height)/numRows;

      // Tell this camera his new viewport.
      // height looks a little goofy because we are allocating height
      // from the top of the coordinate system and working down,
      // so we have to take the complement.
      it->viewport( allocWidth, ((this->Height)-(allocHeight+myHeight)), myWidth, myHeight );
      if (0) fprintf( stderr, "Camera: (%lu x %lu) @ (%lu,%lu)\n",
		      myWidth, myHeight,
		      allocWidth, ((this->Height)-(allocHeight+myHeight)));

      // Increment our allocated width counter.
      allocWidth += myWidth;
    }
    // Increment our allocated height counter.
    allocHeight += ((this->Height)/numRows);
  }
}
