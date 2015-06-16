#ifndef RPNServer_h
#define RPNServer_h

#include "remoteRPNCalc.h"
#include <calculator.h>

class RPNServer : public calculator {
  
  RPNServer(void);
  ~RPNServer(void);
  
  void init(void);
  void sendReply(void);
  void geyKeyPress(void);
 
  char keyBuff[10];  // Should be MORE than enough!
};  

#endif

