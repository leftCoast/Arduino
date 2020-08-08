#include "adafruit_1947_Obj.h"
#include "screen.h"
#include "eventMgr.h"
#include "flasher.h"
#include "label.h"
#include "colorRect.h"



label* gLabel;          // Global label pointer. Global so we can fnid it later.


// callback, I HATE callbacks, but other love em. Hers'an example..
void firstButton(void) {

   gLabel->setValue("Howdy!");
}

// Another callback..
void secondButton(void) {

   gLabel->setValue("Well fine!");
}


// Your setup() gets quite large. This is where we not only stat up the hardware, but typically
// we also populate the screen with its drawObj(s).
void setup() {

   // First thing, lets set up and get our stuff running.
   // *********
   
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {  // If we can't initialze our display..
      Serial.println("NO SCREEN!");                               // Send a error message to the serial port.
      Serial.flush();                                             // Teensy's don'e block on Serial.print calls. Force it!
      while(true);                                                // Lock here forever. No point going on.
   }
   screen->fillScreen(&black);                                    // Should be running, Set a color to see if it is.                
   ourEventMgr.begin();                                           // Kickstart our event manager.


   // Next we populate our screen with the items it needs to display.
   // *********
   
   flasher*    aFlasher;                                          // Setup a pointer for our flasher objects.
   label*      aLabel;                                            // And for some labels.
   colorRect*  aRect;                                             // And color rects to use as buttons.

   
   // We are ging to flash a some rectangles. We'll create the "off" color for them.

   colorObj blinkOffcolor;                                        // Define a colorObj.
   blinkOffcolor.setColor(&black);                                // Set this colorObj to black.
   blinkOffcolor.blend(&white,10);                                // Mix in 10% white to lighten it.
   
   aFlasher = new flasher(10,10,10,10,&red,&blinkOffcolor);       // We setup a red flashing rectangle. (xPos,yPos,width,length,onColor,offColor)
   aFlasher->setOnOff(true);                                      // Fire up the flasher.
   viewList.addObj(aFlasher);                                     // Give the flasher to the gloabl viewList to manage.

   aFlasher = new flasher(10,25,10,10,&green,&blinkOffcolor);     // We setup a green flashing rectangle. (xPos,yPos,width,length,onColor,offColor)
   aFlasher->setPeriod(250);                                      // We'll set the period at 250 Ms. Faster than the default.
   aFlasher->setPulse(150);                                       // We'll set the pulse to 125 Ms. Faster than the default.
   aFlasher->setOnOff(true);                                      // Fire up this flasher.
   viewList.addObj(aFlasher);                                     // Give the flasher to the gloabl viewList to manage.

   // NOTE : We created these flashers on the heap, as we must. Then, we just tossed the objects into viewList and dropped the pointers.
   // Typically this is the way you populate your display. If you need to interact with one of these objects, you have to keep a pointer
   // to it around so you can access it later. If you plan on deleting one of these objects, you also need to keep a pinter to it is so
   // you can do that.
   //
   // This is a special case using only a single screen. Later, when you decide you want multiple screens (called "panels") you will
   // probaby never deal with deleting drawObj(s). (Everything we are putting on the display are drawObj(s)) This is because, when you
   // swap panels, the disposal of all the drawObj(s) is handled automatically.
   
    
   // Lets add some labels.
   // *********
   
   aLabel = new label(40,300,160,8,"Screen bottom");              // Create a label for the bottom of the screen. (xPos,yPox,width,height,Initial text)
   aLabel->setJustify(TEXT_CENTER);                               // We'll center it to make it look good..
   aLabel->setColors(&white);                                     // Color the text white. No backgroud.
   viewList.addObj(aLabel);                                       // Give the label to the gloabl viewList to manage.

   aLabel = new label(70,84,100,8,"Say Hi :");                   // Create a label for button one.  (xPos,yPox,width,height,Initial text)
   aLabel->setJustify(TEXT_RIGHT);                                // We'll right justify it to make it look good..
   aLabel->setColors(&white);                                     // Color the text white. No backgroud.
   viewList.addObj(aLabel);                                       // Give the label to the gloabl viewList to manage.

   aLabel = new label(70,124,100,8,"Something else :");           // Create a label for button two.  (xPos,yPox,width,height,Initial text)
   aLabel->setJustify(TEXT_RIGHT);                                // We'll right justify it to make it look good..
   aLabel->setColors(&white);                                     // Color the text white. No backgroud.
   viewList.addObj(aLabel);                                       // Give the label to the gloabl viewList to manage.
   
   // Lets add the global label.
   // *********

   colorObj aColor(LC_LIGHT_BLUE);                                 // Another way to start off a color.
   
   aColor.blend(&red,40);                                          // And we'll give this a splash of red.
   gLabel = new label(40,10,160,8,"Editable lable");               // Create a label for the top of the screen (xPos,yPox,width,height,Initial text).
   gLabel->setJustify(TEXT_CENTER);                                // We'll center it to make it look good.
   gLabel->setColors(&aColor,&black);                              // Color the text white. Black backgroud.
   viewList.addObj(gLabel);                                        // Give the label to the gloabl viewList to manage.


   // OK simple buttons..
   // *********

   aRect = new colorRect(180,80,40,20,-2);                        // Create our first button. (xPos,yPox,width,height,inset) Inset is the optinal border. - for outset.
   aRect->setColor(LC_ORANGE);                                    // Set the color to orange.
   aRect->setEventSet(fullClick);                                 // We want the classic full click. (Simple version, works great.)
   aRect->setCallback(firstButton);                               // And we'll set a "gasp" callback. I typically never use callbacks, but there availible.
   viewList.addObj(aRect);                                        // Give the button to the gloabl viewList to manage.

   aRect = new colorRect(180,120,40,20,-2);                       // Create our second button. (xPos,yPox,width,height,inset).
   aRect->setColor(LC_CYAN);                                      // Set the color to cyan.
   aRect->setEventSet(fullClick);                                 // Again the classic full click.
   aRect->setCallback(secondButton);                              // And this button's callback.
   viewList.addObj(aRect);                                        // Give the button to the gloabl viewList to manage.

   // Everything is set up. there is basically nothing left to do. Once you have it set up it all runs by itslef in the
   // background. That is what the call to idle() is for. And why you should NEVR use delay() once you start down this path.
   // Actually I do use delay, but for maybe a milisecond. And I have massive guilt whenever I do.
   //
   // What you have here is just some of the basic building blocks for drawObj(s) that can go on the screen. Typically these are
   // just starting points of what you actualy want on the screen. Every drawObj has a virtual doAction() method you can fill out
   // to have it do things when clicked on. They also have a virtual drawSelf() method that you will fill out to control the look
   // of your draawObj(s). Including the ability do use .bmp files from an SD drive.
}


void loop() {

   idle();     // Alwas call idle() in your loop.
               // And then whatever else you'd like to call..
}
