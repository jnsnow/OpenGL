/**
   @file terrain.cpp
   @authors John Huston, Nicholas StPierre, Chris Compton
   @date 2012-12-06
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for terrain generation using an udpated
   engine derived from fly.cpp, which was mostly based on Ed Angel's code
   from his book.
**/

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
#include "Object.hpp"
#include "Timer.hpp"

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

Screen myScreen( 800, 600 );
GLuint gShader;
// Must create a POINTER, because if we try to initialize it before OpenGL, W.W.III.
Object *terrain;

float rand_float( void ) {
  return rand() / (float)RAND_MAX;
}
double jitter( double H ) {
  return (-H) + rand() * (H - (-H)) / RAND_MAX;
}


void landGen( std::vector<point4> &vec, std::vector<unsigned int> &drawIndex ) {

  Timer Tick;

  const int _N = 5;
  const int S = pow(2,_N) + 1;

  if (DEBUG) printf( "\nEntering landGen()...\n");
  // the range (-h -> h) for the average offset
  // This determines the jaggedness of the peaks and valleys.
  // A smaller initial value will create smaller peaks and shollow valleys for
  // a look more similar to rolling hill tops.
  double h = 5.0;
  double CH = 0.5;
  double magnitude = (h*(2 - pow(2,-(_N))));
  fprintf( stderr, "landGen theoretical magnitude: %f\n", magnitude );

  /* Initialize all points in the vector to have their X,Z (and w) coordinates. */
  if (vec.size()) vec.clear();
  vec.reserve( S * S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j ) 
      vec.push_back( vec4( j, 0, i, 1 ) );

  /* Initialize our color vectors. */
  std::vector< vec4 > &col = terrain->colors;
  if (col.size()) col.clear();
  col.reserve( S*S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j )
      col.push_back( vec4( 0.5, 0.5, 0.5, 1.0 ) );

  /* Simulate a 2D array in this 1D array. Use these Macros to help. */
#define OffsetAt(X,Z) ((X)*S+(Z))
#define VertexAt(X,Z) (vec.at(OffsetAt(X,Z)))
#define HeightAt(X,Z) (VertexAt(X,Z).y)
#define ColorAt(X,Z) (col.at(OffsetAt(X,Z)))

  // Initialize the corners of the grid
  HeightAt( 0, 0 )     = 0;
  HeightAt( S-1, 0 )   = 0;
  HeightAt( 0, S-1 )   = 0;
  HeightAt( S-1, S-1 ) = 0;

  // Populate the (x, y, z) values of vec4 according to the
  // Diamond-Square algorithm
  // sideLength is the distance of a single square side or
  // distance of diagonal in diamond.
  if (DEBUG) printf("\nEntering for( sideLength...) ...\n");
  for (int sideLength = S-1; sideLength >= 2; sideLength /= 2, h /= 2.0, CH /= 2.0) { 
    int halfSide = sideLength / 2;
    // generate new square values
    for ( int x = 0 ; x < S-1 ; x += sideLength ) {
      for ( int z = 0 ; z < S-1 ; z += sideLength) { 
	double avg =						\
	  (HeightAt( x, z ) + HeightAt( x + sideLength, z ) +
	   HeightAt( x, z + sideLength ) + HeightAt( x + sideLength, z + sideLength ))/4.0;
	
	vec4 color_avg =				\
	  (ColorAt(x,z) + ColorAt(x+sideLength,z) +
	   ColorAt(x,z+sideLength) + ColorAt(x+sideLength,z+sideLength))/4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );
	
	HeightAt( x + halfSide, z + halfSide ) = avg + jitter(h);
	ColorAt( x + halfSide, z + halfSide ) = color_avg + color_jitter;
      } // for z
    } // for x
   
    // Generate the diamond values
    // Since diamonds are staggered, we only move x by half side
    // NOTE: If the data shouldn't wrap the x < SIZE and y < SIZE
    for ( int x = 0 ; x < S-1 ; x += halfSide ) {
      for (int z = ( x+halfSide ) % sideLength ; z < S-1 ; z += sideLength ) {
 
        // x,z is center of diamond
        // We must use mod and add SIZE for subtraction
        // so that we can wrap around the array to find the corners

	double avg = 
	  (HeightAt( (x-halfSide + S) % S, z ) +
	   HeightAt( x + halfSide % S, z ) +
	   HeightAt( x, z + halfSide % S ) +
	   HeightAt( x, (z - halfSide + S) % S )) / 4.0;

	vec4 color_avg = 
	  (ColorAt( (x-halfSide + S) % S, z ) +
	   ColorAt( x + halfSide % S, z ) +
	   ColorAt( x, z + halfSide % S ) +
	   ColorAt( x, (z - halfSide + S) % S )) / 4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );
 
	HeightAt( x, z ) = avg + jitter(h);
	ColorAt( x, z ) = color_avg + color_jitter;

	// Wrapping:
	if (x == 0) {
	  HeightAt( S-1, z ) = HeightAt( x, z );
	  ColorAt( S-1, z ) = ColorAt( x, z );
	}
	if (z == 0) {
	  HeightAt( x, S-1 ) = HeightAt( x, z );
	  ColorAt( x, S-1 ) = ColorAt( x, z );
	}
      } // for z
    } // for x
  } // for sideLength


  //vec.reserve(2*S*S - 2*S);
  // Convert 2D array into 1D array to pass to shaders
  // Size of 1D array is 2*SIZE^2 - 2SIZE for
  // the sake of proper parsing by glTriangleStrips
  for ( int i = 0, j = 0 ; i + 1 < S ; i++ ) {
    for ( j = 0 ; j < S ; j++ ) {
      drawIndex.push_back(OffsetAt(i,j));
      drawIndex.push_back(OffsetAt(i+1,j));
    }

    /* If we're out of rows to serialize, give up. */
    if (++i == S) break;

    for ( ; j > 0 ; j-- ){
      drawIndex.push_back(OffsetAt(i,j-1));
      drawIndex.push_back(OffsetAt(i+1,j-1));
    }
  }

  for ( int i = 0; i < S*S; i++ ) {
    terrain->colors.push_back(vec4(rand_float(),rand_float(),rand_float(),1.0));
  }
				    
  if (DEBUG) printf("\nExiting landGen()...\n");

  Tick.Tock();
  fprintf( stderr, "Landgen took %lu usec, %f msec, %f sec to generate %d vertices.\n", 
	   Tick.Delta(), Tick.Delta()/1000.0, Tick.Delta()/1000000.0, S*S );

  return;
}







