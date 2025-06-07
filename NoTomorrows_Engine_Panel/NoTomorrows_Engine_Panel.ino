
#include <serialStr.h>
#include "tachMgr.h"
#include <toneObj.h>

#define OIL_PIN      20
#define TEMP_PIN     21
#define TACH_IN_PIN  22
#define TACH_OUT_PIN 23
#define BEEP_OUT_PIN 16    // Purple on tycho's test board. High for beep.
#define FUTURE_PIN   17
#define TEETH_ROTATE 97
#define WAIT_TIME_MS 1000  // One second

serialStr   serialMgr;
tachMgr     ourTachMgr(TEETH_ROTATE,WAIT_TIME_MS);

void setup() {
   
   toneObj   mrTone(BEEP_OUT_PIN);
   tune      WinnerTune;
   
   WinnerTune.addNote(NOTE_E6,Q_NOTE);
   WinnerTune.addNote(NOTE_E6,Q_NOTE);
   WinnerTune.addNote(NOTE_C6,E_NOTE);
   WinnerTune.addNote(NOTE_D6,E_NOTE);
   
   WinnerTune.adjustSpeed(.58);
   
  // WinnerTune.startTune(&mrTone);
   
   Serial.begin(9600);
   Serial.println();
   serialMgr.setCallback(gotCmd);
   if (!ourTachMgr.begin(TACH_IN_PIN,TACH_OUT_PIN)) {
      Serial.println("No Tach!");
      while(1);
   }
   pinMode(BEEP_OUT_PIN,OUTPUT);
   
   float calc;
   int   steps = 30;
   for(int j=0;j<10;j++) {
   for(int i=0;i<steps;i++) {
      calc= i*4 + 200;
      tone(BEEP_OUT_PIN,round(calc));
      delay(i/4);
      noTone(BEEP_OUT_PIN);
   }
   //delay(10);
   for(int i=steps;i;i--) {
      calc= i*4 + 200;
      tone(BEEP_OUT_PIN,round(calc));
      delay(i/4);
      noTone(BEEP_OUT_PIN);
   }
   delay(10);
  
   }
   //digitalWrite(BEEP_OUT_PIN,LOW);
   
}


void setTach(float inRPM) { ourTachMgr.setTach(inRPM); }


void gotCmd(char* inStr) { 
   
   //setTach(atof(inStr));
}


void loop() { idle(); }
