#include <stdexcept>

#include "mat.hpp"
#include "vec.hpp"
#include "Camera.hpp"
using namespace Angel;

const float Camera::Speed = 0.01;

/**
   Initialization Constructor; sets the X,Y,Z coordinates explicitly.
   @param x The initial X coordinate.
   @param y The initial Y coordinate.
   @param z The initial Z coordinate.
**/
Camera::Camera( float x, float y, 
		float z ) {
  this->pos( x, y, z, false );
}


/**
   Initialization Constructor, uses a vec3 as its initial coordinates.
   @param in A vec3 representing the initial coordinates.
**/
Camera::Camera( vec3 &in ) {
  this->pos( in, false );
}


/**
   Initialization Constructor, uses a vec4 as its initial coordinates.
   @param in A vec4 representing the initial coordinates. The w component is ignored.
**/
Camera::Camera( vec4 &in ) {
  this->pos( in, false );
}


/**
   Default destructor. Nothing of note.
**/
Camera::~Camera( void ) { }


/**
   Sets the X coordinate of the camera.
   @param in     The new X coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::X( const float &in, const bool &update ) { 
  ctm[0][3] = T[0][3] = -in;
  if (update) send( TRANSLATION );
}


/**
   Sets the Y coordinate of the camera.
   @param in     The new Y coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::Y( const float &in, const bool &update ) { 
  ctm[1][3] = T[1][3] = -in;
  if (update) send( TRANSLATION );
}


/**
   Sets the Z coordinate of the camera.
   @param in     The new Z coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::Z( const float &in, const bool &update ) { 
  ctm[2][3] = T[2][3] = -in;
  if (update) send( TRANSLATION );
}


/**
   Sets the absolute position of the camera.
   @param x The new X coordinate of the camera.
   @param y The new Y coordinate of the camera.
   @param z The new Z coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::pos( const float &x, const float &y, 
		  const float &z, const bool &update ) {
  X(x, false);
  Y(y, false);
  Z(z, false);
  if (update) send( TRANSLATION );
}


/**
   Sets the absolute position of the camera.
   @param in A vec4 containing the x, y, and z coordinates to set the camera to. The w coordinate is ignored.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::pos( const vec4 &in, const bool &update ) {
  this->pos( in.x, in.y, in.z, update );
}


/**
   Sets the absolute position of the camera.
   @param in A vec3 containing the x, y, and z coordinates to set the camera to.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
**/
void Camera::pos( const vec3 &in, const bool &update ) {
  this->pos( in.x, in.y, in.z, update );
}

  
/**
   Moves the camera along the X axis.
   @param by The float value of the X-axis displacement.
   @param update A boolean indicating whether or not to update the shader. 
   update defaults to true.
   @return void.
**/
void Camera::dX( const float &by, const bool &update ) { 
  X( X() + by, update ); 
}


/**
   Moves the camera along the Y axis.
   @param by The float value of the Y-axis displacement.
   @param update A boolean indicating whether or not to update the shader.
   update defaults to true.
   @return Void.
**/
void Camera::dY( const float &by, const bool &update ) {
  Y( Y() + by, update );
}


/**
   Moves the camera along the Z axis.
   @param by The float value of the Z-axis displacement.
   @param update A boolean indicating whether or not to update the shader.
   update defaults to true.
   @return Void.
**/
void Camera::dZ( const float &by, const bool &update ) { 
  Z( Z() + by, update ); 
}


/** 
    Moves the camera along the x, y, and z axes.
    @param x the X-axis displacement.
    @param y the Y-axis displacement.
    @param z the Z-axis displacement.
    @return Void.
**/
void Camera::dPos( const float &x, const float &y, 
		   const float &z ) { 
  dX( x, false );
  dY( y, false );
  dZ( z, false );
  send( TRANSLATION );
}


/** 
    Moves the camera along the x, y, and z axes.
    @param by A vec3 containing the X, Y, and Z axis displacements.
    @return Void.
**/
void Camera::dPos( const vec3 &by ) {
  dPos( by.x, by.y, by.z );
}


/** 
    Moves the camera along the x, y, and z axes.
    @param by A vec4 containing the X, Y, and Z axis displacements. The w component is ignored.
    @return Void.
**/
void Camera::dPos( const vec4 &by ) {
  dPos( by.x, by.y, by.z );
}


/** 
    adjustRotation is an internal function that rotates the camera.
    Technically, any transformation, not just a rotation, is possible.
    @param adjustment The 4x4 matrix to transform the CTM by.
    @return Void.
**/
void Camera::adjustRotation( const mat4 &adjustment ) {
#ifdef POSTMULT
  // In a post-mult system, the argument order is left-to-right,
  // So the adjustment appears last.
  R = R * adjustment;
#else
  // In a pre-mult system, the last argument is applied first,
  // So the adjustment should appear first.
  R = adjustment * R;
#endif
  send( ROTATION );
}

/** 
  ROTATE_OFFSET is a macro which is used to normalize
  the six camera motion directions with respect to the
  current camera rotation. It is used in heave(), sway() and surge().
  @param V a vec4 representing the movement offset vector.
  @return A rotated vec4.
**/
//#define ROTATE_OFFSET(V) (transpose(R) * V)
#define ROTATE_OFFSET(V) (V * R)


