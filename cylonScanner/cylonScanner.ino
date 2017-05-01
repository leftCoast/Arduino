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


//                   ***********  A general pupose scanner class  ***********


class cylonScan : public neoPixel,
                  public idler {

  public:
                  cylonScan(int numLEDs,int LEDPin,neoPixelType LEDType=NEO_GRB + NEO_KHZ800);
   virtual        ~cylonScan(void);

            void      setColors(colorObj* backColor,colorObj* scanColor);
            void      setFrameTime(float frameMS);
            void      drawLEDs(void);
   virtual  void      idle(void);

            colorObj  backColor;
            colorObj  scanColor;
            timeObj   frameTimer;
            int       maxIndex;
            byte      frame;
            bool      goingUp;
  };



// Colors are defaulted to black so setColors must be called.
// This is good because serColors() calls hookup().
cylonScan::cylonScan(int numLEDs,int LEDPin,neoPixelType LEDType)
  : neoPixel(numLEDs,LEDPin,LEDType), idler() {

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


void cylonScan::setFrameTime(float frameMS) { frameTimer.setTime(frameMS); }


void  cylonScan::drawLEDs() {

  setAll(&backColor);
  setPixelColor(frame,&scanColor);
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


//       ***********  Your sketch is here. All that's left of it. The class runs your lights in the background for you.  *********** 
//                The one rule. Don't feed them meat.. No, really. You can add things to this ketch, but DON'T use delay().

#define NUM_LEDS  8     // Change to match your setup
#define LED_PIN   3     // This one too.

//cylonScan scanner(NUM_LEDS,LED_PIN,NEO_GRBW+NEO_KHZ800);  // This one for 4 color
cylonScan scanner(NUM_LEDS,LED_PIN);                        // This one for 3 color.

void setup() {

  colorObj  backColor(WHITE); // Initial colors from stock.
  colorObj  scanColor(BLUE);

  backColor.blend(&black,95);   // Tone down the white . Blend in 95% black.
  scanColor.blend(&green,30);   // Bring out thre blue, toss in some green.
  
  scanner.begin();
  scanner.setColors(&backColor,&scanColor);   // set in our colors.
  //scanner.setColors(&black,&red);           // uncomment this for a different look.
  scanner.setFrameTime(100);                  // 100 ms between moves.
}


void loop() {
  idle();

}
