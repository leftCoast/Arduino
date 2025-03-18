#include <DFRobot_0995_Obj.h>
#include <idlers.h>
//#include "FreeSans9pt7b.h"


#define TFT_DC  9
#define TFT_CS  10
#define TFT_RST 3
#define TFT_BL  5


void setup() {
   
   Serial.begin(9600);
      while(!Serial) {
      delay(100);
   }
   analogWrite(TFT_BL,255);                                     // Turn on backlight.
   screen = (displayObj*) new DFRobot_0995_Obj(TFT_CS,TFT_RST);
   if (screen) {
      if (screen->begin()) {
         screen->setRotation(PORTRAIT);
         screen->fillScreen(&black);
         screen->setTextColor(&yellow);
         screen->setTextSize(3);
         screen->setCursor(5,10);
         screen->drawText("Hi there!");
      } else {
         Serial.println("Failed to start display.");
      }
   } else {
      Serial.println("No Display at all!");
   }
}

  
void loop() {
   
  idle();         // Idlers get their time.
}
