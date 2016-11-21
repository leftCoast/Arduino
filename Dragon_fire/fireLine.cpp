#include "fireLine.h"
#include <colorObj.h>

colorObj    fireHead;
colorMapper flameMapper;
mapper      indexToColor;
boolean     fireInit = false;   // To force a "begin()" to be called.


fireLine::fireLine(neoPixel* inLites,int inLength) : liteLine(inLites,inLength) {  }


fireLine::~fireLine(void) { }


// Sets up globals so only one need be called for all.
void fireLine::begin(void) {

  fireHead.setColor(&yellow);
  fireHead.blend(&black,70);
  
  flameMapper.setColors(&(red.mixColors(&black,70)),&black);
  
  indexToColor.setValues(1,getLength(),0,100);
  fireInit = true;
}


colorObj fireLine::calcColor(int index,int i) {

    colorObj fireTail;
    float tailPercent;
    colorObj aColor;
    
    if (i==0) {
      int r = random(1,100);
      if (r>95)
        aColor.setColor(&white);
      else
        aColor = fireHead;
    } else {
      tailPercent = indexToColor.Map(i);
      aColor = flameMapper.Map(tailPercent);
    }
    return aColor;
}


void fireLine::setLights(int index,boolean wrap=false) { if (fireInit) liteLine::setLights(index,wrap); }


