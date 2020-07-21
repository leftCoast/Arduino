#include "globals.h"

float    moisture;            // Calculated moisture %
uint16_t capread;             // Capacitence.
enum     weDo;
weDo     weAre;               // Current state.
bool     crash = false;       // Did the moisture sensor crash?
timeObj* waterTime = NULL;    // Time length to water when plant shows dry. (ms)
timeObj* soakTime = NULL;     // Time to wait after watering before going back to watching mosture level.
bool  pumpCom;                // Are we being told to turn the pump on by someone?
