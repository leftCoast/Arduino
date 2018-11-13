#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>

#define NUM_LEDS      150
#define PIXEL_PIN     2
#define NUM_COLORS    6
#define DARK_PERCENT  75
#define FADE_PERCENT  50.999
#define CHANGE_DELAY  50
#define NUM_SPACES    12
#define SPARKLE_ODDS  50  // 1:X odds againced.
#define REV_ODDS      3   // 1:X same thing
#define SPARKLE_SLOW  2
#define SPARKLE_FAST  5



// **************************************
// ************** sparkle ***************
// **************************************


class sparkle : public linkListObj {

  public:
    sparkle(neoPixel * inLEDs, uint16_t inIndex);
    ~sparkle(void);

    void erase(void); // Put the saved color back.
    void draw(void);  // Save off the base color then do whatever.

    neoPixel* LEDs;
    uint16_t  numLEDs;
    uint16_t  index;
    int       cIndex;
    int       stepSize;
    colorObj  backColor;
    boolean   alive;
};


sparkle::sparkle(neoPixel* inLEDs, uint16_t inIndex) {

  boolean rev;

  if (inLEDs) {
    LEDs = inLEDs;
    index = inIndex;
    numLEDs = LEDs->numPixels();
    cIndex = random(0, 7);
    stepSize = random(2, 8);
    rev = index > 0;
    if (rev) {
      stepSize = -stepSize;
    }
    alive = true;
  } else {
    alive = false;
  }
}


sparkle::~sparkle(void) { }

void sparkle::erase(void) {

  if (index < numLEDs) {                    // unsigned. So too small or too big is all too big.
    LEDs->setPixelColor(index, &backColor); // Erase us..
  }
}


void sparkle::draw(void) {

  if (alive) {                                // Live! Be animated!
    index = index + stepSize;                 // jump.
    if (index < numLEDs) {                    // unsigned. So too small or too big is all too big.
      backColor = LEDs->getPixelColor(index); // Copy new host color.
      switch (cIndex) {
        case 0 : LEDs->setPixelColor(index, &red); break;
        case 1 : LEDs->setPixelColor(index, &white); break;
        case 2 : LEDs->setPixelColor(index, &green); break;
        case 3 : LEDs->setPixelColor(index, &magenta); break;
        case 4 : LEDs->setPixelColor(index, &yellow); break;
        case 5 : LEDs->setPixelColor(index, &blue); break;
        case 6 : LEDs->setPixelColor(index, &cyan); break;
      }
    } else {
      alive = false;                          // We're done, time to die.
    }
  }
}


// **************************************
// ************** sparkles **************
// **************************************

class sparkles : public linkList {

  public:
    sparkles(neoPixel* inLEDs);
    ~sparkles(void);

    void  addSparkle(uint16_t index);
    void  erase(void);
    void  cleanUp(void);
    void  draw(void);

    neoPixel* LEDs;
};


sparkles::sparkles(neoPixel* inLEDs) : linkList() {
  LEDs = inLEDs;
}

sparkles::~sparkles(void) {  }


void sparkles::addSparkle(uint16_t index) {

  sparkle* newSparkle;

  newSparkle = new sparkle(LEDs, index);
  addToTop((linkListObj*)newSparkle);
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
  for (int i = count-1; i >=0; i--) {
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
        if (!(aSparkle->alive)) {
          unlinkObj((linkListObj*)aSparkle);
          delete aSparkle;
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

sparkles theSparkles(&lites); // May not work..

colorObj colors[NUM_COLORS];

int       colorIndex;
int       lastColorIndex;
int       liteIndex;
int       spaceCount;
colorObj  fadeColor;

void setup() {

  colorObj  aColor;
  //Serial.begin(9600);
  lites.begin();

  colors[0].setColor(&red);
  colors[0].blend(&black, 0);

  colors[1].setColor(&white);

  colors[2].setColor(&green);
  colors[2].blend(&black, 0);

  colors[3].setColor(&magenta);
  colors[3].blend(&black, 0);

  colors[4].setColor(&blue);
  colors[4].blend(&black, 0);

  colors[5].setColor(&yellow);
  colors[5].blend(&black, 0);

  for (int i = 0; i < NUM_COLORS ; i++) {
    if (i != 1) colors[i].blend(&black, DARK_PERCENT);
  }

  spaceCount = 0;
  for (int i = NUM_LEDS-1; i >=0 ; i--) {
    if (spaceCount) {
      fadeColor.blend(&black, FADE_PERCENT);
      lites.setPixelColor(i, &fadeColor);
    } else {
      aColor = chooseNonWhite();
      lites.setPixelColor(i, &aColor);
      fadeColor.setColor(&aColor);
    }
    incSpaceCount();
  }
  lites.show();
}


void incSpaceCount(void) {

  spaceCount++;
  if (spaceCount > NUM_SPACES) {
    spaceCount = 0;
  }
}


colorObj chooseColor(void) {

  int colorIndex = random(0, NUM_COLORS);
  if (colorIndex == lastColorIndex) colorIndex = random(0, NUM_COLORS);
  if (colorIndex == 1) colorIndex = random(0, NUM_COLORS);
  lastColorIndex = colorIndex;
  return colors[colorIndex];
}


colorObj chooseNonWhite(void) {

  int colorIndex = random(0, NUM_COLORS);
  if (colorIndex == lastColorIndex) colorIndex = random(0, NUM_COLORS);
  while (colorIndex == 1) colorIndex = random(0, NUM_COLORS);
  lastColorIndex = colorIndex;
  return colors[colorIndex];
}


void loop() {

  colorObj  aColor;

  theSparkles.erase();
  lites.shiftPixels();
  if (spaceCount) {
    fadeColor.blend(&black, FADE_PERCENT);
    lites.setPixelColor(0, &fadeColor);
  } else {
    //aColor = chooseNonWhite();
    aColor.setColor(&white);
    lites.setPixelColor(0, &aColor);
    fadeColor.setColor(&aColor);
  }
  incSpaceCount();
  if (!random(0, SPARKLE_ODDS)) {
    if (!random(0, REV_ODDS)) {
      theSparkles.addSparkle(lites.numPixels() - 1);
    } else {
      theSparkles.addSparkle(0);
    }
  }
  theSparkles.draw();
  lites.show();
  delay(CHANGE_DELAY);
}
