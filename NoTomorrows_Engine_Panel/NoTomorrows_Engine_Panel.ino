
#include <serialStr.h>
#include "tachMgr.h"


#define OIL_PIN      20
#define TEMP_PIN     21
#define TACH_IN_PIN  22
#define TACH_OUT_PIN 23
#define TEETH_ROTATE 97
#define WAIT_TIME_MS 1000  // One second

serialStr   serialMgr;
tachMgr     ourTachMgr(TEETH_ROTATE,WAIT_TIME_MS);

void setup() {
   
   Serial.begin(9600);
   Serial.println();
   serialMgr.setCallback(gotCmd);
   if (!ourTachMgr.begin(TACH_IN_PIN,TACH_OUT_PIN)) {
      Serial.println("No Tach!");
      while(1);
   }
}


void setTach(float inRPM) { ourTachMgr.setTach(inRPM); }


void gotCmd(char* inStr) { setTach(atof(inStr)); }


void loop() { idle(); }
