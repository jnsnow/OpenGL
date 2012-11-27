#define X_SIZE (1024)
#define Y_SIZE (600)
 
#include "platform.h"
#include "Angel.h"
#include "model.hpp"
#include "Camera.hpp"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

/*
extern void divide_tetra_alt( const point4& a, const point4& b,
			      const point4& c, const point4& d, int count );
*/
/*
 * Takes a base and an exponent and returns the appropriate exponent.
 * If you pass a negative number, one will be returned.
 * We need this because the pow in math.h returns a floating point number.
 * edit: actually this is useless neverind.


int intPow( int base, int exponent ) {
  int res = 1 ;
  for ( ; exponent > 0 ; --exponent ){    res *= base;  }
  return res;
}
*/

//#define __CUBE__ 1

/**** PYRAMID ****/
#ifndef __CUBE__
const int NumTimesToSubdivide = 4;
const int NumTetrahedrons = 4*4*4*4;//intPow(4, NumTimesToSubdivide) ;
const int NumTriangles = 4*NumTetrahedrons;
const int LIGHT_DUMMY = 0 ;
const int NumVertices = 3*NumTriangles+6+LIGHT_DUMMY ;
#endif
#ifdef __CUBE__
/**** CUBE ****/
const int NumVertices = 36;
#endif

point4 points[NumVertices];
color4 colors[NumVertices];
vec3  normals[NumVertices];

Camera theCamera;

int Width = X_SIZE;
int X_Center = (Width/2);
int Height = Y_SIZE;
int Y_Center = (Height/2);

//--------------------------------------------------------------------
// OpenGL initialization


// Moving light source
point4 light_position( 0.0, 1.0, 0.0, 1.0 );

// Fixed light source
point4 light_position2( 0.0, 1.0, 0.1, 1.0 );


// 11/26 changes: mode of the static light source.
int lightMode = 0;


//glGetUniformLocation(program, "LightPosition");
// this one corresponds to the moving light source
GLuint gl_light_position ; // = glGetUniformLocation(program, "LightPosition");
GLuint gl_AmbientProduct ;
GLuint gl_DiffuseProduct ;
GLuint gl_SpecularProduct ;

GLuint gl_AmbientProduct2 ;
GLuint gl_DiffuseProduct2 ;
GLuint gl_SpecularProduct2 ;


// Initialize shader lighting parameters
// This will eventually be replaced by a call to the Lights class init function:
//   the lights class is still being written.
void init_lights( GLuint program ) {


    color4 light_ambient( 0.12, 0.12, 0.12, 1.0 );
    color4 light_diffuse( 0.4, 0.4, 0.4, 1.0 );
    color4 light_specular( 0.4, 0.4, 0.4, 1.0 );


    // fun fact, we don't use any of this! (yet??????)
    color4 material_ambient( 0.6, 0.6, 0.6, 1.0 ); //1.0, 0.0, 1.0, 1.0
    color4 material_diffuse( 0.5, 0.5, 0.5, 1.0 );
    color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
    float  material_shininess = 16.0; // 100.0

    /*     original code. not that great. can probably abstract this out.

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;
    */

    color4 ambient_product = light_ambient;
    color4 diffuse_product = light_diffuse;
    color4 specular_product = light_specular;

    gl_AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
    gl_DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
    gl_SpecularProduct = glGetUniformLocation(program, "SpecularProduct") ;

    gl_AmbientProduct2 = glGetUniformLocation(program, "AmbientProduct2");
    gl_DiffuseProduct2 = glGetUniformLocation(program, "DiffuseProduct2");
    gl_SpecularProduct2 = glGetUniformLocation(program, "SpecularProduct2") ;

    gl_light_position = glGetUniformLocation(program, "LightPosition"); // sets a global variable
    // no matching line for the other source because it is fixed.

    //source 1
    glUniform4fv( gl_AmbientProduct,  1, ambient_product );
    glUniform4fv( gl_DiffuseProduct,  1, diffuse_product );
    glUniform4fv( gl_SpecularProduct, 1, specular_product );

    //source 2
    glUniform4fv( gl_AmbientProduct2,  1, ambient_product );
    glUniform4fv( gl_DiffuseProduct2,  1, diffuse_product );
    glUniform4fv( gl_SpecularProduct2, 1, specular_product );


    glUniform4fv( gl_light_position, 1, light_position );


    // doesn't change in this demo, so lazymode
    glUniform4fv( glGetUniformLocation(program, "LightPosition2"),
		    1, light_position2 );
    
    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );

    glShadeModel(GL_FLAT); // ??? I forget if/why we need this

}


