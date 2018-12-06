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
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include "editField.h"


#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

editField hello("Hello world, Nigel");

void setup() {
  colorObj aColor;

  Serial.begin(9600);
  while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  viewList.addObj(&hello);
  doColor(LC_BLACK,"BLACK");
  doColor(LC_WHITE ,"WHITE");
  doColor(LC_NAVY,"NAVY");
  doColor(LC_DARK_GREEN,"DARKGREEN");
  doColor(LC_PURPLE,"PURPLE");
  doColor(LC_ORANGE,"ORANGE");
  doColor(LC_PINK,"PINK");
  doColor(LC_OLIVE,"OLIVE");
  doColor(LC_BLUE,"BLUE");
  doColor(LC_LIGHT_BLUE,"LIGHT_BLUE");
  doColor(LC_LAVENDER,"LAVENDER");
  doColor(LC_RED,"RED");
  doColor(LC_GREEN,"GREEN");
  doColor(LC_CYAN ,"CYAN");
  doColor(LC_MAGENTA,"MAGENTA");
  doColor(LC_YELLOW,"YELLOW");
  doColor(LC_LIGHT_GREY,"LIGHT_GREY");
  doColor(LC_DARK_GREY,"DARK_GREY");

}

void doColor(byte r,byte g,byte b,char* color) {

  colorObj aColor;
  
  aColor.setColor(r,g,b);
  screen->fillScreen(&aColor);
  word value = aColor.getColor16();
  Serial.print(value,HEX);Serial.print(" : ");Serial.println(color);
  while (!Serial.available());
  char inChar = Serial.read();
} 

  
void loop() {
  idle();

}
