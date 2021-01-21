#include <mechButton.h>

#define  TRIGGER_PIN 2        // What pin the trigger will be on.
#define  OUT_PIN     13       // What pin we'll toggle high when the trigger goes low.
#define  HOLD_MS     500      // How long we'll hold the output high after being triggered.


mechButton  trigger(TRIGGER_PIN);   // Button debouncer.
timeObj     timer;                  // Our output hold timer.


// Your standard Arduino setup() function.
void setup() {
   
   pinMode(OUT_PIN,OUTPUT);               // Setup output pin.
   digitalWrite(OUT_PIN,LOW);             // Lets make sure its off.
   trigger.setCallback(triggerAction);    // Set up the function to call when the trigger is clicked.
   timer.setTime(HOLD_MS,false);          // Set the timer, but don't start it yet.
}


// When you click the button, this function will be called.
void triggerAction(void) {

   if (!trigger.trueFalse()) {      // If the trigger returns false, button has been clicked.
      digitalWrite(OUT_PIN,HIGH);   // So we fire the output pin.
      timer.start();                // And start the timer.
   }
}


// Your standard Arduino loop() function.
void loop() {
   
   idle();                       // Give the stuff running behind the scenes time to run.
   if (timer.ding()) {           // If the timer has expired..
      digitalWrite(OUT_PIN,LOW); // Shut down the output.
      timer.reset();             // Reset the timer for next time.
   }
}
