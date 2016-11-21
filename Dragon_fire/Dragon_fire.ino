#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>

#include <neoPixel.h>
#include <chainPixels.h>
#include <liteLine.h>

#include "fireLine.h"

#define FAST_DELAY    1000// How long is the timer?
#define DARK_PERCENT  75
#define MAX_LIGHTS    120
#define FIRELINE_LEN  24
#define MAX_FLAMES    6

class autoFlame;

autoFlame* flameList[MAX_FLAMES];

void spawn(void) {

  
}


class autoFlame {

  autoFlame(fireLine* inFireLine);

  public:
          void    incIndex(int inIndex);
          void    setSpawnPoint(int inSpawn);  // 0 means stop.
          void    drawSelf(void);
          void    bumpIndex(void);
          boolean busy();
          
  fireLine* mFireLine;
  int       index;
  int       lastIndex;
  int       spawnPoint;
};


autoFlame::autoFlame(fireLine* inFireLine) {
  
  mFireLine = inFireLine;
  lastIndex = mFireLine->getNumPixels() + mFireLine->getLength() - 2; 
  index = lastIndex+1;                                             // puts us off the end.
  spawnPoint = mFireLine->getLength();                             // nice defualt.
}


void autoFlame::setSpawnPoint(int inSpawn) { spawnPoint = inSpawn; }


void autoFlame::drawSelf(void) { if (mFireLine) mFireLine->setLights(index); }

    
void autoFlame::bumpIndex(void) {
  
    index++; 
    if (index==spawnPoint) spawn();
}


boolean autoFlame::busy(void) { return index > lastIndex; }



colorObj baseColor(BLACK);
neoPixel lightBar(MAX_LIGHTS, 8);
timeObj timer(FAST_DELAY);
int runningDark;
int r; 
byte index;

fireLine fireLine(&lightBar,FIRELINE_LEN);


void setup() {

  lightBar.begin();
  setAll(&baseColor);
  fireLine.begin();
  for (byte i;i<MAX_FLAMES;i++) {
    flameList[i] = NULL;
  }
  runningDark = 100;
  index = 0;
}


void setAll(colorObj* color) {

  for (int i = 0; i < MAX_LIGHTS; i++) {
    lightBar.setPixelColor(i, color);
  }
}



void randomColors(void) {

   byte r;
   byte g;
   byte b;
   colorObj  rColor;
    
  for (int i = 0;i<MAX_LIGHTS;i++) {
    r = random(0,256);
    g = random(0,256);
    b = random(0,256);
    rColor.setColor(r,g,b);
    rColor.blend(&black,runningDark);     //Knock down power draw.
    lightBar.setPixelColor(i, &rColor);
  }
  if (runningDark>DARK_PERCENT) runningDark--;
}


void drawFlame(void) {

  setAll(&baseColor);
  fireLine1.setLights(index);
  fireLine2.setLights(index-(FIRELINE_LEN*2));
  fireLine3.setLights(index-(FIRELINE_LEN*4));
  fireLine4.setLights(index-(FIRELINE_LEN*8));
  //fireLine5.setLights(index-(FIRELINE_LEN*16));
  index++;
  if (index>=MAX_LIGHTS+(FIRELINE_LEN*9)) index = 0;
  
}


void loop() {

  //idle();
  r = random();     // Randomize the randoms.
  if (timer.ding()) {
    lightBar.show();
    timer.stepTime();
    //drawFlame();
    randomColors();
  }
}



