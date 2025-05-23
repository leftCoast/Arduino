#include "src/wristDisp.h"


#define TFT_CS  10
#define TFT_RST 3
#define TFT_BL  5
#define TFT_DC  9

wristDisp   ourDisp(TFT_CS,TFT_RST,TFT_BL,TFT_DC);

void setup() {
   Serial.begin(57600);
   startSerial();
   Serial1.begin(9600);
   if (!ourDisp.begin()) {
      switch(ourDisp.errMsg()) {
         case noErr     : Serial.println("Odd. Failed to start but shows no error.");  break;
         case initErr   : Serial.println("Failed to start display.");                  break;
         case memErr    : Serial.println("No Display at all!");                        break;
         default        : Serial.println("Unknown error!");                            break;
      }
      while(1);
   }
}

  
void loop() {
   
  idle();               // Idlers get their time.
  ourDisp.checkDisp();  // displayTime.
}
