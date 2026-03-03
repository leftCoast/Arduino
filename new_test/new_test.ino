#include <DFRobot_0669.h>
#include <SD.h>

// For DFRobot_0669
#define DSP_CS    10
#define SD_CS     4
#define DSP_RST   26



void setup() {
   
   Serial.begin(9600);
   delay(2000);
   Serial.println("Allocation of screen");
   screen = (displayObj*) new DFRobot_0669(DSP_CS,DSP_RST);
   Serial.println("screen allocated");
   screen->begin();
   Serial.println("begin called");
   screen->fillScreen(&green);
   Serial.println("screen green?");
   if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
      Serial.println("NO SD CARD!");                              // Send an error out the serial port.
      screen->setCursor(10,10);     // Move cursor to the top left.
      screen->setTextColor(&red); // Drawing in white..
      screen->setTextSize(2);       // Big enough to notice.
      screen->drawText("NO SD CARD!");     // Draw the error message.
   } else {
      screen->setCursor(10,10);     // Move cursor to the top left.
      screen->setTextColor(&black); // Drawing in white..
      screen->setTextSize(2);       // Big enough to notice.
      screen->drawText("FOUND SD CARD!");     // Draw the error message.
   }
}

void loop() {
   

}
