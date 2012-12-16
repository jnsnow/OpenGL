#include "TransCache.hpp"
#include "Transformation.hpp"
#include "mat.hpp"

void TransCache::PTM( const Angel::mat4 &new_ptm ) {
  /* Apply right-to-left.
     PTM applies first, then CTM. */

  /* Update our cached PTM. */
  this->ptm = new_ptm;

  /* Update our Result Matrix. */
  otm = ctm * ptm;
}

void TransCache::CalcCTM( void ) {

  /* Recompute our CTM */
  ctm = displacement * orbit * offset * rotation * scale;

  /* Recompute our Cached Result Transformation Matrix */
  otm = ctm * ptm;

  /*
  fprintf( stderr, "CalcCTM: \n" );
  std::cerr << "OTM {" << otm << "}\n";
  std::cerr << "CTM {" << ctm << "}\n";
  std::cerr << "PTM {" << ptm << "}\n";
  */
}

const Angel::mat4 &TransCache::PTM( void ) const { return ptm; }
const Angel::mat4 &TransCache::CTM( void ) const { return ctm; }
const Angel::mat4 &TransCache::OTM( void ) const { return otm; }
