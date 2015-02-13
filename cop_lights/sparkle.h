#ifndef sparkle_h
#define sparkle_h


#include <idlers.h>
#include <neoPixel.h>
#include <colorObj.h>
#include <timeObj.h>

// Single LED that flashes in background.
class sparkle : public idler {

public:
  sparkle(neoPixel* inLights,word inLoc,float inOnTime=10);
  ~sparkle(void);

           void    setColor(colorObj* inColor);
           void    setTime(float inOnTime=10);
           boolean getState(void);
           void    flash(void);
           void    fade(void);
  virtual  void    idle(void);

protected:
  neoPixel* lights;
  word      loc;
  colorObj  color;
  colorObj  savedColor;
  timeObj   onTime;
  boolean   lightOn;
  boolean   alwaysOn;
};

#endif
