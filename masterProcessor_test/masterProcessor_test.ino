#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <timeObj.h>

#include <drawObj.h>
#include <label.h>
#include <screenObj.h>


void setup() {
 
  if (initScreen(INV_PORTRAIT)) {
    screen->fillScreen(BLACK);
  }
  screen->setTextSize(1);
  screen->setTextWrap(true);
  screen->setCursor(10, 10);
  screen->println("waiting for data..");

  Serial.begin(9600);
  //Serial.setTimeout(1500);
  
}

void loop() {

  char  buff[20];
  byte  numChars;
  
  numChars = Serial.readBytes(buff,19);
  if(numChars) {
    buff[numChars] = '\0';
    screen->print("Recieved : ");
    screen->print(numChars);
    screen->print(" Bytes : ");
    screen->print(buff);
    screen->println();
  }hello?
}

