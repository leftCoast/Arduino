
#include <drawObj.h>

class sinwave : public drawObj {

  public :
        sinwave(word inX,word inY,word width,word height,boolean inClicks=false);

virtual void setFreq(float pixelsPerCycle);
virtual void setColors(colorObj* lColor,colorObj* bColor=NULL); // NULL bColor for transparent.
virtual void drawSelf(void);

        float     freq;
        colorObj  lColor;
        colorObj  bColor;
        boolean   transparent;
};

