#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#include <colorObj.h>

#include <liteLine.h>
#include <neoPixel.h>
  
#include "chainPixels.h"
#include "robbyRing.h"

#define NUM_LEDS 12
#define LED_PIN 3



 
class  tialLight : public pixelGroup {
  
  public:
            tialLight(void);
            ~tialLight(void);
            
    virtual void   draw(void);
    
            boolean drawn;
};


tialLight::tialLight(void) : pixelGroup(8) { drawn = false; }


tialLight::~tialLight(void) { }
            
            
 void tialLight::draw(void) {
   
      if (!drawn) {
        colorObj result;
        result.setColor(&red);
        result = result.blend(&black,75);
        result = result.blend(&blue,15);
        setPixels(&result);
        drawn = true;
      }
 }


chainPixels theChain(LED_PIN);
robbyRing* ring1;
tialLight* lightStick;
  
void setup() {
  
  //Serial.begin(9600);
  //delay(10000);  // 10 seconds to get screen up.
  
  ring1 = new robbyRing(NUM_LEDS);
  theChain.addGroup(ring1);
  
  lightStick = new tialLight();
  theChain.addGroup(lightStick);
  
  theChain.hookup();
}

void loop() { idle(); }
