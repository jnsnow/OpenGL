#define X_SIZE (1024)
#define Y_SIZE (600)

#include "platform.h"
#include "Angel.h"
#include "model.h"
#include "Camera.hpp"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

/**** PYRAMID ****/
#ifndef __CUBE__
const int NumTimesToSubdivide = 4;
const int NumTetrahedrons = 4*4*4*4;
const int NumTriangles = 4*NumTetrahedrons;
const int NumVertices = 3*NumTriangles + 6;
#endif
#ifdef __CUBE__
/**** CUBE ****/
const int NumVertices = 36;
#endif
point4 points[NumVertices];
color4 colors[NumVertices];

Camera theCamera;

int Width = X_SIZE;
int X_Center = (Width/2);
int Height = Y_SIZE;
int Y_Center = (Height/2);

//--------------------------------------------------------------------
// OpenGL initialization
void init() {

#ifndef __CUBE__
  divide_tetra( 
	       vec4(  0,  1,  0, 1 ),
	       vec4( -1, -0.999, -1, 1 ),
	       vec4(  1, -0.999, -1, 1 ),
	       vec4(  0, -0.999,  1, 1 ),
	       NumTimesToSubdivide );
  
  // Draw a "floor" or something for reference.
  quad(       
       vec4( -1, -1, -1, 1.0 ),
       vec4( -1, -1, 1, 1.0 ),
       vec4( 1, -1, 1, 1.0 ),
       vec4( 1, -1, -1, 1.0 ),
       vec4( 0, 1, 0, 1 ),
       vec4( 0, 1, 0, 1 ),
       vec4( 0, 1, 0, 1 ),
       vec4( 0, 1, 0, 1 ) );
#endif

#ifdef __CUBE__
  colorcube(GLfloat(0.5));
#endif

  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  
  // First, we create an empty buffer of the size we need by passing
  //   a NULL pointer for the data values
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW );
  
  // Next, we load the real data in parts.  We need to specify the
  //   correct byte offset for placing the color data after the point
  //   data in the buffer.  Conveniently, the byte offset we need is
  //   the same as the size (in bytes) of the points array, which is
  //   returned from "sizeof(points)".
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
  
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );
  
  // Initialize the vertex position attribute from the vertex shader  
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(0) );
  
  // Likewise, initialize the vertex color attribute.  Once again, we
  //    need to specify the starting offset (in bytes) for the color
  //    data.  Just like loading the array, we use "sizeof(points)"
  //    to determine the correct value.
  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(sizeof(points)) );

  theCamera.link( program, Camera::TRANSLATION, "T" );
  theCamera.link( program, Camera::ROTATION, "R" );
  theCamera.link( program, Camera::PERSPECTIVE, "P" );
  theCamera.link( program, Camera::PRT_M, "PRT" );
  theCamera.FOV( 45.0 ); /* Must be set /after/ linking perspective ... ! */

  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 );
  
}

//--------------------------------------------------------------------

void display( void ) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  //theCamera.send( Camera::TRANSLATION );
  //theCamera.send( Camera::ROTATION );

  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
  glutSwapBuffers();

}

void keylift( unsigned char key, int x, int y ) {
  switch( key ) {
  case 'w':
    theCamera.Stop( Camera::Forward );
    break;
  case 'W':
    theCamera.surge( 5 );
    break;
  case 's':
    theCamera.Stop( Camera::Backward );
    break;
  case 'a':
    theCamera.Stop( Camera::Left );
    break;
  case 'd':
    theCamera.Stop( Camera::Right );
    break;
  case 'q':
    theCamera.Stop( Camera::Up );
    break;
  case 'e':
    theCamera.Stop( Camera::Down );
    break;
  }
}

void keyboard( unsigned char key, int x, int y ) {
  switch( key ) {
  case 033: // Escape Key	  
    exit( EXIT_SUCCESS );
    break;
    
  case 'w':
    theCamera.Move( Camera::Forward );
    break;
  case 's':
    theCamera.Move( Camera::Backward );
    break;
  case 'a':
    theCamera.Move( Camera::Left );
    break;
  case 'd':
    theCamera.Move( Camera::Right );
    break;
  case 'q':
    theCamera.Move( Camera::Up );
    break;
  case 'e':
    theCamera.Move( Camera::Down );
    break;

  case 'l':
    theCamera.yaw(1);
    break;
  case 'L':
    theCamera.yaw(45);
    break;
  case 'j':
    theCamera.yaw(-1);
    break;
  case 'J':
    theCamera.yaw(-45);
    break;
    
  case 'p': // Print Info
    fprintf( stderr, "POS: (%f,%f,%f)\n",
	     theCamera.X(), theCamera.Y(),
	     theCamera.Z() );
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: theCamera.dFOV( 1 ); break;
    case 4: theCamera.dFOV( -1 ); break;
    }
  }

}

void mouseroll( int x, int y ) {

  if ((x != X_Center) || (y != Y_Center)) {
    theCamera.roll( x - X_Center );
    glutWarpPointer( X_Center, Y_Center );
  }

}

void mouselook( int x, int y ) {

  if ( x != X_Center || y != Y_Center ) {
    const double dx = ((double)x - X_Center);
    const double dy = ((double)y - Y_Center);
    theCamera.pitch( dy );
    theCamera.yaw( dx );
    glutWarpPointer( X_Center, Y_Center );
  }

}

void resizeEvent( int width, int height ) {
  
  Height = height;
  Width = width;
  X_Center = (Width/2);
  Y_Center = (Height/2);

}
 
void idle( void ) {

  theCamera.Idle();
  glutPostRedisplay();
  
}

//--------------------------------------------------------------------

int main( int argc, char **argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( X_SIZE, Y_SIZE );
    glutCreateWindow( "Gasket Flythrough" );
    glutSetCursor( GLUT_CURSOR_NONE );
    glutWarpPointer( X_Center, Y_Center );

    GLEW_INIT();
    init();

    /* Plugins and shit oh yeah */
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutKeyboardUpFunc( keylift );
    glutMouseFunc( mouse );
    glutMotionFunc( mouseroll );
    glutPassiveMotionFunc( mouselook );
    glutIdleFunc( idle );
    glutReshapeFunc( resizeEvent );

    /* PULL THE TRIGGER */
    glutMainLoop();
    return 0;
}
