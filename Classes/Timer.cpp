#include <time.h>
#include <sys/time.h>
#include "Timer.hpp"

/* Globally provided Timer. */
Timer Tick;

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

/**
   Tick is an alias for Tock.
   Ha, Ha, Ha.
   @return An unsigned long corresponding to how much
   time has passed since the last Tick. Microseconds normally,
   Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Tick( void ) {
  return this->Tock();
}

/**
   Tock returns the time elapsed since the last Tock.
   @return An unsigned long corresponding to how much time
   has passed since the last Tock. Microseconds normally,
   Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Tock( void ) {
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

/**
   Delta returns the time elapsed between the last Tick and the last Tock.
   Does not start a new timer.
   @return Time elapsed in Microseconds, or Nanoseconds if _RT was enabled.
**/
unsigned long Timer::Delta( void ) const {
  return delta;
}
