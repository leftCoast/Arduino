#include "sprit.h"


sprit::sprit(void) { spritRunning = false; }

void sprit::startSprit(void) { spritRunning = true; }

void sprit::stopSprit(void) { spritRunning = false; }

boolean sprit::active(void) { return spritRunning; }



// ********* bitmap8x8 *********


bitmap8x8::bitmap8x8(Adafruit_8x8matrix* inMatrix) {
  matrix = inMatrix;
}


// Don't actually start things, just get them set up
// so they will start automatically.
void bitmap8x8::startSprit(void) {

  frameNum = -1;          // Set our frame number -before- the first frame.
  hookup();               // Make sure the idle() will be active.
  frameTimer.setTime(0);  // Time of zero, lets start ASAP!
  spritRunning = true;    // Allow the mehanism to run!
}


// Does nothing, override to show your frame.
void bitmap8x8::showFrame(void) { }


// Idle controls when and what frame is shown.
// Also controls ending the sprit at the right time.
void bitmap8x8::idle(void) {

  if (spritRunning) {              // Ok, if were running..
    if (frameTimer.ding()) {       // Check to see if its time to do something.
      if (frameNum > numFrames) {  // It was time. If we've gone to the end of our sprit.
        spritRunning = false;      // We're done. When the world asks, we'll say so.
      } else {                     // Still more frames. Lets show the next one.
        frameNum++;                // Bump frame count.
        showFrame();               // Show it.
      }
    }
  }
}


void bitmap8x8::showBitmap(const uint8_t* bitmap, unsigned long Ms,boolean inverse) {

  if (!inverse) {
    matrix->clear();
    matrix->drawBitmap(0, 0, bitmap, 8, 8, LED_ON);
  } else {
    matrix->drawBitmap(0, 0, on_bmp, 8, 8, LED_ON);  // Clear by setting all on.
    matrix->drawBitmap(0, 0, bitmap, 8, 8, LED_OFF); // Draw by turning LEDs off.
  }
  matrix->writeDisplay();
  frameTimer.setTime(Ms);
  frameTimer.start();
}


// ********* text8x8 *********


text8x8::text8x8(Adafruit_8x8matrix* inMatrix, char* inStr, float inStepDelay) {

  int numChars;

  matrix = inMatrix;
  numChars = strlen(inStr);
  theStr = (char*) malloc(numChars + 1);
  strcpy(theStr, inStr);
  stepDelay = inStepDelay;
  endNum = -6 * numChars;       // Calculate the number of steps.
}


text8x8::~text8x8(void) {
  free(theStr);
}


// Logic here is same as bitmap8x8 above.
void text8x8::startSprit(void) {

  stepNum = 1;                   // Set our step number -before- the first step.
  hookup();                      // Make sure the idle() will be active.
  stepTimer.setTime(0);          // Time of zero, lets start ASAP!
  matrix->setTextSize(1);        // We are going to assume we own the matrix for now.
  matrix->setTextWrap(false);    // We dont want text to wrap so it scrolls nicely
  matrix->setTextColor(LED_ON);  // Writing in light.
  spritRunning = true;           // Allow the mechanism to run!
}


void text8x8::idle(void) {

  if (spritRunning) {                 // Ok, if were running..
    if (stepTimer.ding()) {           // Check to see if its time to do something.
      if (stepNum < endNum) {         // It was time. If we've gone to the end of our sprit.
        spritRunning = false;         // We're done. When the world asks, we'll say so.
      } else {                        // Still more steps. Lets show the next one.
        stepNum--;                    // Bump step count down.
        matrix->clear();              // Show it.
        matrix->setCursor(stepNum, 0);
        matrix->print(theStr);
        matrix->writeDisplay();
        if (stepNum==0) {                // Special case we need to set the delay for the rest of the steps.
          stepTimer.setTime(stepDelay);
        } else {
          stepTimer.stepTime();
        }
      }
    }
  }
}


