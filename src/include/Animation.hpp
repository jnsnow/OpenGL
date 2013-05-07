#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include "vec.hpp"
#include "TransCache.hpp"
#include "Transformation.hpp"
#include "Object.hpp"

namespace Animation {

  void seekTopTo( Object *obj, float y = 0 );
  void seekBottomTo( Object *obj, float y = 0 );
  float scaleBottomFixed( Object *obj, float amt = 1.0 );
  void candleMelt( Object *candle, Object *tip, float rawScale );

}

#endif
