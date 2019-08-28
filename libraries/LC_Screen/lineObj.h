#ifndef lineObj_h
#define lineObj_h

#include <drawObj.h>

enum slopeType { vertical, positiveSlope, negativeSlope, horizontal };

class lineObj : public drawObj {

public:
             lineObj(void);
  			 lineObj(int x1,int y1,int x2,int y2,colorObj* inColor);
	virtual	~lineObj(void);
	
				void	setColor(colorObj* inColor);
				void	setSize(byte inSize);                      // line width. Sadly, unsuported.
				void	setEnds(int x1,int y1,int x2,int y2);
				void	setEnds(point* startPt,point* endPt);
				void	setEnds(rect* inRect,slopeType inSlope);
	virtual	void	drawSelf();
  
  				colorObj	color;
  				byte		slope;    // We already have a rectangle. This notes which diagonal.
  				byte		lnSize;
};

#endif
