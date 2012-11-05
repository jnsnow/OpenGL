#include "Angel.h"
#include "Camera.hpp"
#include "mat_util.h"

typedef void (Camera::*mov_func)( const float &by );
const bool Camera::ON = true;
const bool Camera::OFF = false;
const bool Camera::FORWARD = true;
const bool Camera::BACKWARD = false;
const bool Camera::LEFT = false;
const bool Camera::FORWARD = true;
const bool Camera::REVERSE = false;
const bool Camera::FORWARD = true;


/**** CONSTRUCTORS ****/

Camera::Camera( float x, float y, 
		float z, float w ) {
  this->pos( x, y, z, w );
}

Camera::Camera( vec3 &in ) {
  this->pos( in );
}

Camera::Camera( vec4 &in ) {
  this->pos( in );
}

Camera::~Camera( void ) { }


/**** SETTERS AND DATA MODIFYING FUNCTIONS ****/

// Set Camera Positionals
void Camera::X( const float &in ) { 
  position.x = in;
  translation.x = -in;
}
void Camera::Y( const float &in ) { 
  position.y = in;
  translation.y = -in;
}
void Camera::Z( const float &in ) { 
  position.z = in;
  translation.z = -in;
}
void Camera::W( const float &in ) { 
  position.w = in;
}
void Camera::pos( const float &x, const float &y, 
		  const float &z, const float &w ) {
  X(x);
  Y(y);
  Z(z);
  W(w);
}
void Camera::pos( const vec4 &in ) {
  X(in.x);
  Y(in.y);
  Z(in.z);
  W(in.w);
}
void Camera::pos( const vec3 &in ) {
  X(in.x);
  Y(in.y);
  Z(in.z);
}

// Adjust Camera Positionals
void Camera::dX( const float &by ) { X( X() + by ); }
void Camera::dY( const float &by ) { Y( Y() + by ); }
void Camera::dZ( const float &by ) { Z( Z() + by ); }
void Camera::dW( const float &by ) { W( W() + by ); }
void Camera::dPos( const float &x, const float &y, 
		   const float &z, const float &w ) { 
  dX( x );
  dY( y );
  dZ( z );
  dW( w );
}
void Camera::dPos( const vec3 &by ) {
  dPos( by.x, by.y, by.z, 0.0 );
}
void Camera::dPos( const vec4 &by ) {
  dPos( by.x, by.y, by.z, by.w );
}

// Adjust Camera Orientation
void Camera::adjustRotation( const mat4 &adjustment ) {
  rotational = rotational * adjustment;
  //updateCTM();
}


/**
   Adjusts the camera's X coordinate relative to its current position.
   Negative values move the camera left,
   and positive values move the camera right.
   @param by The float to adjust the X coordinate by.
   @return Void.
**/
void Camera::sway( const float &by ) {
  dPos(rotational * vec4(by,0,0,0));
}


/**
   Adjusts the camera's Z coordinate relative to its current position.
   Positive values move the camera forward,
   and negative values move the camera backward.
   Note that the camera uses model coordinates internally,
   so moving forward will increase the camera's Z position negatively.
   @param by The float to adjust the Z coordinate by.
   @return Void.
**/
void Camera::surge( const float &by ) {
  dPos(rotational * vec4(0,0,-by,0));
}


/**
   Adjusts the camera's Y coordinate relative to its current position.
   Positive values move the camera up,
   and negative values move the camera down.
   @param by The float to adjust the Y coordinate by.
   @return Void.
**/
void Camera::heave( const float &by ) {
  dPos(rotational * vec4(0,by,0,0));
}


/**
   pitch adjusts the X axis rotation (up/down look.)
   A positive value represents looking up,
   while a negative value represents looking down.
   @param by A float, in degrees, to adjust the pitch by.
   @return Void.
**/
void Camera::pitch( const float &by ) {
  /*
    Since negative values are interpreted as pitching down,
    We leave the input uninverted, because a negative rotation
    about the X axis rotates counter-clockwise (looking right),
    and clockwise (looking left), which achieves the effect of
    looking 'down'.
  */ 
  adjustRotation(RotateX(by));
}


/**
   yaw adjusts the Y axis rotation (left/right look.)
   A positive value represents looking right,
   while a negative value represents looking left.
   @param by A float, in degrees, to adjust the yaw by.
   @return Void.
**/
void Camera::yaw( const float &by ) {
  /*
    Since a positive 'by' should represent looking right,
    we invert the rotation because rotating by a positive value
    will rotate right, which simulates looking left.
    Therefore, invert.
  */
  adjustRotation(RotateY(-by));
}


/**
   roll adjusts the Z axis rotation (tilt or lean left/right.)
   A positive value represents leaning right,
   while a negative value represents leaning left.
   @param by A float, in degrees, to adjust the roll by.
   @return Void.
**/
void Camera::roll( const float &by ) {
  adjustRotation(RotateZ(-by));
}

void Camera::autoSway( const bool &on, const bool &positive ) {
  autoMove( on, positive, 0 );
}

void Camera::autoHeave( const bool &on, const bool &positive ) {
  autoMove( on, positive, 1 );
}

void Camera::autoSurge( const bool &on, const bool &positive ) {
  autoMove( on, positive, 2 );
}

void Camera::autoMove( const bool &on, const bool &positive, 
		       int axis ) {
  if (on) {
    if (positive) {
      autoMoveBools[axis][0] = true;
      autoMoveBools[axis][1] = false;
    } else {
      autoMoveBools[axis][0] = false;
      autoMoveBools[axis][1] = true;
    }      
  } else {
    autoMoveBools[axis][0] = false;
    autoMoveBools[axis][1] = false;
  }
}

void Camera::idleMove( void ) {
  static const mov_func mov_funcs[3] = { &Camera::sway,
					 &Camera::heave,
					 &Camera::surge };

  for ( size_t i = 0; i < 3; i++ ) {
    mov_func fptr = mov_funcs[i];
    if (autoMoveBools[i][0]) (this->*fptr)( 0.01  );
    else if (autoMoveBools[i][1]) (this->*fptr)( -0.01 );
  }

}


/**** READ-ONLY FUNCTIONS / GETTERS ****/

// Get Position
float Camera::X  ( void ) const { return position.x; }
float Camera::Y  ( void ) const { return position.y; }
float Camera::Z  ( void ) const { return position.z; }
float Camera::W  ( void ) const { return position.w; }
 vec4 Camera::pos( void ) const { return position;   }
float Camera::FOV( void ) const { return fovy;       }


// Field of View
void Camera::FOV( const float &in ) { 
  GLint size[4];
  fovy = in;
  glGetIntegerv( GL_VIEWPORT, size );  
  perspective = Perspective( fovy,
			     (float)size[2]/(float)size[3],
			     0.3, 3.0 );
  send( VIEW );
}

void Camera::dFOV( const float &by ) {
  FOV( FOV() + by );
}

/**** PRIVATE METHODS ****/
/**** OPENGL METHODS ****/

void Camera::send( const glsl_var &which ) const {
  
  switch (which) {
  case TRANSLATION:
    glUniform4fv( glsl_handles[which], 1, position );
    break;
    
  case ROTATION:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_TRUE, rotational );
    break;

  case VIEW:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_TRUE, perspective );
    break;

  case CTM:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_TRUE, ctm );
    break;
  }
}

void Camera::link( GLuint &program, const glsl_var &which, const string &glslVarName ) {
  
  glsl_handles[which] = glGetUniformLocation( program, glslVarName.c_str() );
  
}
