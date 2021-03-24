#include <SD.h>
#include <neoPixel.h>
#include <bmpImage.h>
#include <adafruit_1431_Obj.h>
#include <screen.h>

#define NUM_LEDS     74 //150
#define NUM_LINES    25
#define PIXEL_PIN    10
#define SD_CS        4
//#define FILE_NAME    "/neoPixel.bmp"
#define FILE_NAME    "/big.bmp"

#define OLED_CS      7
#define OLED_RST     8


neoPixel    lites(NUM_LEDS, PIXEL_PIN);
bmpImage    thePic;
RGBpack     theLine[NUM_LEDS];
int         lineNum;
File        hackFile;

void setup() {
  
   lites.begin();
   if (initScreen(ADAFRUIT_1431,OLED_CS,OLED_RST,INV_PORTRAIT)) {
      lites.setAll(&green);
      screen->fillScreen(&green);
   } else {
      lites.setAll(&red);
   }
   lites.show();
   delay(500);
   lites.setAll(&black);
   lites.show();
   delay(500);
   if (SD.begin(SD_CS)) {
      lites.setAll(&green);
   } else {
      Serial.println("No SD Card?");
      lites.setAll(&red);
   }
   lites.show();
   delay(500);
   lites.setAll(&black);
   lites.show();
   delay(500);
   if (thePic.openDocFile(FILE_NAME)) {
      hackFile = SD.open(FILE_NAME,FILE_READ);
      thePic.setGFile(&hackFile);
      lites.setAll(&green);
   } else {
      lites.setAll(&red);
   } 
   lites.show();
   delay(250);
   lites.setAll(&black);
   lites.show();
   screen->fillScreen(&black);
   lineNum = 0;
}



void loop(void) {
    
  thePic.getRow(lineNum++,theLine,NUM_LEDS);
  for (int i=0;i<NUM_LEDS;i++) {
    lites.setPixelColor(i,&(theLine[i]));
    //printColor(lineNum,i,&(theLine[i]));
  }
  if (lineNum>=NUM_LINES) lineNum = 0;
  lites.show();
  delay(200);
}
