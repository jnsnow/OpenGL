#include <vector>

#include <sys/time.h>
#include <cmath>
#include "platform.h" /* Multi-platform support and OpenGL headers */
#include "vec.hpp"
#include "mat.hpp"
#include "model.hpp"
#include "Camera.hpp"
#include "InitShader.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"

// Turn on debugging if it's been requested of us by the Makefile environment.
#ifndef DEBUG
#define DEBUG false
#else
#undef DEBUG
#define DEBUG true
#endif

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Wii Connectivity 
#ifdef WII
#include <wiicpp.h>  // WiiC++ API
#include "WiiUtil.h" // Wii Controller Handler Util
CWii Wii;
bool usingWii = false;
#endif
////

std::vector<point4> pointVector;

Screen myScreen( 800, 600 );
GLuint gShader;
struct timeval _T1;
struct timeval _T2;

void landGen( std::vector<point4> &vec ) {

  const int _N = 9;
  const int SIZE = pow(2,_N) + 1;

  if (DEBUG) printf( "\nEntering landGen()...\n");
  // the range (-h -> h) for the average offset
  // This determines the jaggedness of the peaks and valleys.
  // A smaller initial value will create smaller peaks and shollow valleys for
  // a look more similar to rolling hill tops.
  double h = 10.0;  
  double magnitude = (h*(2 - pow(2,-(_N))));
  fprintf( stderr, "landGen magnitude: %f\n", magnitude );

  // Create 2D Array of point4 
  point4 **terrainPoints = (point4 **)calloc( SIZE, sizeof(point4 *) );
  for ( int i = 0; i < SIZE; ++i ) terrainPoints[i] = (point4 *)calloc( SIZE, sizeof(point4));
  /*point4 terrainPoints[SIZE][SIZE];*/
   
  // Initialize the corners of the grid
  terrainPoints[0][0]           = point4(   0.0, 0.0,    0.0, 1);
  terrainPoints[SIZE-1][0]      = point4(SIZE-1, 0.0,    0.0, 1);
  terrainPoints[0][SIZE-1]      = point4(   0.0, 0.0, SIZE-1, 1);
  terrainPoints[SIZE-1][SIZE-1] = point4(SIZE-1, 0.0, SIZE-1, 1);
 
  // Populate the (x, y, z) values of vec4 according to the
  // Diamond-Square algorithm
  // sideLength is the distance of a single square side or
  // distance of diagonal in diamond.
  if (DEBUG) printf("\nEntering for( sideLength...) ...\n");
  for (int sideLength = SIZE -1; sideLength >= 2; sideLength /= 2, h /= 2.0){
 
    int halfSide = sideLength / 2;
 
    // generate new square values
    for ( int x = 0 ; x < SIZE-1 ; x += sideLength ) {
      for ( int z = 0 ; z < SIZE-1 ; z += sideLength) {
 
        // x,z is uper left corner of square
        // calculate average of existing corners
        double avg = terrainPoints[x][z].y +           // top left
          terrainPoints[x+sideLength][z].y +           // top right
          terrainPoints[x][z+sideLength].y +           // lower left
          terrainPoints[x+sideLength][z+sideLength].y; // lower right
 
        avg /= 4.0;
 
        // Center is average plus random offset in the range (-h, h)
        double offset = (-h) + rand() * (h - (-h)) / RAND_MAX;
        terrainPoints[x+halfSide][z+halfSide] = point4( x, avg + offset, z, 1 );
 
      } // for z
    } // for x
   
    // Generate the diamond values
    // Since diamonds are staggered, we only move x by half side
    // NOTE: If the data shouldn't wrap the x < SIZE and y < SIZE
    for ( int x = 0 ; x < SIZE-1 ; x += halfSide ) {
      for (int z = ( x+halfSide ) % sideLength ; z < SIZE-1 ; z += sideLength ) {
 
        // x,z is center of diamond
        // We must use mod and add SIZE for subtraction
        // so that we can wrap around the array to find the corners
 
        double avg =
          terrainPoints[(x-halfSide+SIZE)%SIZE][z].y +    // left of center  
          terrainPoints[(x+halfSide)%SIZE][z].y      +    // right of center
          terrainPoints[x][(z+halfSide)%SIZE].y      +    // below center
          terrainPoints[x][(z-halfSide+SIZE)%SIZE].y;
 
        avg /= 4.0;
 
        // new value = average plus random offset
        // calculate random value in the range (-h, h)
        double offset = (-h) + rand() * (h - (-h)) / RAND_MAX;
        avg += offset;
 
        // update value for center of diamond
        terrainPoints[x][z] = point4( x, avg, z, 1 );
 
        // wrap values on the edges
        // Allows seamless tiling of generated terrain. MOAR LAND!
        if ( x == 0 ) terrainPoints[SIZE-1][z] = point4( SIZE-1, avg, z, 1 );
        if ( z == 0 ) terrainPoints[x][SIZE-1] = point4( x, avg, SIZE-1, 1 );
      } // for z
    } // for x
  } // for sideLength

  for ( int i = 0; i < SIZE; ++i ) {
    for ( int j = 0; j < SIZE; ++j ) {
      terrainPoints[i][j].z = i;
      terrainPoints[i][j].x = j;
    }
  }


  vec.reserve(2*SIZE*SIZE - 2*SIZE);
  // Convert 2D array into 1D array to pass to shaders
  // Size of 1D array is 2*SIZE^2 - 2SIZE for
  // the sake of proper parsing by glTriangleStrips
  int S = SIZE;
  int i, j;
  //  point4 points[S];
  for ( i = 0 ; i + 1 < S ; i++ ) {
    for ( j = 0 ; j < S ; j++ ) {
      if (0) fprintf( stderr, "A(%d,%d) (X:%f,Y:%f,Z:%f)\n", i, j, 
	       terrainPoints[i][j].x, terrainPoints[i][j].y, terrainPoints[i][j].z );
      vec.push_back( terrainPoints[i][j] );
      if (0) fprintf( stderr, "A(%d,%d) (X:%f,Y:%f,Z:%f)\n", i+1, j,
	       terrainPoints[i+1][j].x, terrainPoints[i+1][j].y, terrainPoints[i+1][j].z );
      vec.push_back( terrainPoints[i+1][j] );
    }

    if (++i == S) break;

    for ( ; j > 0 ; j-- ){
      if (0) fprintf( stderr, "B(%d,%d) (X:%f,Y:%f,Z:%f)\n", i, j-1,
	       terrainPoints[i][j-1].x, terrainPoints[i][j-1].y, terrainPoints[i][j-1].z );
      vec.push_back( terrainPoints[i][j-1] );
      if (0) fprintf( stderr, "B(%d,%d) (X:%f,Y:%f,Z:%f)\n", i+1, j-1,
	       terrainPoints[i+1][j-1].x, terrainPoints[i+1][j-1].y, terrainPoints[i+1][j-1].z );
      vec.push_back( terrainPoints[i+1][j-1] );
    }
  }
  if (DEBUG) printf("\nExiting landGen()...\n");
  return;
}



