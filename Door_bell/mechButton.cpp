#include "mechButton.h"

// mechButton::mechButton(byte inPinNum) : timeObj(BOUNCE_MS) {

mechButton::mechButton(byte inPinNum) {
  pinNum = inPinNum;
  beenInitialized = false;
  checkNum = 0;
}

/*
bool mechButton::clicked(void) {

  if (beenInitialized) {              // Ready to go? Lets look!
    if (ding()) {                     // Has timer expired?
      if (!digitalRead(pinNum)) {     // We can look now. Pushed?
        start();                      // Restart the timer.
        return true;                  // Report!
      } else {                        // Or not pushed..
        return false;                 // Report these too.
      }
    } else {                          // Timer's running.
      return true;                    // So ignoring hardware, still true.
    }
  } else {                            // Whoops, not set up yet!
    pinMode(pinNum, INPUT_PULLUP);    // Do it.
    beenInitialized = true;           // Note it.
    return false;                     // So, false to you!
  }
}
*/

bool mechButton::clicked(void) {

  if (beenInitialized) {                  // Ready to go? Lets look!
    if (setAs == digitalRead(pinNum)) {   // If we get another like the last.
      if (checkNum<NUM_CHECKS) {          // If we don't have enough checks.
        checkNum++;                       // We add another to the checkNum.
      }
    } else {                              // The reading didn't match the last one?
      setAs = !setAs;                     // Swap the setAs to the new reading.
      checkNum = 0;                       // Restart our count.
    }
  } else {
    pinMode(pinNum, INPUT_PULLUP);        // Do it.
    setAs = digitalRead(pinNum);          // What's the first thing we see? tru or false?
    beenInitialized = true;               // Note it.
    return false;                         // So, false to you!
  }
  if (!setAs && checkNum==NUM_CHECKS) {   // Only intereted in the Zero case with enough checks.
    return true;
  } else {
    return false;
  }
}

