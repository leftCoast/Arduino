#ifndef globals_h
#define globals_h

#include <stdint.h>


extern  float     moisture;                             // The current moisture reading.
extern  float     tempC;
extern  uint16_t  capread;
        enum      weDo{ sitting, watering, soaking };  // Possible states.
extern  weDo      weAre;                                // Current state.
extern  bool      crash;

#endif