void cameraInit( Camera& cam ) {

  /* Link this camera to our standard shader variables. */
  cam.link( gShader, Camera::TRANSLATION, "T" );
  cam.link( gShader, Camera::ROTATION, "R" );
  cam.link( gShader, Camera::VIEW, "P" );
  cam.link( gShader, Camera::CTM, "CTM" );

}

void init() {

  // Load shaders and use the resulting shader program
  gShader = Angel::InitShader( "vterrain.glsl", "fterrain.glsl" );
  terrain = new Object( gShader );
  glUseProgram( gShader );

  /** Fill points[...] with terrain map **/
  landGen( terrain->points, terrain->indices );
  terrain->Buffer();
    
  // Link however many cameras we have at this point to the shader.
  myScreen.camList.LinkAll( gShader, Camera::TRANSLATION, "T" );
  myScreen.camList.LinkAll( gShader, Camera::ROTATION, "R" );
  myScreen.camList.LinkAll( gShader, Camera::VIEW, "P" );
  myScreen.camList.LinkAll( gShader, Camera::CTM, "CTM" );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.6, 0.6, 0.6, 1.0 );
  
}

//--------------------------------------------------------------------

/** A function that takes no arguments.
    Is responsible for drawing a SINGLE VIEWPORT. **/
void displayViewport( void ) {  
  terrain->Draw();
  //glDrawArrays( draw_mode, 0, pointVector.size() );
  //glDrawElements( draw_mode, pointIndices.size(), GL_UNSIGNED_INT, 0 );
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

  case GLUT_KEY_F1:
    terrain->Mode( GL_POINTS );
    break;
  case GLUT_KEY_F2:
    terrain->Mode( GL_LINE_STRIP );
    break;
  case GLUT_KEY_F3:
    terrain->Mode( GL_TRIANGLE_STRIP );
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

  Tick.Tock();
  //fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );

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
