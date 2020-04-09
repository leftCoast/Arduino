#include <adafruit_1947_Obj.h>
#include <Adafruit_GFX.h>
#include <colorObj.h>
#include <idlers.h>
#include <drawObj.h>
#include <resizeBuff.h>
#include <screen.h>
#include <bmpPipe.h>
#include "bitmap.h"
#include "offscreen.h"
#include "bmpObj.h"

#define SD_CS     4
#define BEEP_PIN  23
//#define JIM_LEE   "/icons/jimLee32.bmp" // 32 x 32
#define JIM_LEE   "/icons/redSq.bmp" // 32 x 32
//#define JIM_LEE   "/icons/term32.bmp" // 32 x 32
#define KEYPAD    "/icons/keyCap24.bmp" // 24 x 33


bitmap myFace;    // Test the sized version as well.
bmpPipe myPict;
 
void setup() {

  rect      theRect;
  colorObj  aColor;
  
  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
  }

  if (!SD.begin(SD_CS)) {                   // With icons, we now MUST have an SD card.
    Serial.println("NO SD CARD!");
    Serial.flush();
    screen->setCursor(10,10);
    screen->setTextColor(&white);
    screen->setTextSize(2);
    screen->drawText("No SD card.");
    while(true);
  }
  
  if (myFace.setSize(32,32)) {
    Serial.println("Got te RAM!");
  } else {
    Serial.println("No RAM for you!");
    screen->fillScreen(&red);
    while(true);  // LOCK
  }
  screen->fillScreen(&white);
  
  offscreen vPort;                          // Create a virtual screen.                       
  
  theRect.setRect(0,0,32,32);               // Set up go grab the ,bmp image..
  myPict.openPipe(JIM_LEE);                 //
  myPict.setSourceRect(theRect);            //
  
  Serial.println("Filling bitmap..");       // Tell 'em.
  vPort.beginDraw(&myFace);                 // Start capture to my face.
  myPict.drawImage(0,0);                    // Do capture..
  vPort.endDraw();                          // End capture.
  Serial.println("All filled. I think..");  // Tell 'em.
  
  unsigned long startTime;
  unsigned long endTime;
  unsigned long drawTime;
  unsigned long blitTime;
  bmpObj*  aBmpObj = new bmpObj(28,10,32,32,JIM_LEE);
  aBmpObj->begin();
  viewList.addObj(aBmpObj);
  
  
  Serial.println("Drawing image..");
  startTime = micros();
  screen->startWrite();
  myPict.drawImage(60,10);
  screen->endWrite();
  endTime = micros();
  drawTime = endTime - startTime;
  Serial.println(drawTime);

  Serial.println("Blitting image..");
  startTime = micros();
  screen->blit(92,10,&myFace);
  endTime = micros();
  blitTime = endTime - startTime;
  Serial.println(blitTime);
  Serial.print("Time saved : ");Serial.print((drawTime-blitTime)/1000.0);Serial.println(" ms");
  ourEventMgr.begin();
}


void loop() {
  idle();

}
