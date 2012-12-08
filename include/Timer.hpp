#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <time.h>

class Timer { 

public:
  Timer();  
  unsigned long Tick();
  unsigned long Delta() const;

private:
#ifdef _RT
  struct timespec _T1;
  struct timespec _T2;
#else
  struct timeval _T1;
  struct timeval _T2;
#endif
  unsigned long delta;
};

#endif
