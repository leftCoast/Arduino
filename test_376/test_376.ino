#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

//#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
//#include <PulseOut.h>
#include <runningAvg.h>
//#include <servo.h>
#include <timeObj.h>

//#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
//#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <adafruit_376_Obj.h>  // ADAFRUIT_376

//#include <bmpObj.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

//label hello("hello World!");
drawObj aRect(40,50,70,20,true);

void setup() {

  Serial.begin(9600);
  if (!initScreen(ADAFRUIT_376,INV_PORTRAIT)) {
    while(true); // Kill the process.
  }
  screen->fillScreen(&green);
  //screen->setCursor(40,40);
  //screen->setTextColor(&black,&green);
  //screen->drawText("whatever!");
  //viewList.addObj(&hello);
  //hello.setColors(&black,&green);
  //hello.setLocation(40,50);
  viewList.addObj(&aRect);
  aRect.drawSelf();
  //screen->fillRect(40, 70, 70, 20, &black); // Default draw.
  //screen->drawRect(40, 70, 70, 20, &white);
}


void loop() {
  idle();
}
