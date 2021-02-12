#include <SD.h>
#include <neoPixel.h>
#include <bmpPipe.h>
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


neoPixel  lites(NUM_LEDS, PIXEL_PIN);
bmpPipe   thePic;
RGBpack   theLine[NUM_LEDS];
int       lineNum;

void setup() {
  
   lites.begin();
   if (initScreen(ADAFRUIT_1431,OLED_CS,OLED_RST,INV_PORTRAIT)) {
      lites.setAll(&green);
      
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
    screen->fillScreen(&green);
    delay(25);
    screen->fillScreen(&black);
  } else {
    lites.setAll(&red);
  }
  lites.show();
  delay(500);
  lites.setAll(&black);
  lites.show();
  delay(500);
  if (thePic.openPipe(FILE_NAME)) {
    lites.setAll(&green);
  } else {
    lites.setAll(&red);
  } 
  lites.show();
  delay(250);
  lites.setAll(&black);
  lites.show();

  lineNum = 0;
}


void printColor(int inLine,int inCol,RGBpack* inColor) {
   
   colorObj aColor;

   Serial.print("line : ");
   Serial.print(inLine);
   Serial.print(", Col : ");
   Serial.print(inCol);
   Serial.print(" ");
   aColor.setColor(inColor);
   aColor.printRGB();
   Serial.println();  
}


void loop(void) {
    
  thePic.getLine(theLine,lineNum++);
  for (int i=0;i<NUM_LEDS;i++) {
    lites.setPixelColor(i,&(theLine[i]));
    //printColor(lineNum,i,&(theLine[i]));
  }
  if (lineNum>=NUM_LINES) lineNum = 0;
  lites.show();
  delay(20);
}
