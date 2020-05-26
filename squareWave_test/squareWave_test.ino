#include "squareWave.h"
#include "timeObj.h"

int signal;

class testWave :  public squareWave {

   public:
   virtual void pulseOn(void);
   virtual void pulseOff(void);
};


void testWave::pulseOn(void) { signal = 1; }
void testWave::pulseOff(void) { signal = 0; }

testWave ourSquareWave;
char     inBuff[100];
int      i;
timeObj  outTimer(10);

void setup() {

   signal   = 0;
   i        =0;
   ourSquareWave.setPeriod(1000);
   ourSquareWave.setPulse(250);
   ourSquareWave.setOnOff(true);
   outTimer.start();
}


void loop() {

   char  aChar;
   int   newVal;
   
   idle();
   if (Serial.available()) {
      aChar = Serial.read();                       
      if (aChar=='\n') {
         inBuff[i] = '\0';
         i = 0;
         newVal = atoi(inBuff);
         ourSquareWave.setPulse(newVal);
         Serial.print("Set pulse to : ");
         Serial.println(newVal);
      } else {
         inBuff[i++] = aChar;
      }
   }
   if (outTimer.ding()) {
      Serial.println(signal);
      outTimer.start();
   }
}
