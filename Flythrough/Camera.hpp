#include "Angel.h"
#include <string>
using std::string;

class Camera {

public:

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

  typedef enum { 
    TRANSLATION,
    ROTATION,
    VIEW,
    CTM,
    NumGlslVars
  } glsl_var;
  
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
  void Move( const Camera::Direction &in );
  void Stop( const Camera::Direction &in );
  void Idle( void );

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
  
  static const float Speed;
  
private:

  void adjustRotation( const mat4 &adjustment );
  void recalculateTranslation( void );

  vec4 position;    // Absolute coordinates of camera,
  vec4 translation; // Inverse of position.
  mat4 rotational;  // Matrix for transformations
  mat4 perspective; // Matrix for perspective transformation
  mat4 ctm;         // Current Transformation Matrix (TR)

  /** Current field-of-view angle for perspective view. **/
  GLfloat fovy;

  /** Booleans correlating to the different motion directions. **/
  bool Motion[ Camera::End ];
  
  /** Handles for communicating with the shader. **/
  GLuint glsl_handles[ Camera::NumGlslVars ];

};
