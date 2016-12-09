#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

class neoPixelRev : public neoPixel {
   
      public:

  neoPixelRev(uint16_t n, uint8_t p,bool inRev=false);
  ~neoPixelRev();
  
  void     setPixelColor(uint16_t n,colorObj* inColor);
  colorObj getPixelColor(uint16_t n);

  bool reverse;
};


neoPixelRev::neoPixelRev(uint16_t n, uint8_t p,bool inRev) : neoPixel(n,p) { reverse = inRev; }


neoPixelRev::~neoPixelRev() {  }

  
void neoPixelRev::setPixelColor(uint16_t n,colorObj* inColor) {

    if (reverse) {
      neoPixel::setPixelColor((numPixels()-1)-n,inColor);
    } else {
      neoPixel::setPixelColor(n,inColor);
    }
  }

  
colorObj neoPixelRev::getPixelColor(uint16_t n) {

  if (reverse) {
      return neoPixel::getPixelColor((numPixels()-1)-n);
  } else {
      return neoPixel::getPixelColor(n);
  }
}


  
neoPixelRev SLC(8, 8,true);

colorObj randomColor(void) {

  byte r;
  byte g;
  byte b;
  colorObj acolor;

  r = random(0, 256);
  g = random(0, 256);
  b = random(0, 256);
  acolor.setColor(r, g, b);
  return acolor;
}

// **************** Don't look up here *************

int count;

void setup() {

  int i;
  colorObj acolor;

  SLC.begin();
  SLC.setAll(&red);
  SLC.show();
  count = 0;
}


void colorStep(colorObj* incolor) {

  colorObj acolor;

  for (int i = 0; i < 8; i = i + 1) {
    if (i == 7) {
      SLC.setPixelColor(i, incolor);
    }
    else {
      acolor = SLC.getPixelColor(i + 1);
      SLC.setPixelColor(i, &acolor);
    }
  }
}


void loop() {

  colorObj acolor;

  if (count >= 0 && count <3) {
    colorStep(&white);
  } else {
    colorStep(&red);
  }
  count++;
  if (count >= 7) {
    count = 0;
  }
  SLC.show();
  delay(50);
  

}

