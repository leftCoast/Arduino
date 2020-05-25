#ifndef globals_h
#define globals_h

#include <SD.h>
#include <stdint.h>
#include <timeObj.h>

extern   float    moisture;                           // The current moisture reading.
extern   float    tempC;
extern   uint16_t capread;
         enum     weDo{ sitting, watering, soaking }; // Possible states.
extern   weDo     weAre;                              // Current state.
extern   bool     crash;
extern   File     logFile;

extern   timeObj* waterTime;                          // Time length to water when plant shows dry. (ms)
extern   timeObj* soakTime;                           // Time to wait after watering before going back to watching mosture level.

extern   bool     pumpCom;                            // Are we being told to turn the pump on by someone?

#endif
