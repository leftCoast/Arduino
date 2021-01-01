#include "sparkles.h"

// **************************************
// ************** sparkle ***************
// **************************************


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
