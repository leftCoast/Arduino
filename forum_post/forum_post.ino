#include <idlers.h>
#include <lists.h>
#include <timeObj.h>


// **************************************
//            CHASE CLASS
// **************************************


int ledList[] = {2,3,5,7,8,9,A1,A2};

// The deivation of the class.
class chaseBlinker :  public idler {
   
   public:
                  chaseBlinker(void);
         virtual  ~chaseBlinker(void);

                  void  begin(float timeOnMs = 100);
         virtual  void  idle(void); 

                  int      index;
                  bool     beenSetup;
                  timeObj  onTimer;
};

// Constructor. Not much to do here.
chaseBlinker::chaseBlinker(void) { beenSetup = false;  }


// Destructor, also not much to do.
chaseBlinker::~chaseBlinker(void) {  }


// Initialzer to be called in your setup() funtion.
// You can set the times differently here as well.
void chaseBlinker::begin(float timeOnMs) {

   hookup();
   if (!beenSetup) {
      for (int i=0;i<8;i++) {
         pinMode(ledList[i], OUTPUT);
      }
      beenSetup = true;
   }
   onTimer.setTime(timeOnMs);
   index = 0;
}


// Idle is where you check the timer to see if its time to do somwthing.
// If so, we do that something (swap LEDS) and reset the timer.
void  chaseBlinker::idle(void) {

   if (onTimer.ding()) {                  // If our timer has expired..
      digitalWrite(ledList[index], LOW);  // Turn off the current LED.
      index++;                            // Increment the counter to the next LED.
      if (index>=8) {                     // If we overshot..
         index = 0;                       // We reset to zero.
      }
      digitalWrite(ledList[index], HIGH); // Counter has been incremented, turn on the next LED.
      onTimer.start();                    // Restart the timer for this LED.
   }
}



// **************************************
//         PROGRAM STARTS HERE
// **************************************

chaseBlinker   ourChaser;

void setup(void) {

   ourChaser.begin();
}


void loop(void) {

   idle();
   // Do your stuff, DON"T call delay.
}
