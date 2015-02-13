#ifndef policeBar_h
#define policeBar_h

#include <colorObj.h>
#include <neoPixel.h>
#include "sparkle.h"

#define NUM_LIGHTS 8
#define TIME_LOW   5
#define TIME_HIGH  100

enum lightStates {
  litesOff, flashing, flood};


class policeBar : public idler {

public:
  policeBar(neoPixel* inLights);
  ~policeBar(void);

  colorObj      chooseColor(void);
  void          setBar(lightStates inState);
  lightStates   getBar(void);
  virtual void  idle(void);

private:
  lightStates state;
  neoPixel*   lights;
  sparkle*    lightList[NUM_LIGHTS];
};

#endif
