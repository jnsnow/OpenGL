#include "mat.hpp"
#include "Transformation.hpp"

class TransCache {

  void PTM( const Angel::mat4 &ptm_in );

  const Angel::mat4 &PTM( void ) const;
  const Angel::mat4 &CTM( void ) const;
  const Angel::mat4 &CRTM( void ) const;

  // Component Transformations //
  ScaleMat scale;        /* Scale Matrix */
  RotMat rotation;       /* Rotation-in-place Matrix */
  TransMat offset;       /* Radius, or offset from origin. */
  RotMat orbit;          /* Rotation about the origin */
  TransMat displacement; /* Offset of entire animation from origin. */

private:

  /* Updates our CTM. Private Use. */
  void CalcCTM( void );
  
  // Cached Result Matrices
  Angel::mat4 ptm;   /* Parent's Cumulative Transformation Matrix */
  Angel::mat4 ctm;   /* Current Transformation Matrix */
  Angel::mat4 crtm;  /* Cached Result Transformation Matrix: e.g; CTM * PTM */

};
