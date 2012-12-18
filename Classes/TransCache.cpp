#include "TransCache.hpp"
#include "Transformation.hpp"
#include "mat.hpp"

void TransCache::PTM( const Angel::mat4 &new_ptm ) {
  /* Update our cached PTM. */
  this->ptm = new_ptm;
  /* Update our Result Matrix. */
  otm = ptm * ctm;
}

void TransCache::CalcCTM( void ) {
  /*
  std::cerr << "displacement" << displacement.Matrix() << "\n";
  std::cerr << "orbit" << orbit.Matrix() << "\n";
  std::cerr << "offset" << offset.Matrix() << "\n";
  std::cerr << "rotation" << rotation.Matrix() << "\n";
  std::cerr << "scale" << scale.Matrix() << "\n";
  */

  /* Recompute our CTM */
  ctm = displacement * orbit * offset * PreOffset * rotation * scale;
  /* Recompute our Cached Result Transformation Matrix */
  otm = ptm * ctm;
  
  /*
    std::cerr << "CTM:" << ctm << "\n";
    std::cerr << "---------------\n";
  */

}

const Angel::mat4 &TransCache::PTM( void ) const { return ptm; }
const Angel::mat4 &TransCache::CTM( void ) const { return ctm; }
const Angel::mat4 &TransCache::OTM( void ) const { return otm; }
