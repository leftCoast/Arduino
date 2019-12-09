#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      75
#define SPARKLE_LOW_LIFE  25
#define SPARKLE_LIFE      1000
#define SPARKLE_ODDS      5 // x+1 to one.  IE 1 = 1+1 to 1 or 1:2 chance.
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


sparkle::sparkle(neoPixel* inLEDs, uint16_t inIndex, colorObj* inColor) {

  LEDs = inLEDs;
  index = inIndex;
  ourColor.setColor(inColor);
  setTime(random(SPARKLE_LOW_LIFE,SPARKLE_LIFE));
}


sparkle::~sparkle(void) { LEDs = NULL; }


void sparkle::erase(void) {

  if (LEDs) {
    LEDs->setPixelColor(index, &backColor); // Erase us..
  }
}


void sparkle::draw(void) {

  if (LEDs) {
    if (!ding()) {                              // Live! Be animated!
      backColor = LEDs->getPixelColor(index);   // Copy host color.
      LEDs->setPixelColor(index, &ourColor);
    }
  }
}


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


sparkles::sparkles(neoPixel* inLEDs) : linkList() { LEDs = inLEDs; }

sparkles::~sparkles(void) { }


void sparkles::addSparkle(uint16_t index, colorObj* sparkleColor) {

  sparkle* newSparkle;
  if (getCount() < MAX_SPARKLES) {  // Lets not blow up the RAM please..
    newSparkle = new sparkle(LEDs, index, sparkleColor);
    addToTop((linkListObj*)newSparkle);
  }
}


void sparkles::erase(void) {

  int       count;
  sparkle*  aSparkle;

  count = getCount();
  for (int i = 0; i < count; i++) {
    aSparkle = (sparkle*)getByIndex(i);
    aSparkle->erase();
  }
  cleanUp();
}


void sparkles::draw(void) {

  int       count;
  sparkle*  aSparkle;

  count = getCount();
  for (int i = count - 1; i >= 0; i--) {
    aSparkle = (sparkle*)getByIndex(i);
    aSparkle->draw();
  }
  cleanUp();
}


void sparkles::cleanUp(void) {

  int       count;
  int       index;
  sparkle*  aSparkle;
  boolean   done;

  count = getCount();
  index = 0;
  done = false;
  while (!done) {
    if (index < count) {
      aSparkle = (sparkle*)getByIndex(index);
      index++;
      if (aSparkle) {                         // Sanity! Check for null.
        if (aSparkle->ding()) {
          unlinkObj((linkListObj*)aSparkle);  // Single linked list. One must unlink first, then delete.
          delete aSparkle;                    // We take care of the memory we allocate.
          done = true;                        // Only kill one. Make 'em sweat.
        }
      } else {
        done = true;                          // Was handed a null, I'm outta' here!
      }
    } else {
      done = true;                            // Gone past our count.
    }
  }
}



// **************************************
// ************ Main program ************
// **************************************


neoPixel lites(NUM_LEDS, PIXEL_PIN);

sparkles theSparkles(&lites);

colorObj backColor;
colorObj sparkleColor[SPARKLE_COLORS];
int       maxCIndex;
void setup() {

  int i;
  
  Serial.begin(9600);
  i=0;
  //for(int j=0;j<5;j++) sparkleColor[i++].setColor(LC_WHITE);
  
  sparkleColor[i++].setColor(LC_RED);
  /*
  sparkleColor[i++].setColor(LC_GREEN);
  sparkleColor[i++].setColor(LC_BLUE);
  sparkleColor[i++].setColor(LC_YELLOW);
  sparkleColor[i++].setColor(LC_PURPLE);
  sparkleColor[i++].setColor(LC_LAVENDER);
  sparkleColor[i++].setColor(LC_ORANGE);
  sparkleColor[i++].setColor(LC_MAGENTA);
  */
  
  maxCIndex = i-1;
  
  backColor.setColor(&white);
  //backColor.blend(&black, DARK_PERCENT);
  
  lites.begin();
  lites.setAll(&backColor);
  lites.show();
}


void loop() {

  colorObj  aColor;

  theSparkles.erase();
  lites.setAll(&backColor);
  if (!random(0, SPARKLE_ODDS)) {
    //theSparkles.addSparkle(random(0, NUM_LEDS - 1), &(sparkleColor[random(0,maxCIndex)]));
    theSparkles.addSparkle(random(0, NUM_LEDS - 1), &red);
  }
  theSparkles.draw();
  lites.show();
}
