#ifndef serialWatch_h
#define serialWatch_h

#include <idlers.h>

class serialWatch :  public idler {

   public:
               serialWatch(char endChar='\n',int numBytes=64);
   virtual     ~serialWatch(void);

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
