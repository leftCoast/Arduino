#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>

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

//#include <adafruit_1431_Obj.h>
#include <adafruit_1947_Obj.h>
//#include <bmpObj.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <screen.h>

label message("Woo hoo!!?",2);
drawObj button(20, 60, 40,20,true);
int clickCount = 0;

void (*callbackPtr)(void) = &callback;

void setup() {
  
   Serial.begin(9600);
   if (initScreen(ADAFRUIT_1947,INV_PORTRAIT)) {
     Serial.println("Got screen.");
   } else {
     Serial.println("Scrren failed, halting program.");
     while(true);                  //  kill the program.
   }
   screen->fillScreen(&blue);
   message.setLocation(20,20);
   message.setColors(white);
   message.draw();

   button.draw();
   Serial.println("Setting callback");
   button.setCallback(&callback);
   Serial.println("Callback should be set.");
   viewList.addObj(&button);
}

void callback(void) {

  Serial.println("Callback!");
  message.setValue(clickCount++);
  message.draw();
}

void loop() {
  idle();

}
