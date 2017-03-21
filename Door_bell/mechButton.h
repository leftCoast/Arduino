#ifndef mechButton_h
#define mechButton_h

#include <Arduino.h>
//#include <timeObj.h>

#define BOUNCE_MS   50
#define NUM_CHECKS  5

//class mechButton : public timeObj {
class mechButton {
  
  public:
    mechButton(byte inPinNum);

    bool  clicked(void);

    bool  beenInitialized;
    byte  pinNum;
    byte  checkNum;
    byte  setAs;
};

#endif

