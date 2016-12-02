#ifndef fireLine_h
#define fireLine_h

#include <mapper.h>
#include <colorObj.h>
#include <liteLine.h>

class fireLine : public liteLine {
  
  public:
    fireLine(neoPixel* inLites,int inNumLEDs,colorObj* inPatternArray,int inPatternLength,int inNumPatterns);
    ~fireLine(void);
    virtual colorObj calcColor(int index,int i);
    int getMaxIndex(void);              // We calculate this from the setup data.

    int       patternLength;
    colorObj* patternArray;
    int       maxIndex;
};

#endif

