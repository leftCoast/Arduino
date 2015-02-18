#ifndef policeBar_h
#define policeBar_h

#include <timeObj.h>
#include <chainPixels.h>

enum pbStates { pbOff, pbFlood, pbBlink };

class policeBar : public pixelGroup {
  
  public:
            policeBar(void);    // Hardcoded for 8 LED light sick.
            ~policeBar(void);
            
            void       lightOff(void);
            void       floodOn(void);
            void       blinkOn(void);
            void       setupBlink(void);
            colorObj*  chooseColor(int index);
            void       setLights(int index);
            void       blinkTime(void);
    virtual void       draw(void);
    
  private:
          boolean  init;
          pbStates  nextState;
          pbStates  state;
          timeObj   flashTimer;
          int       flashIndex;
          colorObj  pbRed;
          colorObj  pbBlue;
};

#endif

