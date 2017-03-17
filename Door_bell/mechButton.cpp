#include "mechButton.h"

mechButton::mechButton(byte inPinNum) : timeObj(BOUNCE_MS) {

  pinNum = inPinNum;
  beenInitialized = false;
}


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


