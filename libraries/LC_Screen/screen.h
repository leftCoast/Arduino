#ifndef screen_h
#define screen_h

#include "displayObj.h"

#define ADAFRUIT_1947       1   // 2.8" TFT Touch Shield for Arduino w/Capacitive Touch.
#define ADAFRUIT_1431       2   // OLED Breakout Board - 16-bit Color 1.5" w/microSD holder.
#define ADAFRUIT_376        3	  // 2.8" TFT Touch Shield for Arduino w/Resistive Touch. Version 1
//#define SUMO_TOY_SSD_13XX   4	  // OLED Breakout Board - 16-bit Color 1.5" w/microSD holder.

// NOTE : ADAFRUIT_376 & ADAFRUIT_1431 Conflict with one another. comment one or the other 
// out in the .cpp file.

extern displayObj* screen;

boolean initScreen(byte hardware, byte csPin, byte inRotation=0);
boolean initScreen(byte hardware, byte csPin, byte rsPin, byte inRotation);
#endif

