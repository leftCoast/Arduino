#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#define NUM_LEDS  8
#define LED_PIN   3


class cylonScan : public neoPixel,
                  public idler {

  public:
                  cylonScan(int numLEDs,int LEDPin);
   virtual        ~cylonScan(void);

            void      setColors(colorObj* backColor,colorObj* scanColor);
            void      setFrameTime(float frameMS);
            void      drawtLEDs(void);
   virtual  void      idle(void);

            colorObj  backColor;
            colorObj  scanColor;
            timeObj   frameTimer;
            int       maxIndex;
            byte      frame;
            bool      goingUp;
  };



// colors are defaulted to black so setColors must be caklled.
// This is good because serColors() calls hookup.
cylonScan::cylonScan(int numLEDs,int LEDPin)
  : neoPixel(numLEDs,LEDPin), idler() {

    backColor.setColor(&black);   // Default to black.
    scanColor.setColor(&black);   // Default to black.
    frameTimer.setTime(250);      // Default to 250 milliseconds.
    maxIndex = numLEDs - 1;
    frame = 0;
    goingUp = true;
  }

  
cylonScan::~cylonScan(void) {  }


void cylonScan::setColors(colorObj* inBackColor,colorObj* inScanColor) {

  hookup();
  backColor.setColor(inBackColor);
  scanColor.setColor(inScanColor);
}


void cylonScan::setFrameTime(float frameMS) { frameTimer.setTime(frameMS) ;}


void  cylonScan::drawLEDs() {

  setAll(backColor);
  setPixalColor(frame,scanColor);
  show();
}


void cylonScan::idle(void) {

  if (frameTimer.ding()) {
    frameTimer.stepTime();
    if (goingUp) {
      frame++;
      drawLEDs();
      if (frame>=maxIndex) {
        goingUp = false;
      }
    } else {
      frame--;
      drawLEDs();
      if (frame<=0) {
        goingUp = true;
      }
    }
  }
}

  
cylonScan scanner(NUM_LEDS,LED_PIN);

void setup() {
  scanner.begin();
  scanner.setPixelColor(2,&green);
  scanner.show();
}


void loop() {

  
}
