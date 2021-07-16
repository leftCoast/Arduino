#include "serialWatch.h"

serialWatch strReader('\n',16);


void setup(void) {

   Serial.begin(57600);                                     // Fire up the serial stuff.
   Serial.println(F("Enter something. I'll repeat it."));   // Tell Mrs user to start inputting numbers.
   strReader.setCallback(haveStr);                          // Hook up our callback.
}


// Magically complete c strings show up here.
void haveStr(const char* inStr) {
   
   int i=0;

   if (strReader.hadOverrun()) {                      // If we had an overrun..
      Serial.println(F("Oh no! overrun! Lost data!"));  // Tell the user.
   }
   Serial.print(F("Entered : "));                     // Show what we got..
   Serial.println(inStr);                             
   Serial.print(F("Reply   : "));                     // Show our reply.
   while(inStr[i]) {
      Serial.print((char)toupper(inStr[i++]));
   }
   Serial.println();
}


void loop(void) { idle(); }