// this file and its shaders have NO lighting, for sake of simplicity and debugging for implementing the terrain gen.



void cameraInit( Camera& cam ) {

  /* Link this camera to our standard shader variables. */
  cam.link( gShader, Camera::TRANSLATION, "T" );
  cam.link( gShader, Camera::ROTATION, "R" );
  cam.link( gShader, Camera::VIEW, "P" );
  cam.link( gShader, Camera::CTM, "CTM" );

}

void init() {

  /** Fill points[...] with terrain map **/
  landGen( pointVector );
  
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  
  // First, we create an empty buffer of the size we need
  glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * pointVector.size(), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * pointVector.size(), &(pointVector[0]) );
  
  // Load shaders and use the resulting shader program
  gShader = Angel::InitShader( "vterrain.glsl", "fterrain.glsl" );
  glUseProgram( gShader );

  // Initialize the vertex position attribute from the vertex shader  
  GLuint vPosition = glGetAttribLocation( gShader, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			 (GLvoid*)0 );
  
  // Link however many cameras we have at this point to the shader.
  myScreen.camList.LinkAll( gShader, Camera::TRANSLATION, "T" );
  myScreen.camList.LinkAll( gShader, Camera::ROTATION, "R" );
  myScreen.camList.LinkAll( gShader, Camera::VIEW, "P" );
  myScreen.camList.LinkAll( gShader, Camera::CTM, "CTM" );

  gettimeofday( &_T1, NULL );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.06, 0.06, 0.06, 1.0 );
  //  glClearColor( 0.6, 0.6, 0.6, 1.0 );
  
}

