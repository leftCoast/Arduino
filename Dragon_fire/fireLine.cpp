#include "fireLine.h"
#include <colorObj.h>

mapper dimmer(0,120,0,100);

fireLine::fireLine(neoPixel* inLites,int inNumLEDs,colorObj* inPatternArray,int inPatternLength,int inNumPatterns) 
: liteLine(inLites,inPatternLength*inNumPatterns) {

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
    //aColor.blend(&black,dimmer.Map(index));
    return aColor;
}



