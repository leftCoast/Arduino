
#include "sparkle.h"

sparkle::sparkle(int inIndex, colorObj* color, float onTime, float dormantTime, float buildTime, float decayTime) {

  unsigned int startTime = micros();
  unsigned int time = startTime;
  index = inIndex;
  baseColor = *color;
  if (onTime<=0) onTime=1;
  if (dormantTime<=0) dormantTime=1;
  if (buildTime<=0) buildTime=1;
  if (decayTime<=0) decayTime=1;
  blendMapper.addPoint(time,0);
  time = time + dormantTime;
  blendMapper.addPoint(time,0);
  time = time + buildTime;
  blendMapper.addPoint(time,100);
  time = time + onTime;
  blendMapper.addPoint(time,100);
  time = time + decayTime;
  blendMapper.addPoint(time,0);
  lifeTimer = new timeObj(time-startTime);
  lifeTimer->start();
}


sparkle::~sparkle(void) { if (lifeTimer) delete(lifeTimer); }


// Check to see if this sparkle is still alive.
boolean sparkle::checkSparkle(void) { return !lifeTimer->ding(); }


// Calculate the color to display at this time.
colorObj sparkle::getColor(colorObj * backgroundColor) { return baseColor.blend(backgroundColor,blendMapper.Map(micros())); }



