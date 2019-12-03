
#include "screen.h"
#include "mechButton.h"
#include "cClockOS.h"
#include "eventMgr.h"


void setup() {

  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    while (1);
  }
  ourOS.begin();
}

void loop() {     // During loop..
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
