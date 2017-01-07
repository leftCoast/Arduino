#ifndef screen_h
#define screen_h

#include "displayObj.h"

#define ADAFRUIT_1947 1   // 2.8" TFT Touch Shield for Arduino w/Capacitive Touch.
#define ADAFRUIT_1431 2   // OLED Breakout Board - 16-bit Color 1.5" w/microSD holder.

extern displayObj* screen;

boolean initScreen(byte hardware, int inRotation=0);


#endif

