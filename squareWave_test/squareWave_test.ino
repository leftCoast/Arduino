#include "squareWave.h"

class xblinker : public squareWave {

   public:
            xblinker(void);
   virtual  ~xblinker(void);
   virtual  void  pulseOn(void);
   virtual  void  pulseOff(void);
};


xblinker::xblinker(void)
: squareWave() { }


xblinker::~xblinker(void) {  }


void xblinker::pulseOn(void) { digitalWrite(13,true); }

void xblinker::pulseOff(void) { digitalWrite(13,false); }

xblinker ourxblinker;


void setup() {

   pinMode(13,OUTPUT);
   ourxblinker.setPeriod(5000);
   ourxblinker.setPulse(40);
   ourxblinker.setPercent(50);
   ourxblinker.setOnOff(true);
}


void loop() {
   idle();
}
