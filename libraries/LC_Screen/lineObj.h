#ifndef lineObj_h
#define lineObj_h

#include <drawObj.h>

enum slopeType { vertical, positiveSlope, negativeSlope, horizontal };

class lineObj : public drawObj {

public:
             lineObj(void);
  			 lineObj(word x1,word y1,word x2,word y2,colorObj* inColor);
  			 
        void setColor(colorObj* inColor);
        void setSize(byte inSize);                      // line width. Sadly, unsuported.
        void setEnds(word x1,word y1,word x2,word y2);
        void setEnds(point* startPt,point* endPt);
        void setEnds(rect* inRect,slopeType inSlope);
virtual void drawSelf();
  
  colorObj color;
  byte  slope;    // We already have a rectangle. This notes which diagonal.
  byte  lnSize;
};

#endif
