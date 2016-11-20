#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>



#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>


#define FAST_DELAY    1025     // How long is the timer?
#define DARK_PERCENT  75

#define MAX_LIGHTS    120
//#define MAX_LIGHTS    8

colorObj baseColor;
blinker aBLinker;
neoPixel lightBar(MAX_LIGHTS, 8);
timeObj timer(FAST_DELAY);
int runningDark;

void setup() {

  baseColor.setColor(&black);
  lightBar.begin();
  setAll(&baseColor);
  runningDark = 100;
}


void setAll(colorObj* color) {

  for (int i = 0; i < MAX_LIGHTS; i++) {
    lightBar.setPixelColor(i, color);
  }
  lightBar.show();   // Lest show it!
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
  lightBar.show();
}




void loop() {

  idle();
  int r = random();     // Randomize the randoms.
  if (timer.ding()) {
    randomColors();
    timer.stepTime();
    if (runningDark>DARK_PERCENT) runningDark--;
  }
}



