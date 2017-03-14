#ifndef mechButton_h
#define mechButton_h

#include <Arduino.h>
#include <timeObj.h>

#define BOUNCE_MS 50

class mechButton : public timeObj {

  public:
    mechButton(byte inPinNum);

    bool  clicked(void);

    bool  beenInitialized;
    byte  pinNum;
};

#endif

