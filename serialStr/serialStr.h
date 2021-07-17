#ifndef serialStr_h
#define serialStr_h


// serialStr.. serialStr is a quick and easy way to read complete c strings from the
// serial port. It runs in the background reading bytes from the serial port as they are 
// available. When it finds the EOS char, typically the newline char '\n', it calls
// user's callback function for the user to deal with the string that has been read in.
//
// This is built on LC_baseTools and you will need to make a call to idle() in your main
// loop() function. And, as always, don't use delay(). That will stop everything.


#include <idlers.h>

class serialStr :  public idler {

   public:
               serialStr(char endChar='\n',int numBytes=64);
   virtual     ~serialStr(void);

               void  setCallback(void(*funct)(const char*)); // Use a callback for a complete string.
   virtual     void  idle(void);
               bool  hadOverrun(void);

               int   index;
               char  EOL;
               int   bytes;
               char* buff;
               void  (*callback)(const char*);
               bool  overrun;
};

#endif
