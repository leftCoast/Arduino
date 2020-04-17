#include <Adafruit_GFX.h>
#include <ILI9341_t3.h>
#include "adafruit_1947_Obj.h"
#include "screen.h"
#include "idlers.h"

void setup() {

  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
  }
  screen->fillScreen(&blue);
  screen->fillScreen(&red);
  screen->fillScreen(&green);
}

void loop() {
  idle();
}
