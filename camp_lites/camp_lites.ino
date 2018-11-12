#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      75
#define SPARKLE_LOW_LIFE  5
#define SPARKLE_LIFE      30
#define SPARKLE_ODDS      40 // to one.
#define MAX_SPARKLES      15



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

    void  addSparkle(uint16_t index, colorObj* flashColor);
    void  erase(void);
    void  cleanUp(void);
    void  draw(void);

    neoPixel* LEDs;
};


sparkles::sparkles(neoPixel* inLEDs) : linkList() { LEDs = inLEDs; }

sparkles::~sparkles(void) { }


void sparkles::addSparkle(uint16_t index, colorObj* flashColor) {

  sparkle* newSparkle;
  if (getCount() < MAX_SPARKLES) {  // Lets not blow up the RAM please..
    newSparkle = new sparkle(LEDs, index, flashColor);
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
          unlinkObj((linkListObj*)aSparkle);
          delete aSparkle;                    // We take care of memory we allocated.
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
colorObj flashColor;

void setup() {

  Serial.begin(9600);

  flashColor.setColor(&white);
  backColor.setColor(&blue);
  backColor.blend(&black, DARK_PERCENT);

  lites.begin();
  lites.setAll(&backColor);
  lites.show();
}


void loop() {

  colorObj  aColor;

  theSparkles.erase();
  lites.setAll(&backColor);
  if (!random(0, SPARKLE_ODDS)) {
    theSparkles.addSparkle(random(0, NUM_LEDS - 1), &flashColor);
  }
  theSparkles.draw();
  lites.show();
  //delay(LOOP_DELAY);
}
