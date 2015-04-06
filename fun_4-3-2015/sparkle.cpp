#include "sparkle.h"

sparkle::sparkle(int inIndex, colorObj* color, float onTime, float dormantTime, float buildTime, float decayTime) {

  index = inIndex;
  baseColor = *color;
  if (onTime > 0) {                                            // Ok tricky.. If we have valid onTime..
    onTimer = new timeObj(onTime);                             // create the onTimer.
    if (dormantTime > 0) {                                     // if we have dormant time,
      dormantTimer = new timeObj(dormantTime);                 // create the dormantTimer.
      state = dormant;                                         // And set state to dormant.
      if (buildTime > 0) {                                     // If we have build time,
        buildTimer = new timeObj(buildTime);                   // create the buildTimer.
      }                                                        // if no build time? In this case, do nothing.
    } else {                                                   // Ah! we DON'T have dormant time!
      if (buildTime > 0) {                                     // If we have buildTime,
        buildTimer = new timeObj(buildTime);                   // Create the buildTimer.
        state = building;                                      // And the state is building. 'Cause there's no dormant time.
      } else {                                                 // We DON'T have buildTime, or dormantTime..
        state = glowing;                                       // So the state is glowing! Whewh!
      }
    }
    if (decayTime > 0) {                                       // In any case if we have decayTime,
      decayTimer = new timeObj(decayTime);                     // create the decayTimer.
    }                                                          // No decayTime? Doesn't matter. Just don't create the decayTimer.
  } else {                                                     // And in the case we don't even have onTime? Stillborn!
    state = dead;                                              // We are already dead!
  }
  if (buildTimer || decayTimer) {                              // After all is said and done. If we have a buildTimer or decayTimer.
    blendPercent = new mapper();                               // We'll need a mapper for the blending calculations.
  }
}



sparkle::~sparkle(void) {

  if (onTimer) delete(onTimer);
  if (dormantTimer) delete(dormantTimer);
  if (buildTimer) delete(buildTimer);
  if (decayTimer) delete(decayTimer);
  if (blendPercent) delete(blendPercent);
}


void sparkle::idle(void) {

  unsigned long now;
  switch (state) {
    case dormant :
      if (dormantTimer->ding()) {
        delete(dormantTimer);
        dormantTimer = NULL;
        if (buildTimer) {
          now = micros();
          blendPercent->setValues(now,now+buildTimer->getTime(),100,0);
          state = building;
        } else {
          state = glowing;
          onTimer->start();
        }
      }
      break;
    case building :
      if (buildTimer->ding()) {
        delete(buildTimer);
        buildTimer = NULL;
        state = glowing;
        onTimer->start();
      }
      break;
    case glowing :
      if (onTimer->ding()) {
        delete(onTimer);
        onTimer = NULL;
        if (decayTimer) {
          now = micros();
          blendPercent->setValues(now,now+decayTimer->getTime(),0,100);
          state = decaying;
          decayTimer->start();
        } else {
          state = dead;
        }
      }
      break;
    case decaying :
      if (decayTimer->ding()) {
        delete(decayTimer);
        decayTimer = NULL;
        state = dead;
      }
  }
}


// Check to see if this sparkle is still alive.
boolean sparkle::checkSparkle(void) {
  return state != dead;
}


// Calculate the color to display at this time.
colorObj sparkle::getColor(colorObj * backgroundColor) {

  switch (state) {
    case dormant :  return *backgroundColor; break;
    case building : return baseColor.blend(backgroundColor, blendPercent->Map(micros())); break;
    case glowing :  return baseColor; break;
    case decaying : return baseColor.blend(backgroundColor, blendPercent->Map(micros())); break;
    case dead :     return *backgroundColor; break;
  }
}

