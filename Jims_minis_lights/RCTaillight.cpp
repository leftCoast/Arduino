
#include "RCTaillight.h"
#include <colorObj.h>

RCTaillight::RCTaillight(
              int leftYaw,
              int rightYaw,
              int throttleLow,
              int throttleHi,
              int rollPort,
              int rollStarboard,
              int pitchDn,
              int pitchUp,
              boolean reverse,
              int numPixels
              ) : pixelGroup(numPixels) {

  pitchPercent.setValues(pitchDn, pitchUp,  0, 100);
  throttlePercent.setValues(throttleLow, throttleHi, 0, 100);
  if (reverse) {
    rollPercent.setValues(rollPort, rollStarboard, 100, -100);
    yawPercent.setValues(leftYaw, rightYaw, 100, -100);
  } else {
    rollPercent.setValues(rollPort, rollStarboard, -100, 100);
    yawPercent.setValues(leftYaw, rightYaw, -100, 100);
  }
  setValues(0, 0, 0, 0);
}


RCTaillight::~RCTaillight(void) { }


void RCTaillight::setValues(int inYaw, int inThrottle, int inRoll, int ibPitch) {

  yaw      = inYaw;
  throttle = inThrottle;
  roll     = inRoll;
  pitch    = ibPitch;
}


void RCTaillight::draw(void) {

  float     pitchValue;
  float     throttleValue;
  float     yawValue;
  float     rollValue;
  float     turnValue;
  colorObj  result(GREEN);  // Starting with green..
  colorObj  darkSide;

  if (yaw != oldYaw || throttle != oldThrottle || roll != oldRoll || pitch != oldPitch) {  // We get called a LOT. Make sure there really is a change.
    pitchValue = pitchPercent.Map(pitch);             // Get pitch as 0..100 (How much red to add..)
    result = result.blend(&red, pitchValue);          // Create the inital color.

    throttleValue = throttlePercent.Map(throttle);    // Get throttle as 0..100
    result = black.blend(&result, throttleValue);     // Take black and blend this much pitch color into it..
    setPixels(&result);                               // This is the base color for the bar.

    yawValue = yawPercent.Map(yaw);                   // Get yaw as -100..100
    rollValue = rollPercent.Map(roll);                // Get roll as -100..100
    turnValue = (yawValue + rollValue) / 2.0;         // Avarage the two..
    darkSide = result.blend(&black, abs(turnValue)*2);  // The darker value..

    if (turnValue > 0) {                              // The actual side is somewhat arbitrary. The mounting direction is undefined.
      for (word i = 0; i < numPixels / 2; i++) {
        setPixel(i, &darkSide);
      }
    } else {
      for (word i = numPixels / 2; i < numPixels; i++) {
        setPixel(i, &darkSide);
      }
    }
    oldYaw = yaw;              // Save 'em so we don't waste time.
    oldThrottle = throttle;
    oldRoll = roll;
    oldPitch = pitch;
  }
}


