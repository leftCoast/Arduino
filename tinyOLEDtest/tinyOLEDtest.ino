#include <adafruit_684_Obj.h>
#include <Adafruit_GFX.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <drawObj.h>
#include <screen.h>
#include <label.h>
#include <debug.h>
#include <colorRect.h>
#include <bmpPipe.h>

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   4


class percent : public label {

  public:
                percent(int x,int y);
  virtual       ~percent(void);

          void  setPercent(float percent);
};


percent::percent(int x,int y)
  : label(x,y,96-x,8,"---",2) {
  setColors(&white,&black); 
}


percent::~percent(void) { }


void percent::setPercent(float percent) {

  int   intPercent;
  char  pText[6];
  
  if (percent<0) {                         
    strcpy(pText,"---");
  } else {
    intPercent = round(percent);              // Comes in as a percent float.
    if (intPercent<10) {                      // If its 1-9..
      snprintf (pText,5,"  %d",intPercent);   // put in two spaces.
    } else if (intPercent<100) {              // Else if its less than 100..               
      snprintf (pText,5," %d",intPercent);    // One space.
    } else {                                  // Else.. (>99) 
      snprintf (pText,5,"%d",intPercent);     // No spaces.
    }
  }
  setValue(pText);
}




// Globals

//float     moisture;                             // Calculated moisture %
float     tempC;                                // Temp.
uint16_t  capread;                              // Capacitence.
//enum      weDo;
//weDo      weAre;                                // Current state.
percent* limit;
percent* moisture;

timeObj timer(1000);
timeObj timer2(5000);
bool    crash;

void setup() {

  crash = false;
  
  if (!initScreen(ADAFRUIT_684,OLED_CS,OLED_RST,PORTRAIT)) {
    while(true);
  }

  SD.begin(OLED_SDCS);
  
  screen->fillScreen(&black);
  
  int yPos = 0;

  limit = new percent(5,yPos);
  viewList.addObj(limit);

  moisture = new percent(53,yPos);
  viewList.addObj(moisture);
  
  label* readings = new label(41,yPos,96,16,"/",2);
  readings->setColors(&white);
  viewList.addObj(readings);

  yPos = yPos + 20;
  label* key = new label(9,yPos,96,16,"Limit/Reading",1);
  key->setColors(&white);
  viewList.addObj(key);

  yPos = yPos + 20;                      //"100/100"
  label* statText = new label(0,yPos,96,16,"Watering",2);
  statText->setColors(&white);
  viewList.addObj(statText);

  limit->setPercent(21);
  timer.start();
  timer2.start();
}

/*
"Starting" 
"Watering"
"Reading"
"Soaking"
*/


void loop() {

  idle();
  if (!crash && timer.ding()) {
    moisture->setPercent((int)random(-1,101));
    timer.start();
  }
  if (timer2.ding()) {
    bmpPipe bomb(rect(0,0,96,64));
    bomb.openPipe("/bomb9664.bmp");
    bomb.drawBitmap(0,0);
    crash = true;
  }
}
