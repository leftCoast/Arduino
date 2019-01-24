#include <enterArrow.h>

enterArrow::enterArrow(rect* inRect)
	: stampObj(inRect) { }

enterArrow::~enterArrow(void) { }


void enterArrow::setColors(colorObj* foreColor,colorObj* backColor) {

	mForeColor = *foreColor;
	mBackColor = *backColor;
}

          
void enterArrow::stamp(void) {

	point	arrowPoint[6];
	
	int	rad;
	
	int	left;
	int	aEnd;
	int	right;
	int	lEndx;
	int	centx;
	
	int	top;
	int	lineH;
	int	vBot;
	int	bottom;
	int	aTop;
	int	centy;
	
	rad		= round(((height+width)/2.0)*.25);
	
	left		= x;
	aEnd		= round(x + (.3 * width));
	right		= x+width-1;
	lEndx		= right-rad;
	centx		= lEndx;
	
	top		= round(y + (.25 * height));
	lineH		= round(y + (.75 * height));
	vBot		= lineH - rad;
	bottom	= round(y + height);
	aTop		= lineH - (bottom-lineH);
	centy		= vBot;
  

  	screen->fillRect(this,&mBackColor);
  
	arrowPoint[0].x = right; arrowPoint[0].y = top;		// top of vertical line
	arrowPoint[1].x = right; arrowPoint[1].y = vBot;	// bottom of vertical line
  
	arrowPoint[2].x = lEndx; arrowPoint[2].y = lineH;	// right end of horiz. line
	arrowPoint[3].x = left; arrowPoint[3].y = lineH;	// left end of horiz. line
	arrowPoint[4].x = aEnd; arrowPoint[4].y = bottom;	// bottom end of arrow point (rear)
	arrowPoint[5].x = aEnd; arrowPoint[5].y = aTop;		// top end of arrow point (rear)


	screen->drawLine(arrowPoint[0].x,arrowPoint[0].y,arrowPoint[1].x,arrowPoint[1].y,&mForeColor);
	screen->drawLine(arrowPoint[2].x,arrowPoint[2].y,arrowPoint[3].x,arrowPoint[3].y,&mForeColor);
  	screen->drawLine(arrowPoint[3].x,arrowPoint[3].y,arrowPoint[4].x,arrowPoint[4].y,&mForeColor);
  	screen->drawLine(arrowPoint[3].x,arrowPoint[3].y,arrowPoint[5].x,arrowPoint[5].y,&mForeColor);
  	
  	//screen->drawCircle(centx,centy,rad,&mForeColor);
  	screen->startWrite();
  	screen->drawCircleHelper(centx,centy,rad,4,&mForeColor);
  	screen->endWrite();
}
  	
  	