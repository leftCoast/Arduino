

#include <colorObj.h>           // Base color handler. Works for Our NeoPixle stuff too.
#include <idlers.h>             // idlers get their own private time to do things. DON'T BLOCK.
#include <lists.h>              // Link list building tool kit.
#include <mapper.h>             // Float mappers & multi mappers. Used by colorObj for gradients & blends.
#include <multiMap.h>           // Like I said, multi mappers for fancy stuff.
#include <timeObj.h>            // Timers the can be reset and watched.

#include <adafruit_1947_Obj.h>  // The glue code between the Adafruit product# 1947 and the windowing system.
#include <displayObj.h>         // The Base display command object.
#include <drawObj.h>            // The base drawing object. Inheit this for making your own things for the screen. Also the refresh and drawing management is in here.
#include <label.h>              // A simple label object for the screen.
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
    screen->fillCircle(x,y,height,&onColor);
  } else {
    screen->fillCircle(x,y,height,&offColor);
  }
}

// *********** Meanwile, back to the main program ***********



label message("A Label",2);   // A label is all about just getting text and numbers to the screen. 
drawObj button;               // drawObj can be used as a crude button. Best to inherit them to make more interesting things.
scrBlinker blinker(250);      // An example of inheriting some goodies for an automatic blinker.
scrBlinker blinker2(500);

int clickCount = 0;           // Something for the button to change.

// Here's how you hook it all up.
void setup() {

    colorObj  backColor;          // We'll build up a nice background color.
    colorObj  liteOnColor;        // These next two are for setting up the blinker colors.
    colorObj  liteOffColor;
    
   // Initalize the screen hardware.
   Serial.begin(9600);
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
     Serial.println(F("Screen failed, halting program."));
     while(true);                  //  kill the program.
   }
   Serial.println("Got screen.");
   Serial.flush();
   // Set up the screen background.
   backColor.setColor(&blue);
   backColor.blend(&green,50);
   screen->fillScreen(&backColor);
   Serial.println("Screen filled");
   Serial.flush();
   
   // Set up our label.
   message.setColors(&white,&backColor);
   viewList.addObj(&message);

   // Now for the button.
   //button.clickable(true);
   button.setEventSet(fullClick);
   button.setSize(45,30);
   button.setLocation(100,100);
   button.setCallback(callback);
   viewList.addObj(&button);

   // Now our little custom blinker.
   liteOnColor.setColor(&yellow);
   liteOnColor.blend(&white,20);
   liteOffColor.setColor(&yellow);
   liteOffColor.blend(&black,70);
   blinker.setColors(&liteOnColor,&liteOffColor);
   blinker.setSize(10,10);
   blinker.setLocation(200,16);
   viewList.addObj(&blinker);

   // And another..
   blinker2.setSize(10,10);
   blinker2.setLocation(215,16);
   blinker2.setColors(&green,&backColor);
   viewList.addObj(&blinker2);
   
   ourEventMgr.begin();
}


// The callback for the button.
void callback(void) {
  message.setValue(++clickCount);
}


// And, of course the loop() function. Ready for your code. 
// Just let the first line be idle();
// Try NOT to use Delay().
void loop() {
  
  idle();
}
