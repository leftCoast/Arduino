#ifndef screen_h
#define screen_h

#include "displayObj.h"

#define ADAFRUIT_1947		1   // 2.8" TFT Touch Shield for Arduino w/Capacitive Touch.
#define ADAFRUIT_1431		2   // OLED Breakout Board - 16-bit Color 1.5" w/microSD holder.
#define ADAFRUIT_684			3   // OLED Breakout Board - 16-bit Color 0.69" w/microSD holder.

extern displayObj* screen;

bool initScreen(byte hardware, byte csPin, byte inRotation=0);
bool initScreen(byte hardware, byte csPin, byte rsPin, byte inRotation);

#endif

