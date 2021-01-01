#ifndef sparkles_h
#define sparkles_h

#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      75
#define SPARKLE_LOW_LIFE  15
#define SPARKLE_LIFE      50
#define SPARKLE_ODDS      0.125 // x+1 to one.  IE 1 = 1+1 to 1 or 1:2 chance.
#define SPARKLE_COLORS    25
#define MAX_SPARKLES      120


// **************************************
// ************** sparkle ***************
// **************************************


class sparkle : public linkListObj, public timeObj {

  public:
            sparkle(neoPixel* inLEDs, uint16_t inIndex, colorObj* inColor);
    virtual ~sparkle(void);

    void erase(void); // Put the saved color back.
    void draw(void);  // Save off the base color then do whatever.

    neoPixel* LEDs;
    uint16_t  index;
    colorObj  backColor;
    colorObj  ourColor;
};



// **************************************
// ************** sparkles **************
// **************************************


class sparkles : public linkList {

  public:
            sparkles(neoPixel* inLEDs);
    virtual ~sparkles(void);

    void  addSparkle(uint16_t index, colorObj* sparkleColor);
    void  erase(void);
    void  cleanUp(void);
    void  draw(void);

    neoPixel* LEDs;
};


#endif
