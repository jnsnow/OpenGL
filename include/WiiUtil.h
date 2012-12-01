#ifndef __WII_H
#define __WII_H

#include <wiicpp.h>

extern int LED_MAP[4];
extern bool tare;
bool initWii( CWii &wii );

#endif
