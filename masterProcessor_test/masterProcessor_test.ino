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
  screen->println("waiting for data");
  screen->print(">");

  Serial.begin(9600);
  Serial.setTimeout(50);
  
}

void loop() {

  char buff[20];

  Serial.readBytes(buff,20);
  screen->print(buff);
}

