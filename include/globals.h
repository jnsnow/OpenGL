#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <cmath>
#define DegreesToRadians (M_PI/180)
#define SQRT2 (1.41421356237)
#define SQRT3 (1.73205080757)
#define POW5(X) ((X)*(X)*(X)*(X)*(X))
const float DivideByZeroTolerance = float(1.0e-07);

// Turn on debugging if it's been requested of us by the Makefile environment.                    
#ifndef DEBUG
#define DEBUG false
#else
#undef DEBUG
#define DEBUG true
#endif

#endif
