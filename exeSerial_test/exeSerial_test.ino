#include  <extSerial.h>                  // Including the pibrary.

extSerial mySerial(&Serial);           // Create by choosing your port.

void setup(void) {

   Serial.begin(9600);                 // Start up as usual.
   if (!Serial) {
      //wait, or not..
   }
   mySerial.begin();                   // Don't need to wait or ask. Just call it.
   Serial.println("We have Serial!");  // Output is unaffected.
}


// We'll do a simple "echo what was typed" exa.mple here.
void loop(void) {

   char  aChar;
   
   idle();                       // IMPORTANT!! idle() runs all the magic behind the scenes.
   if (mySerial.available()) {   // Use extSerial for available().
      aChar = mySerial.read();   // And read().
      Serial.print(aChar);       // Original calls for output.
   }
}
