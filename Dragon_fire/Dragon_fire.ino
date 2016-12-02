#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <blinker.h>

#include <neoPixel.h>
#include <chainPixels.h>
#include <liteLine.h>

#include "fireLine.h"

#define DARK_PERCENT  75

#define NUM_LIGHTS    8//120
#define LED_PIN       8
#define PATTERN_LEN   3//24
#define NUM_PATTERNS  5//10
#define FRAME_DELAY   250//17     // How long between frames.
#define WAIT_DELAY    2000   // How long between flames.


neoPixel lightString(NUM_LIGHTS,LED_PIN);
colorObj patternArray[PATTERN_LEN];
fireLine fireLine(&lightString,NUM_LIGHTS,patternArray,PATTERN_LEN,NUM_PATTERNS);

blinker heartbeat;

void setupPatternArray(void) {

  colorMultiMap flameMapper;
  colorObj  aColor;

  aColor = yellow;
  aColor.blend(&black,50);
  flameMapper.addColor(0,&aColor);

  aColor = yellow;
  aColor.blend(&red,50);
  aColor.blend(&black,50);
  flameMapper.addColor(2,&aColor);

  aColor = red;
  aColor.blend(&black,70);
  flameMapper.addColor(4,&aColor);

  aColor = red;
  aColor.blend(&black,95);
  flameMapper.addColor(PATTERN_LEN-6,&aColor);
  flameMapper.addColor(PATTERN_LEN,&aColor);

  for(byte i=0;i<PATTERN_LEN;i++) {
    patternArray[i] = flameMapper.Map(i);
  }
}

void setupCristmasArray(void) {

  colorMultiMap flameMapper;
  colorObj  aColor;

  aColor = yellow;
  //aColor.blend(&black,70);
  flameMapper.addColor(0,&aColor);

  aColor = yellow;
  aColor.blend(&magenta,50);
  aColor.blend(&black,50);
  flameMapper.addColor(2.5,&aColor);

  aColor = blue;
  aColor.blend(&black,70);
  flameMapper.addColor(4.5,&aColor);

  aColor = green;
  aColor.blend(&black,70);
  flameMapper.addColor(10.5,&aColor);

  aColor = green;
  aColor.blend(&red,50);
  aColor.blend(&black,50);
  flameMapper.addColor(13,&aColor);

  aColor = red;
  aColor.blend(&black,70);
  flameMapper.addColor(16,&aColor);
  
  aColor = red;
  aColor.blend(&black,95);
  flameMapper.addColor(PATTERN_LEN-3,&aColor);
  flameMapper.addColor(PATTERN_LEN,&aColor);

  for(byte i=0;i<PATTERN_LEN;i++) {
    patternArray[i] = flameMapper.Map(i);
  }
}

timeObj frameTimer(FRAME_DELAY);
timeObj waitTimer(WAIT_DELAY);

int   runningDark;
int   r; 
int  index;
int  maxIndex;
bool  runningFlames;

void setup() {

  //Serial.begin(9600);
  heartbeat.setBlink(true);
  maxIndex = fireLine.getMaxIndex();
  lightString.begin();
  setupPatternArray();
  //setupCristmasArray();
  setAll(&black); 
  lightString.show();
  
  runningDark = 100;
  
  index = 0;
  runningFlames = false;
  waitTimer.start();
}




void setAll(colorObj* color) {

  for (int i = 0; i < NUM_LIGHTS; i++) {
    lightString.setPixelColor(i, color);
  }
}



void randomColors(void) {

   byte r;
   byte g;
   byte b;
   colorObj  rColor;
    
  for (int i = 0;i<NUM_LIGHTS;i++) {
    r = random(0,256);
    g = random(0,256);
    b = random(0,256);
    rColor.setColor(r,g,b);
    rColor.blend(&black,runningDark);     //Knock down power draw.
    lightString.setPixelColor(i, &rColor);
  }
  if (runningDark>DARK_PERCENT) runningDark--;
}


void loop() {

  idle();
  r = random();     // Randomize the randoms.
  
  if (runningFlames) {
    if (frameTimer.ding()) {
      lightString.show();
      frameTimer.start();
      setAll(&black);
      fireLine.setLights(index++);
      if (index>maxIndex+1) {     // We already loaded in the last one so we need another to fire it off.
        waitTimer.start();
        runningFlames = false;
      }
    }
  } else {
    if (waitTimer.ding()) {
      index = 0;
      setAll(&black);
      fireLine.setLights(index++);
      runningFlames = true;
      // don't start frame timer, just leave it running for fast startup.
    }
    
  }
}




