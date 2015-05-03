#ifndef animations_h
#define animations_h

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "sprit.h"


class smileSprit : public bitmap8x8 {

  public :
    smileSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};


class eyesSprit : public bitmap8x8 {

  public :
    eyesSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};


class heartSprit : public bitmap8x8 {

  public :
    heartSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};


class thinkSprit : public sprit {

  public :
    thinkSprit(Adafruit_8x8matrix* inMatrix);

    virtual void startSprit(void);
            void newMap(byte odds);
            void drawMap(void);
            void addMap(void);
    virtual void idle(void);

    Adafruit_8x8matrix*  matrix;
    byte                 bitmap[8];
    timeObj              slowTimer;
    timeObj              fastTimer;
};

#endif //animations_h