void init() {

#ifndef __CUBE__
  divide_tetra( //  divide_tetra_alt( 
	       vec4(  0,      1,  0, 1 ),
	       vec4( -1, -0.999,  1, 1 ),
	       vec4(  1, -0.999,  1, 1 ),
	       vec4(  0, -0.999, -1, 1 ),
	       NumTimesToSubdivide );
  
  // Draw a "floor" or something for reference.
  quad(       
       vec4( -1, -1, -1, 1.0 ),
       vec4( -1, -1, 1, 1.0 ),
       vec4( 1, -1, 1, 1.0 ),
       vec4( 1, -1, -1, 1.0 ),
       vec4( 0.8, 0.1, 0.6, 1 ),
       vec4( 0.8, 0.1, 0.6, 1 ),
       vec4( 0.8, 0.1, 0.6, 1 ),
       vec4( 0.8, 0.1, 0.6, 1 ) );


  if (LIGHT_DUMMY) {triangle( light_position, 
	    light_position+point4(0.0,0.1,0.0,0.0),  //point4(0.9, 1.0, 0.9, 1.0), 
	    light_position+point4(0.1,0.0,0.0,0.0),  //point4(0.9, 0.9, 1.0, 1.0), 
	    4 ); // Add a dumb triangle here for the lighting.
  }


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
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals),
		NULL, GL_STATIC_DRAW );
  
  // Next, we load the real data in parts.  We need to specify the
  //   correct byte offset for placing the color data after the point
  //   data in the buffer.  Conveniently, the byte offset we need is
  //   the same as the size (in bytes) of the points array, which is
  //   returned from "sizeof(points)".
  glBufferSubData( GL_ARRAY_BUFFER,                             0, sizeof(points),   points );
  glBufferSubData( GL_ARRAY_BUFFER,                sizeof(points), sizeof(colors),   colors );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), sizeof(normals), normals );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  // More init stuff, but only lighting-related.
  init_lights(program);
  
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

  // Again, initialize another attribute: vNormal.
  GLuint vNormal = glGetAttribLocation( program, "vNormal" );
  glEnableVertexAttribArray( vNormal );
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(sizeof(points)+sizeof(colors)) );



  theCamera.link( program, Camera::TRANSLATION, "T" );
  theCamera.link( program, Camera::ROTATION, "R" );
  theCamera.link( program, Camera::PERSPECTIVE, "P" );
  theCamera.link( program, Camera::PRT_M, "PRT" );
  theCamera.FOV( 45.0 ); /* Must be set /after/ linking perspective ... ! */

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.1, 0.1, 0.1, 1.0 );
  
}

//--------------------------------------------------------------------

// depending on the platform the user is on, these controls may not work. 
// we use the ! operator to toggle between 1 and 0:
// this functionality is defined inside the keyboard callback function.
int red_enable = 1;
int blue_enable = 1;
int green_enable = 1;



