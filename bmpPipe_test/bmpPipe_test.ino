#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <gfxfont.h>
#include <SD.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#define TFT_CS  10
#define TFT_RST -1
#define SD_CS   4

bmpPipe bitMap1;

void setup() {
  
  rect  sourceRect;

  /*
  if (!initScreen(ADAFRUIT_1947,PORTRAIT)) {
    while(true); // Kill the process.
  }
  */


  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  if (!SD.begin(SD_CS)) {     // Bring the diskdrive online.
    Serial.println("NO SD drive.");Serial.flush();
    while(true);
  }
  
  Serial.println("Painting screen.");Serial.flush();
  screen->fillScreen(&blue);
  Serial.println("Opennig pipe.");Serial.flush();
  if (bitMap1.openPipe("icons/gChHL22.bmp")) {
    Serial.println("Pipe open?");Serial.flush();
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.width = 44;
    sourceRect.height = 22;
    bitMap1.setSourceRect(sourceRect);
    bitMap1.drawBitmap(10,1);
    /*
    sourceRect.x = 36;  //36
    sourceRect.y = 127;   //127
    sourceRect.width = 91;
    sourceRect.height = 14;
    bitMap1.setSourceRect(sourceRect);
    //bitMap1.showPipe();
    //Serial.println("Time to draw...");
    bitMap1.drawBitmap(36,127);
    Serial.println("All drawn...");
    */
  } else {
    Serial.println("Pipe failed to open.");
    Serial.flush();
  }
}


void loop() {
  idle();
}
