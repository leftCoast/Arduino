#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

bmpPipe bitMap1;

void setup() {
  
  rect  sourceRect;
  
  //Serial.begin(9600);while(!Serial);
  if (!initScreen(ADAFRUIT_1947,PORTRAIT)) {
    while(true); // Kill the process.
  }
  screen->fillScreen(&black);
  if (bitMap1.openPipe("vacPnl.bmp")) {
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.width = 240;
    sourceRect.height = 320;
    bitMap1.setSourceRect(sourceRect);
    bitMap1.drawBitmap(0,0);
    
    sourceRect.x = 36;  //36
    sourceRect.y = 127;   //127
    sourceRect.width = 91;
    sourceRect.height = 14;
    bitMap1.setSourceRect(sourceRect);
    //bitMap1.showPipe();
    //Serial.println("Time to draw...");
    bitMap1.drawBitmap(36,127);
    Serial.println("All drawn...");
  } else {
    Serial.println("Pipe failed to open.");
  }
}


void loop() {
  idle();
}
