#include <SD.h>
#include <neoPixel.h>
#include <bmpPipe.h>

#define NUM_LEDS    12 //150
#define PIXEL_PIN   10
#define SD_CS       4

neoPixel  lites(NUM_LEDS, PIXEL_PIN);
bmpPipe   thePic;
RGBpack   theLine[NUM_LEDS];
int       lineNum;

void setup() {
  
  lites.begin();
  if (SD.begin(SD_CS)) {
    lites.setAll(&green);
  } else {
    lites.setAll(&red);
  }
  lites.show();
  delay(1000);
  lites.setAll(&black);
  lites.show();
  delay(1000);
  if (thePic.openPipe("/neoPixel.bmp")) {
    lites.setAll(&green);
  } else {
    lites.setAll(&red);
  } 
  lites.show();
  delay(1000);
  lites.setAll(&black);
  lites.show();

  lineNum = 0;
}


void loop(void) {

  thePic.getLine(theLine,lineNum++);
  for (int i=0;i<NUM_LEDS,i++) {
    setPixelColor(i,&(theLine[i]));
  }
  if (lineNum>=NUM_LEDS) lineNum = 0;
}
