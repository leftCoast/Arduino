#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>

#include <colorObj.h>           // Base color handler. Works for Our NeoPixle stuff too.
#include <idlers.h>             // idlers get their own private time to do things. DON'T BLOCK.
#include <lists.h>              // Link list building tool kit.
#include <mapper.h>             // Float mappers & multi mappers. Used by colorObj for gradients & blends.
#include <multiMap.h>           // Like I said, multi mappers for fancy stuff.
#include <runningAvg.h>
#include <timeObj.h>            // Timers the can be reset and watched.

#include <adafruit_1431_Obj.h>  // The glue code between the Adafruit product# 1947 and the windowing system.
#include <displayObj.h>         // The Base display command object.
#include <drawObj.h>            // The base drawing object. Inheit this for making your own things for the screen. Also the refresh and drawing management is in here.
#include <label.h>              // A simple label object for the screen.
#include <lineObj.h>            // A line.
#include <screen.h>             // The screen ties it all together. This is who connects the hardware drivers to the drawing enviroment.


// ******* Example : Make an outomatic blinker ********

class scrBlinker : public drawObj, public timeObj, public idler {

public:
             scrBlinker(float inMs=500);
  
        void setColors(colorObj* inOnColor,colorObj* inOffColor);
virtual void idle(void);
virtual void drawSelf();
  
  colorObj onColor;
  colorObj offColor;
  boolean  liteOn;
};


scrBlinker::scrBlinker(float inMs) {
  
  onColor.setColor(&red);
  offColor.setColor(&black);
  liteOn = false;
  setTime(inMs); 
}


void scrBlinker::setColors(colorObj* inOnColor,colorObj* inOffColor) {

  onColor.setColor(inOnColor);
  offColor.setColor(inOffColor);
  needRefresh = true;
}


void scrBlinker::idle(void) {

  if (ding()) {
    liteOn = !liteOn;
    start();
    needRefresh = true;
  }
}


void  scrBlinker::drawSelf(void) {

  hookup();           
  if (liteOn) {
    screen->fillCircle(locX,locY,height,&onColor);
  } else {
    screen->fillCircle(locX,locY,height,&offColor);
  }
}

// *********** Meanwile, back to the main program ***********


#define NUM_LINES 1           // More than one gets memory tight. But 10 still works on UNOs.

label message("-----",1);   // A label is all about just getting text and numbers to the screen.
label message2("-----",1);
lineObj line[NUM_LINES];      // lineObj is all  about lines. Lets grab a bunch!
scrBlinker blinker(250);      // An example of inheriting some goodies for an automatic blinker.
scrBlinker blinker2;

int clickCount = 0;           // Something for the button to change.
runningAvg smoother(10);

// Here's how you hook it all up.
void setup() {

    colorObj  backColor;          // We'll build up a nice background color.
    colorObj  liteOnColor;        // These next two are for setting up the blinker colors.
    colorObj  liteOffColor;


   
   // Initalize the screen hardware.
   //Serial.begin(9600);
   if (!initScreen(ADAFRUIT_1431,PORTRAIT)) {
     //Serial.println(F("Screen failed, halting program."));
     while(true);                  //  kill the program.
   }

   // Set up the screen background.
   backColor.setColor(&blue);
   backColor.blend(&green,50);
   screen->fillScreen(&backColor);

   label tempMessage("Raw:",1);
   tempMessage.setColors(&white,&backColor);
   tempMessage.setJustify(TEXT_LEFT);
   tempMessage.setLocation(10,16);
   tempMessage.draw();
   tempMessage.setValue("Ave:");
   tempMessage.setLocation(10,32);
   tempMessage.draw();
   
   // Set up our label.
   message.setColors(&white,&backColor);
   message.setJustify(TEXT_RIGHT);
   message.setLocation(35,16);
   viewList.addObj(&message);

   // Set up our second label.
   message2.setColors(&white,&backColor);
   message2.setJustify(TEXT_RIGHT);
   message2.setLocation(35,32);
   message2.setPrecision(0);
   viewList.addObj(&message2);
   
   // Some nice lines.
   for(byte i=0;i<NUM_LINES;i++) {
     line[i].setEnds(10,45,118,45+(4*i));
     viewList.addObj(&(line[i]));
   }
   
   // Now our little custom blinker.
   liteOnColor.setColor(&yellow);
   liteOnColor.blend(&white,20);
   liteOffColor.setColor(&yellow);
   liteOffColor.blend(&black,70);
   blinker.setColors(&liteOnColor,&liteOffColor);
   blinker.setSize(10,10);
   blinker.setLocation(80,16);
   viewList.addObj(&blinker);

   // And another..
   blinker2.setSize(10,10);
   blinker2.setLocation(95,16);
   viewList.addObj(&blinker2);
   
}


// And, of course the loop() function. Ready for your code. 
// Just let the first line be idle();
// Try NOT to use Delay().
void loop() {

  int   potVal;
  float averageVal;
  
  idle();
  potVal = analogRead(A5);
  potVal = potVal>>1;
  averageVal = smoother.addData(potVal);
  message.setValue(potVal<<1);
  message2.setValue(averageVal*2);
}
