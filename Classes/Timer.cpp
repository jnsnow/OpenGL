#include <time.h>
#include <sys/time.h>
#include "Timer.hpp"

#define SecToNSec (1000000000)
#define SecTouSec (1000000)
#define SecToMSec (1000)

Timer::Timer( void ) {
  #ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_T1 );
  clock_gettime( CLOCK_REALTIME, &_T2 );
  #else
  gettimeofday( &_T1, NULL );
  gettimeofday( &_T2, NULL );
  #endif
}

unsigned long Timer::Tick( void ) {
#ifdef _RT
  clock_gettime( CLOCK_REALTIME, &_T2 );
  this->delta = (_T2.tv_nsec - _T1.tv_nsec) + (SecToNSec * (_T2.tv_sec - _T1.tv_sec));
#else
  gettimeofday( &_T2, NULL );
  this->delta = (_T2.tv_usec - _T1.tv_usec) + (SecTouSec * (_T2.tv_sec - _T1.tv_sec));
#endif
  _T1 = _T2;
  return delta;
}

unsigned long Timer::Delta( void ) const {
  return delta;
}
