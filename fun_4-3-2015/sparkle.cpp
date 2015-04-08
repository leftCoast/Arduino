
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
colorObj sparkle::getColor(colorObj * backgroundColor) { return baseColor.blend(backgroundColor,100-blendMapper.Map(micros())); }


// Our location in the list of pixles.
int sparkle::getIndex(void) { return index; }


sparkle* sparkle::getNext(void) { return (sparkle*) next; }


// Create a list to dump sparkles into. The list ownes them and will garbage collect
// them during idle time. REMEBER to call hookup() to start the garbage collection.
sparkleList::sparkleList(void) : linkList(true) {  }


sparkleList::~sparkleList(void) {  }


// People are going to want to access the sparkles. Here's a pointer to the list of them.
sparkle* sparkleList::getSparkles(void) { return (sparkle*) theList; }


// SHould be in base class but.. Return the amount of object in this list.
int sparkleList::numSparkles(void) {
  
  int count = 0;
  linkListObj* trace = theList;
  while(trace) {
    count++;
    trace = trace->next;
  }
  return count;
}



// Don't have a lot of time for idle(). So we knock off one ony (1) dead node.       
void  sparkleList::idle(void) {
   
  if (!isEmpty()) {                                // Should we bother?
    sparkle* trace = getSparkles();                 // A pointer to the top of the non-empty list;
    while(trace->checkSparkle()&&trace->getNext()) {    // If this one is ok, and there is a next one..
      trace = trace->getNext();                           // Go to the next one.
    }
    if (!trace->checkSparkle()) {                  // Either this one was dead or it was the last one. (We forgot, so ask again.)
      deleteObj(trace);                            // Ah! this one's dead, delete it.
    } 
  }
}                                                  // And we're out of time!



