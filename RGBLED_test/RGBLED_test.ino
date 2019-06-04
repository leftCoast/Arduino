#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

class RGBLED  : public colorObj {

  public:
  
          RGBLED(int redPin, int greenPin, int bluePin);
  virtual ~RGBLED(void);

  virtual void  idle(void);

          int   mRedPin;
          int   mGreenPin;
          int   mBluePin;
          bool  mInit;
};


RGBLED::RGBLED(int redPin, int greenPin, int bluePin) {

  mRedPin     = redPin;
  mGreenPin   = greenPin;
  mBluePin    = bluePin;
  mInit       = false;
}

RGBLED::~RGBLED(void) { }


void RGBLED::idle(void) {

  
}


void setup() {
  

}

void loop() {
  
  idle();

}
