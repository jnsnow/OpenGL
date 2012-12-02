#ifndef __WII_H
#define __WII_H

#include <wiicpp.h>
#include "vec.hpp"

extern int LED_MAP[4];
extern Angel::vec2 bb_magnitudes;
bool initWii( CWii &wii );
void pollWii( CWii &wii );
void WiiHandleBB( CWiimote &wm );

#endif
