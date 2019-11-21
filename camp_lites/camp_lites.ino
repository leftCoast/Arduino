#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      75
#define SPARKLE_LOW_LIFE  5
#define SPARKLE_LIFE      15
#define SPARKLE_ODDS      80 // to one.
#define FLASH_ODDS        4000 // to one.
#define FLASH_MS          50
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
colorObj sparkleColor;
colorObj flashColor;

void setup() {

  Serial.begin(9600);

  sparkleColor.setColor(&white);
  backColor.setColor(&blue);
  backColor.blend(&black, DARK_PERCENT);
  flashColor.setColor(&yellow);
  flashColor.blend(&white,40);
  
  lites.begin();
  lites.setAll(&backColor);
  lites.show();
}


void loop() {

  colorObj  aColor;

  theSparkles.erase();
  if (!random(0,FLASH_ODDS)) {
    lites.setAll(&flashColor);
    lites.show();
    lites.setAll(&black);
    delay(random(0,FLASH_MS));
    lites.show();
  }
  lites.setAll(&backColor);
  if (!random(0, SPARKLE_ODDS)) {
    theSparkles.addSparkle(random(0, NUM_LEDS - 1), &sparkleColor);
  }
  theSparkles.draw();
  lites.show();
  //delay(LOOP_DELAY);
}
