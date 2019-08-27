#include "debug.h"
#include <resizeBuff.h>


debug::debug(void) { }
debug::~debug(void) { }

void debug::trace(char* message,bool hold) {

  Serial.print(message);
  Serial.flush();
  if (hold) {
    while(!Serial.available());
    Serial.read();
  } 
}


void debug::trace(char* message,int inNum,bool hold=true) {

  char* buff;
  int   numChars;
  
  buff = NULL;
  numChars = strlen(message);
  if (resizeBuff(numChars+10,&buff)) {
    strcpy(buff,message);
    snprintf(&(buff[numChars]),9,"%d",inNum);
    trace(buff,hold);
    resizeBuff(0,&buff);
  } else {
    Serial.println(F("NO RAM!!"));
  }
}


debug debugger;