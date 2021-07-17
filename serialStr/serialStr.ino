#include "serialStr.h"

serialStr strReader('\n',16); // We'll only allow 16 bytes. Then we can see how it handles overruns.


void setup(void) {

   strReader.setCallback(haveStr);                          // Hook up our callback.
   Serial.begin(57600);                                     // Fire up the serial stuff.
   Serial.println(F("Enter something. I'll repeat it."));   // Tell Mrs user to start inputting numbers.
}


// Magically, complete c strings show up here.
void haveStr(const char* inStr) {
   
   int i=0;

   if (strReader.hadOverrun()) {                         // If we had an overrun..
      Serial.println(F("Oh no! overrun! Lost data!"));   // Tell the user.
   }
   Serial.print(F("Entered : "));                        // Show what we got..
   Serial.println(inStr);                             
   Serial.print(F("Reply   : "));                        // Show our reply.
   while(inStr[i]) {
      Serial.print((char)toupper(inStr[i++]));
   }
   Serial.println();
}


void loop(void) { idle(); }
