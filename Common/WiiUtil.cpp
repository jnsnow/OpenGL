#include "WiiUtil.h"
#include <wiicpp.h>
#include <iostream>
using std::cerr;

int LED_MAP[4] = {CWiimote::LED_1, CWiimote::LED_2, CWiimote::LED_3, CWiimote::LED_4};
bool tare = false;

bool initWii( CWii &wii ) {
  
  std::vector<CWiimote>* wiimotes;
  size_t found;
  
  for ( size_t attempts = 0; attempts < 5; ++attempts ) {
    
    std::cerr << "Please place at least one Wii Remote and one Wii Balance Board in discoverable mode.\n";
    std::cerr << "The Wii Remote can be placed in discoverable mode by pressing 1+2 simultaneously.\n";
    std::cerr << "The Balance Board has a SYNC button under the battery cover on the bottom.\n";
    
    found = wii.Find( 5 );
    
    if (found < 2) {
      cout << "Found " << found << " wiimote device(s), looking for 2+\n";
      continue;
    }
    
    std::vector<CWiimote>::iterator it;
    size_t index;
    wiimotes = &(wii.Connect( true ));
    
    for (index = 0, it = wiimotes->begin();
	 it != wiimotes->end();
	 ++it, ++index ) {
      CWiimote &wm = *it;
      /*CExpansionDevice &CED = wm.ExpansionDevice;*/
      wm.SetLEDs(LED_MAP[index]);;
    }
    return true; /* We're all good! */
  }
  return false; /* Couldn't find 2+ devices. */
}
