#ifndef debug_h
#define debug_h

#include "Arduino.h"


class debug {

  public :
                debug(void);
  virtual       ~debug(void);
  
				void  trace(char* message,bool hold=true);
				//void  tracex(char* message,bool hold=true);
				void  trace(char* message,int inNum,bool hold=true);
				void  trace(char* message,char* inStr,bool hold=true);
};

extern debug debugger;

#endif