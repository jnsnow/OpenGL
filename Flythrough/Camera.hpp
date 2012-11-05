#include "Angel.h"
#include <string>
using std::string;

typedef enum { TRANSLATION, ROTATION, VIEW, CTM } glsl_var;

class Camera {

public:

  typedef enum {
    FORWARD2,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN } Direction;
  
  static const bool ON;
  static const bool OFF;
  static const bool REVERSE;
  static const bool FORWARD;

  //  Camera( void );
  Camera( float x = 0.0, float y = 0.0,
	  float z = 0.0, float w = 0.0 );
  Camera( vec3 &in );
  Camera( vec4 &in );
  virtual ~Camera( void );
  
  /* Set positionals: forcibly move the camera */
  void X( const float &in );
  void Y( const float &in );
  void Z( const float &in );
  void W( const float &in );
  void pos( const float &x, const float &y, 
	    const float &z, const float &w );
  void pos( const vec3 &in );
  void pos( const vec4 &in );
  
  /* Adjust positionals: move the camera by an offset */
  void dX( const float &by );
  void dY( const float &by );
  void dZ( const float &by );
  void dW( const float &by );
  void dPos( const float &x, const float &y, 
	     const float &z, const float &w );
  void dPos( const vec3 &by );
  void dPos( const vec4 &by );
  
  /* Field-of-View */
  void FOV( const float &fovy );
  float FOV( void ) const;
  void dFOV( const float &by );
  
  /* Adjust the camera position with regards to its current vector */
  void sway( const float &by );
  void surge( const float &by );
  void heave( const float &by );
  
  /* Adjust the camera's current view angle */
  void pitch( const float &by );
  void yaw( const float &by );
  void roll( const float &by );

  /* Instruct the camera to automatically move. */
  void autoSway( const bool &on, const bool &positive = true );
  void autoSurge( const bool &on, const bool &positive = true );
  void autoHeave( const bool &on, const bool &positive = true );
  void idleMove( void );

  /* Get Position */
  float X( void ) const;
  float Y( void ) const;
  float Z( void ) const;
  float W( void ) const;
  vec4 pos( void ) const;

  /* OpenGL Methods */
  void send( const glsl_var &which ) const;
  void link( GLuint &program,
	     const glsl_var &which,
	     const string &glslVarName );

private:

  void initDirection( void );
  void autoMove( const bool &on, const bool &positive, int axis );
  void adjustRotation( const mat4 &adjustment );
  void recalculateTranslation( void );

  vec4 position;    // Absolute coordinates of camera,
  vec4 translation; // Inverse of position.
  mat4 rotational;  // Matrix for transformations
  mat4 perspective; // Matrix for perspective transformation
  mat4 ctm;         // Current Transformation Matrix (TR)

  GLfloat fovy;             // Current field-of-view angle.
  bool autoMoveBools[3][2]; // Booleans corresponding to automated motion (SWAY, HEAVE, SURGE)
  GLuint glsl_handles[4];   // Handles for communicating with the shader.

};
