#ifndef __CAMERA_H
#define __CAMERA_H


#include <string>
#include "mat.hpp"
#include "vec.hpp"

using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;

/**
   @author John Huston, jhuston@cs.uml.edu
   @since 16 Nov 2012

   @brief The Camera class represents a logical camera in a model view,
   which posesses a current viewing angle and an absolute position 
   in space as its state.

   Functions are provided to adjust the rotation according to 
   pitch(), yaw() and roll() motions; surge(), sway(), and heave()
   are provided to adjust position in space.

   Move(), Stop(), and Idle() are provided to help the camera
   automatically move along the X, Y, or Z axes.
**/
class Camera {

public:

  /**
     The Direction enumeration lists all of the possible directions
     the camera may travel in. 'Begin' and 'End' are special sentinel
     directions for the purposes of iteration, and are ignored by
     any functions that accept a Direction.
  **/
  typedef enum {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down,
    End,
    Begin = Forward
  } Direction;


  /** 
      The glsl_var enumeration lists the various variables the
      Camera class is capable of sending to the shader.
      The NumGlslVars variable is a sentinel value that is ignored
      by any functions that accept a glsl_var.
  **/
  typedef enum { 
    TRANSLATION,
    ROTATION,
    VIEW,
    CTM, /* CTM is either P*R*T or T*R*P, depending */
    NumGlslVars
  } glsl_var;

  /**
     The view_type enumeration lists the various possibilities
     for the current viewing mode that can be switched between.
     The default is PERSPECTIVE.
  **/
  typedef enum {
    PERSPECTIVE,
    ORTHO,
    IDENTITY,
    FRUSTRUM
  } view_type;
  
  Camera( float x = 0.0, float y = 0.0,
	  float z = 0.0 );
  Camera( vec3 &in );
  Camera( vec4 &in );
  virtual ~Camera( void );
  
  /* Set positionals: forcibly move the camera */
  void X( const float &in, const bool &update = true );
  void Y( const float &in, const bool &update = true );
  void Z( const float &in, const bool &update = true );
  void pos( const float &x, const float &y, 
	    const float &z, const bool &update = true );
  void pos( const vec3 &in, const bool &update = true );
  void pos( const vec4 &in, const bool &update = true );
  
  /* Adjust positionals: move the camera by an offset */
  void dX( const float &by, const bool &update = true );
  void dY( const float &by, const bool &update = true );
  void dZ( const float &by, const bool &update = true );
  void dPos( const float &x, const float &y, 
	     const float &z );
  void dPos( const vec3 &by );
  void dPos( const vec4 &by );
  
  /* Field-of-View */
  void FOV( const float &fovy );
  float FOV( void ) const;
  void dFOV( const float &by );
  void changePerspective( const int &in );
  void resize( void );

  /* Adjust the camera position with regards to its current vector */
  void sway( const float &by );
  void surge( const float &by );
  void heave( const float &by );
  
  /* Adjust the camera's current view angle */
  void pitch( const float &by );
  void yaw( const float &by );
  void roll( const float &by );

  /* Instruct the camera to automatically move. */
  void Move( const Camera::Direction &Dir );
  void Stop( const Camera::Direction &Dir );
  void Idle( void );

  /* Get Position */
  float X( void ) const;
  float Y( void ) const;
  float Z( void ) const;
  vec4 pos( void ) const;

  /* OpenGL Methods */
  void send( const glsl_var &which );
  void link( const GLuint &program,
	     const glsl_var &which,
	     const string &glslVarName );
    
private:

  void adjustRotation( const mat4 &adjustment );

  mat4 T;
  mat4 R;
  mat4 P;
  mat4 ctm;

  view_type currView;

  /** Current speed of camera motion **/
  GLfloat speed;

  /** Upper-bound for absolute speed. **/
  GLfloat maxSpeed;

  /** Current acceleration. **/
  GLfloat accel;

  /** Maximum acceleration. **/
  GLfloat maxAccel;

  /** Current field-of-view angle for perspective view. **/
  GLfloat fovy;

  /** Booleans correlating to the different motion directions. **/
  bool Motion[ Camera::End ];
  
  /** Handles for communicating with the shader. **/
  GLuint glsl_handles[ Camera::NumGlslVars ];

public:
  
  /** The increment the camera should move during the Idle() function. **/
  static const float initSpeed; // Set in Camera.cpp. Sorry! Maybe someday c++0x will exist.

};

#endif