void lightEffects(int frameNumber){


    static const float pulseRad = M_PI/60.0 ; // used in the pulse effect.

    /* static for performance reasons! */
    static color4 light_ambient( 0.12, 0.12, 0.12, 1.0 );
    static color4 light_diffuse( 0.8, 0.8, 0.8, 1.0 )   ;
    static color4 light_specular( 0.8, 0.8, 0.8, 1.0 )  ;
    static color4 dark( 0.0, 0.0, 0.0, 1.0);

    color4 ambient_product, diffuse_product, specular_product ;

    float dim = 1.0;

    switch(lightMode){


    case 0:    // on, no real effect

      ambient_product = light_ambient ;
      diffuse_product = light_diffuse ;
      specular_product = light_specular ;

      break;



    case 1:  // strobe/blink


      if( (frameNumber/30) < 3 ){ //      if( (frameNumber/60) < 3 ){

	ambient_product = dark ;
	diffuse_product = dark ;
	specular_product = dark ;

      } else {

      ambient_product = light_ambient ;
      diffuse_product = light_diffuse ;
      specular_product = light_specular ;

      }

      break;


    case 2: // pulse

      dim = sin( pulseRad * frameNumber) ;
      dim *= dim ;  // sin squared for great justice and positivity

      ambient_product  = light_ambient  * dim;
      diffuse_product  = light_diffuse  * dim;
      specular_product = light_specular * dim;

      break;


    case 3: // off

      ambient_product = dark ;
      diffuse_product = dark ;
      specular_product = dark ;

      break;
    }

    glUniform4fv( gl_AmbientProduct,  1, ambient_product );
    glUniform4fv( gl_DiffuseProduct,  1, diffuse_product );
    glUniform4fv( gl_SpecularProduct, 1, specular_product );


}

void display( void ) {


  static int frameNumber = 0;

  lightEffects(frameNumber); // only screws with the non moving light source
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );
  glutSwapBuffers();

  frameNumber = (frameNumber + 1) % 180 ;

}

void keylift( unsigned char key, int x, int y ) {
  switch( key ) {
  case 'w':
    theCamera.Stop( Camera::Forward );
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

// 11/26 changes:
// adding a light source, and some controls.
int lightOrbit = 1 ; // this is a multiplier for the radius of the light source's orbit



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
    
  case 'p': // Print Info
    fprintf( stderr, "POS: (%f,%f,%f)\n",
	     theCamera.X(), theCamera.Y(),
	     theCamera.Z() );
    break;
  case 'n': // Print normals
    //light_position = point4( 0.0, 0.2, 0.0, 0.0 );
    light_position.y = -light_position.y ;
    break;

  case ';':

    for( int l = 0; l < NumVertices ; l+=3 ) {
      fprintf( stderr, "POS: (%f, %f, %f)\n",
	       normals[l].x, normals[l].y, normals[l].z );
    }
    break;

  case '0':
    lightOrbit = 0; break;
  case '1':
    lightOrbit = 1; break;
  case '2':
    lightOrbit = 2; break;
  case '3':
    lightOrbit = 3; break;
  case '4':
    lightOrbit = 4; break;
  case '5':
    lightOrbit = 5; break;
  case '6':
    lightOrbit = 6; break;
  case '7':
    lightOrbit = 7; break;

  // Pressing m cycles through the light modes for the static light source.
  case 'm':  lightMode = (lightMode+1) % 4; break;
    


    // Z, X, C toggle R G B on the light source, respectively.
  case 'z':  red_enable   = !red_enable   ;  break ;
  case 'x':  blue_enable  = !blue_enable  ;  break ;
  case 'c':  green_enable = !green_enable ;  break ;


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



void movelight(void) {

  static float i = 0;
  
  if( i > 360 ) i = 0 ; //i =- 360 ;
  i += 0.01 ;

  light_position.z = sin(i)*lightOrbit;
  light_position.x = cos(i)*lightOrbit;

  glUniform4fv( gl_light_position, 1, light_position );

}


void idle( void ) {

  movelight();
  theCamera.Idle();
  glutPostRedisplay();

}

//--------------------------------------------------------------------

int main( int argc, char **argv ) {


  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( X_SIZE, Y_SIZE );
    glutCreateWindow( "Gasket Flythrough" );
    glutSetCursor( GLUT_CURSOR_NONE );
    glutWarpPointer( X_Center, Y_Center );

    GLEW_INIT();
    init();

    /* Register our Callbacks */
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
