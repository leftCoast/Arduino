#include "LC_fona.h"



// Drop in a string and this'll strip out anything
// that's not a "dial-able" character.
// ** WARNING ** This writes to the string in place. So you can't pass a string that was
// allocated at compile time IE char myNum = "1 408 340-0352"
void filterPNStr(char* str) {

   int numChars;
   int index;
   
   if (str) {                          // Sanity, they could pass in a NULL. They do that now and then.
      numChars = strlen(str);          // Ok have something. Lets count 'em.
      index = 0;                       // We'll use this to index mRawPN.
      for(int i=0;i<=numChars;i++) {   // We'll loop though including the EOL.
         switch(str[i]) {              // There may be better ways of doing this,
            case '0'  :                //  but this makes it realaly obvious what we're doing.
            case '1'  :
            case '2'  :
            case '3'  :
            case '4'  :
            case '5'  :
            case '6'  :
            case '7'  :
            case '8'  :
            case '9'  :
            case '#'  :
            case '*'  :
            case '\0' : str[index++] = str[i]; break;  // Stuff in the "filtered" char.
         }
      }
   }
}







LC_fona::LC_fona(void)
  : Adafruit_FONA(FONA_RST) {
  ok_reply = F("OK");
}


LC_fona::~LC_fona(void) {  }


bool LC_fona::setParam(FONAFlashStringPtr send, int32_t param) {

  return sendCheckReply(send, param, ok_reply);
}


bool  LC_fona::checkForCallerID(char* IDBuff, byte numBytes) {

  if (mySerial->available()) {                                          // Is something is there we didn't ask for..
    if (readline(25,true) > 15) {                                      // See if we can grab the first 20 or so chars.
      return parseReplyQuoted(F("+CLIP: "), IDBuff, numBytes, ',', 0);  // Run it through the Adafruit parser thing to see if its a caller ID.
    }
  }
  return false;
}