//--------------------------------------------------------------------

/** A function that takes no arguments.
    Is responsible for drawing a SINGLE VIEWPORT. **/
void displayViewport( void ) {
  
  //glDrawArrays( GL_LINE_LOOP, 0, pointVector.size() );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, pointVector.size() );
}

void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our displayViewport rendering function.
  myScreen.camList.Draw( displayViewport );
  glutSwapBuffers();
}

void keylift( unsigned char key, int x, int y ) {
  
  Camera &cam = myScreen.camList.Active();

  switch( key ) {
  case 'w':
    cam.Stop( Camera::Forward );
    break;
  case 's':
    cam.Stop( Camera::Backward );
    break;
  case 'a':
    cam.Stop( Camera::Left );
    break;
  case 'd':
    cam.Stop( Camera::Right );
    break;
  case 'q':
    cam.Stop( Camera::Up );
    break;
  case 'e':
    cam.Stop( Camera::Down );
    break;
  }
}

void keyboard( unsigned char key, int x, int y ) {

  /* A shorthand variable with local scope that refers to "The Active Camera." */
  Camera &cam = myScreen.camList.Active();

  switch( key ) {

  case 033: // Escape Key	  
    exit( EXIT_SUCCESS );
    break;
    
  case '+':
    cameraInit(myScreen.camList[myScreen.camList.addCamera()]);
    break;
  case '-':
    myScreen.camList.popCamera();
    break;
    
  case 'w':
    cam.Move( Camera::Forward );
    break;
  case 's':
    cam.Move( Camera::Backward );
    break;
  case 'a':
    cam.Move( Camera::Left );
    break;
  case 'd':
    cam.Move( Camera::Right );
    break;
  case 'q':
    cam.Move( Camera::Up );
    break;
  case 'e':
    cam.Move( Camera::Down );
    break;
    
  case 'p': // Print Info
    fprintf( stderr, "POS: (%f,%f,%f)\n", cam.X(), cam.Y(), cam.Z() );
    break;
    
    //Perspectives
  case 'z': cam.changePerspective( Camera::PERSPECTIVE ); break;
  case 'x': cam.changePerspective( Camera::ORTHO ); break;
  case 'c': cam.changePerspective( Camera::ORTHO2D ); break;
  case 'v': cam.changePerspective( Camera::FRUSTUM ); break;
  case 'b': cam.changePerspective( Camera::IDENTITY ); break;
  }
}

void keyboard_ctrl( int key, int x, int y ) {
  switch (key) {
  case GLUT_KEY_PAGE_UP:
    myScreen.camList.Prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    myScreen.camList.Next();
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: myScreen.camList.Active().dFOV( 1 ); break;
    case 4: myScreen.camList.Active().dFOV( -1 ); break;
    }
  }

}


void mouseroll( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    myScreen.camList.Active().roll( x - myScreen.MidpointX() );
    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }

}


void mouselook( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    const double dx = ((double)x - myScreen.MidpointX());
    const double dy = ((double)y - myScreen.MidpointY());
    
    myScreen.camList.Active().pitch( dy );
    myScreen.camList.Active().yaw( dx, true ); // Fixed Yaw
    
    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }
  
}


void resizeEvent( int width, int height ) {

  /* Handles resizing the child cameras as well. */
  myScreen.Size( width, height );
  glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );

}


void idle( void ) {

#ifdef WII
  if (usingWii) {
    for (size_t i = 0; i < 20; ++i) {
      pollWii( Wii );
      myScreen.camList.Active().Accel( bb_magnitudes );
    }
  }
#endif

  // Move all camera(s).
  myScreen.camList.IdleMotion();
  glutPostRedisplay();
    
}


//--------------------------------------------------------------------

int main( int argc, char **argv ) {

#ifdef WII
  if (!(usingWii = initWii( Wii ))) {
    std::cerr << "Not using Wii controls for this runthrough.\n";
  }
#endif

  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.Width(), myScreen.Height() );
  glutCreateWindow( "Gasket Flythrough" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );

  GLEW_INIT();
  init();

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutKeyboardUpFunc( keylift );
  glutSpecialFunc( keyboard_ctrl );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseroll );
  glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  if (DEBUG) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ));
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ));
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ));
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n", 
	     glGetString( GL_SHADING_LANGUAGE_VERSION ));
  }

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
