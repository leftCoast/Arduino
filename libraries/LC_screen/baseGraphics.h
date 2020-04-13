#ifndef baseGraphics_h
#define baseGraphics_h

#include "Arduino.h"

// This is the top point.. err.. Foundation base? Of all the drawing.
// You start with a point, use that to define a rectangle. Basically
// everything you draw is a rectangle. Yeah, lines and circles are a
// little different. But really they are too. Everyone gets to know
// what a point and a rect are.

// The lowest common denominator
  struct point {
    int x;
    int y;
  }; 
  
//void	printPoint(point* inPt,char* name="Point : ");
int	xDistance(point ptA,point ptB);
int	yDistance(point ptA,point ptB);
float distance(point ptA,point ptB);  
float	angle(point ptA,point ptB);		// Radians, ptA is center point.



// ***************************************************************
// Base class for a rectangle. 
// ***************************************************************

									
enum rectPt { topLeftPt, topRightPt, bottomLeftPt, bottomRightPt };


class rect {

	public :
  				rect(void);
  				rect(rect* inRect);
  				rect(int inX, int inY, int inWidth,int inHeight);
	virtual	~rect(void);

	virtual	void  setLocation(int inX, int inY);
  			  	void  setSize(int inWidth,int inHeight);
  			  	void  setRect(rect* inRect);                 					// Got a rect? Make this one the same.
  			  	void  setRect(point* inPt1,point* inPt2);							// Or two points..
  			  	void	setRect(int inX, int inY, int inWidth,int inHeight);	// Or the usual way..
          	void	insetRect(int inset);											// Inset all sides by this much. Or expand if negative.
          	int  	maxX(void);                            					// Where's our last pixel?
          	int  	maxY(void);                            					// Same as obove but in the Y direction.
          	int  	minX(void);                            					// Where's our first pixel?
          	int  	minY(void);                            					// Same as obove but in the Y direction.
  			  	bool 	inRect(int inX, int inY);              					// Is this point in us?
  			  	bool 	inRect(point* inPoint);											// Is this point in us?
  			  	point	getCorner(rectPt corner);										// Pass back the corner point.
				bool	overlap(rect* inRect);											// Is that rect touching us?
				//void	printRect(char* = "Rect : ");
					
					int   x;
					int   y;
					int  width;
					int  height;
};

#endif