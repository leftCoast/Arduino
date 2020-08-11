#include "squareWave.h"
#include "blinker.h"


// First we start with creating a swithced waveform class out if the Legos we have avalible.
//
// switchedWave is a low frequency square wave that switches on and off a higher frequency
// square wave that  outputs on a digital pin.
//
// In the constructor you pass in..
// pin number for the resultant output wave form.
// The high freqency pulse width in ms
// The high freqency period in ms (high time + low time)
// The low freqency pulse width in ms
// The low freqency period in ms
//
// You start and stop the process by calling 
// setOnOff() with true or false for on & off.
//
class switchedWave :  public squareWave {

   public:
            switchedWave(int pin,float hfPulse,float hfPeriod,float lfPulse,float lfPeriod);
   virtual  ~switchedWave(void);

   virtual  void     pulseOn(void);
   virtual  void     pulseOff(void);

            blinker* hfWave;
};

// Contructor.
switchedWave::switchedWave(int pin,float hfPulse,float hfPeriod,float lfPulse,float lfPeriod)
   : squareWave(lfPeriod,lfPulse) { hfWave =  new blinker(pin,hfPulse,hfPeriod); }


// Destructor.
switchedWave::~switchedWave(void) { if (hfWave) delete(hfWave); }


// When LF pulse goes high, we tuen the HF waveform on.
void switchedWave::pulseOn(void) { hfWave->setOnOff(true); }


// When LF pulse goes high, we tuen the HF waveform off.
void switchedWave::pulseOff(void) { hfWave->setOnOff(false); }



// ***********************
// Sketch starts here.
// ***********************


// Create a switched wave object.
switchedWave ourWave(13,250,500,10000,20000);   // This gives 20 blinks for 10 seconds then 10 seconds with no blinks.


// In setup() switch it on.
void setup() {
   ourWave.setOnOff(true); // Fire it up.
}


// Call idle in loop(). Anything that runs in the background
// uses this. switchedWave would be one of those things.
void loop() {

   idle();     // Let me all have some idle time. 
}
