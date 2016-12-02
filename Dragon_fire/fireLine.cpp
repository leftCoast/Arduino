#include "fireLine.h"
#include <colorObj.h>

fireLine::fireLine(neoPixel* inLites,int inNumLEDs,colorObj* inPatternArray,int inPatternLength,int inNumPatterns) 
: liteLine(inLites,inPatternLength*inNumPatterns) {

  maxIndex = inNumLEDs + (inPatternLength*inNumPatterns) - 1;
  patternArray = inPatternArray;
  patternLength = inPatternLength;
}


fireLine::~fireLine(void) { }


colorObj fireLine::calcColor(int index,int i) {

    colorObj aColor;
    i = i % patternLength;
    if (i==0) {
      int r = random(1,100);
      if (r>90)
        aColor.setColor(&white);
      else
        aColor = patternArray[0];
    } else {
      aColor = patternArray[i];
    }
    return aColor;
}


int fireLine::getMaxIndex(void) { return maxIndex; }

