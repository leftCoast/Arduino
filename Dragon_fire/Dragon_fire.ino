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


#define FAST_DELAY    20// How long is the timer?
#define DARK_PERCENT  75
#define MAX_LIGHTS    120
#define FIRELINE_LEN  24

colorMapper flameMapper(&red,&black);
mapper indexToColor(1,FIRELINE_LEN,0,100);


class fireLine : public multiColorLine {
  
  public:
    fireLine(int inLength);
    ~fireLine(void);

    virtual colorObj calcColor(int index,int i);

    colorObj fireHead;
    
};

fireLine::fireLine(int inLength) : multiColorLine(inLength) { 

  fireHead.setColor(&yellow);
  fireHead.blend(&black,70);
  }


fireLine::~fireLine(void) { }


colorObj fireLine::calcColor(int index,int i) {

    colorObj fireTail;
    float tailPercent;
    colorObj aColor;
    
    if (i==0) {
      int r = random(1,100);
      if (r>95)
        aColor.setColor(&white);
      else
        aColor = fireHead;
    } else {
      tailPercent = indexToColor.Map(i);
      aColor = flameMapper.Map(tailPercent);
      aColor.blend(&black,70);
    }
    return aColor;
}



colorObj baseColor(BLACK);
neoPixel lightBar(MAX_LIGHTS, 8);
timeObj timer(FAST_DELAY);
int runningDark;
int r; 
byte index;

fireLine fireLine1(FIRELINE_LEN);
fireLine fireLine2(FIRELINE_LEN);
fireLine fireLine3(FIRELINE_LEN);

void setup() {

  lightBar.begin();
  setAll(&baseColor);
  runningDark = 100;
  index = 0;
}


void setAll(colorObj* color) {

  for (int i = 0; i < MAX_LIGHTS; i++) {
    lightBar.setPixelColor(i, color);
  }
  //lightBar.show();   // Lest show it!
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
  if (runningDark>DARK_PERCENT) runningDark--;
}


void showFlame(void) {

  setAll(&baseColor);
  fireLine1.setLights(&lightBar,index);
  fireLine2.setLights(&lightBar,index-(FIRELINE_LEN*2));
  fireLine3.setLights(&lightBar,index-(FIRELINE_LEN*4));
  index++;
  if (index>=MAX_LIGHTS+FIRELINE_LEN) index = 0;
  lightBar.show();
}


void loop() {

  idle();
  r = random();     // Randomize the randoms.
  if (timer.ding()) {
    //randomColors();
    showFlame();
    timer.stepTime();
  }
}



