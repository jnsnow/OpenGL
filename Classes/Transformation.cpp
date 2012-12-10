#include "mat.hpp"
#include "Transformation.hpp"
#include "platform.h" //OpenGL types.

Transformation::~Transformation( void ) {
  // Nihil. Provided for inheritance only.
}

const Angel::mat4 &Transformation::Matrix( void ) const {
  return mat;
}

Angel::mat4 Transformation::operator*( const Angel::mat4 &rhs ) const {
  return mat * rhs;
}

Angel::mat4 Transformation::operator*( const Transformation &rhs ) const {
  return mat * rhs.Matrix();
}

Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs ) {
  return lhs * rhs.Matrix();
}

/* ROTATION */

const RotMat &RotMat::Reset( const Angel::mat4 &NewState ) {
  mat = NewState;
  return (*this);
}

const RotMat &RotMat::RotateX( GLfloat theta ) {
  mat = Angel::RotateX( theta ) * mat;
  return (*this); 
}

const RotMat &RotMat::RotateY( GLfloat theta ) {
  mat = Angel::RotateY( theta ) * mat;
  return (*this);
}

const RotMat &RotMat::RotateZ( GLfloat theta ) {
  mat = Angel::RotateZ( theta ) * mat;
  return (*this);
}

/* TRANSLATION */

const TransMat &TransMat::Set( const float x, const float y, const float z ) {
  mat[0][3] = x;
  mat[1][3] = y;
  mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::Set( const Angel::vec3 &arg ) {
  return Set( arg.x, arg.y, arg.z );
}

const TransMat &TransMat::Delta( const float x, const float y, const float z ) {
  mat[0][3] += x;
  mat[1][3] += y;
  mat[2][3] += z;
  return (*this);
}
  
const TransMat &TransMat::Delta( const Angel::vec3 &arg ) {
  return Delta( arg.x, arg.y, arg.z );
}

/* SCALE */

const ScaleMat &ScaleMat::Set( const float x, const float y, const float z ) {

  mat[0][0] = x;
  mat[1][1] = y;
  mat[2][2] = z;
  return (*this);

}

const ScaleMat &ScaleMat::Set( const float pct ) {
  return Set( pct, pct, pct );
}

const ScaleMat &ScaleMat::Adjust( const float x, const float y, const float z ) {
  mat[0][0] *= x;
  mat[1][1] *= y;
  mat[2][2] *= z;
  return (*this);
}

const ScaleMat &ScaleMat::Adjust( const float pct ) {
  return Adjust( pct, pct, pct );
}