/**
   Adjusts the camera's X coordinate relative to its current position.
   Negative values move the camera left,
   and positive values move the camera right.
   @param by The float to adjust the X coordinate by.
   @return Void.
**/
void Camera::sway( const float &by ) {
  dPos(ROTATE_OFFSET(vec4(by,0,0,0)));
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
  dPos(ROTATE_OFFSET(vec4(0,0,-by,0)));
}


/**
   Adjusts the camera's Y coordinate relative to its current position.
   Positive values move the camera up,
   and negative values move the camera down.
   @param by The float to adjust the Y coordinate by.
   @return Void.
**/
void Camera::heave( const float &by ) {
  dPos(ROTATE_OFFSET(vec4(0,by,0,0)));
}


/**
   pitch adjusts the X axis rotation; up/down look.
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
  adjustRotation(RotateX(-by));
}


/**
   yaw adjusts the Y axis rotation; left/right look.
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
  adjustRotation(RotateY(by));
}


/**
   roll adjusts the Z axis rotation; tilt or lean left/right.
   A positive value represents leaning right,
   while a negative value represents leaning left.
   @param by A float, in degrees, to adjust the roll by.
   @return Void.
**/
void Camera::roll( const float &by ) {
  adjustRotation(RotateZ(by));
}


/**
   Move instructs the camera to begin moving in the specified direction. 
   @param Dir The direction in which to move.
     Can be any direction in the enumerated type Camera::Direction.
   @return Void.
**/
void Camera::Move( const Camera::Direction &Dir ) { 
  Motion[Dir] = true; 
}


/**
   Stop instructs the camera to stop moving in the specified direction.
   @param Dir The direction in which to stop moving.
   @return Void.
**/
void Camera::Stop( const Camera::Direction &Dir ) {
  Motion[Dir] = false;
}


/**
   Idle moves the camera forward in whichever directions it is
   configured to move in. Call it in the glut Idle function.
   @return Void.
**/
void Camera::Idle( void ) {

  if (Motion[Camera::Forward]) surge( Camera::Speed );
  else if (Motion[Camera::Backward]) surge( -Camera::Speed );

  if (Motion[Camera::Right]) sway( Camera::Speed );
  else if (Motion[Camera::Left]) sway( -Camera::Speed );

  if (Motion[Camera::Up]) heave( Camera::Speed );
  else if (Motion[Camera::Down]) heave( -Camera::Speed );

}


/**
   X() returns the current position of the camera in model coordinates.
   @return The current X coordinate of the camera in model coordinates.
**/
float Camera::X( void ) const { return -T[0][3]; }


/**
   Y() returns the current position of the camera in model coordinates.
   @return The current Y coordinate of the camera in model coordinates.
**/
float Camera::Y( void ) const { return -T[1][3]; }


/**
   Z() returns the current position of the camera in model coordinates.
   @return The current Z coordinate of the camera in model coordinates.
**/
float Camera::Z( void ) const { return -T[2][3]; }


/**
   pos() gets the current camera position in model coordinates.
   @return A vec4 that represents the current camera coordinates.
**/
vec4 Camera::pos( void ) const { return vec4( X(), Y(), Z(), 1.0 ); }


/**
   FOV() gets the current camera Field-of-view angle.
   @return A float that is the y axis viewing angle.
**/
float Camera::FOV( void ) const { return fovy; }


/**
   FOV sets the current camera Field-of-view angle.
   This function will send the new perspective matrix to the shader.
   @param in The new field of view angle.
   @return Void.
**/
void Camera::FOV( const float &in ) { 
  GLint size[4];
  fovy = in;
  glGetIntegerv( GL_VIEWPORT, size );  
  P = Perspective( fovy,
		   (float)size[2]/(float)size[3],
		   0.001, 100.0 );
  send( PERSPECTIVE );
}


/**
   dFOV adjusts the field of view angle up or down by an amount.
   @param by The float to adjust the FOV angle by.
   @return Void.
**/
void Camera::dFOV( const float &by ) {
  FOV( FOV() + by );
}


/**
   send will send a glsl variable to the shader.
   @param which The parameter to send. Can be any from enum glsl_var.
   @return Void.
**/
void Camera::send( const glsl_var &which ) {

  switch (which) {
  case TRANSLATION:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_FALSE, T );
    send( PRT_M );
    break;
  case ROTATION:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_FALSE, R );
    send( PRT_M );
    break;
  case PERSPECTIVE:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_FALSE, P );
    send( PRT_M );
    break;
  case TRP_M:
    TRP = T*R*P;
    glUniformMatrix4fv( glsl_handles[which], 1, GL_FALSE, TRP );
    break;
  case PRT_M:
    PRT = P*R*T;
    glUniformMatrix4fv( glsl_handles[which], 1, GL_TRUE, PRT );
    break;
  case CTM:
    glUniformMatrix4fv( glsl_handles[which], 1, GL_FALSE, ctm );
    break;
  default:
    throw std::invalid_argument( "Unknown GLSL variable handle." );
  }
}


/**
   Link associates the camera with a glsl uniform variable.
   @param program a GLuint handle to the shader application.
   @param which A glsl_var enumeration indication which variable to link.
   @param glslVarName The name of the variable in the shader.
   @return Void.
**/
void Camera::link( GLuint &program, const glsl_var &which, 
		   const string &glslVarName ) {
  
  glsl_handles[which] = glGetUniformLocation( program, 
					      glslVarName.c_str() );
  send( which );

}
