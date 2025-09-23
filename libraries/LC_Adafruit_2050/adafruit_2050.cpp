#include <adafruit_2050.h>


// **********************************************************
//								maskableHX8357
//	Adafruit_HX8357 with the ability to have a mask added.	
// **********************************************************


maskableHX8357::maskableHX8357(int inCS, int inDC, int inRst)
	: Adafruit_HX8357(inCS,inDC,inRst) { }
	
	
maskableHX8357::~maskableHX8357(void) {  }
	
	
void maskableHX8357::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			Adafruit_HX8357::drawPixel(x,y,color);
		}
	} else {
		Adafruit_HX8357::drawPixel(x,y,color);
	}
}


void maskableHX8357::writePixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			Adafruit_HX8357::writePixel(x,y,color);
		}
	} else {
		Adafruit_HX8357::writePixel(x,y,color);
	}
}


void maskableHX8357::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

	int	lastX;
	int	lastY;
	
	if (gMask) {
		switch(gMask->checkRect(x,y,w,h)) {
			case unMasked			: Adafruit_HX8357::fillRect(x,y,w,h,color); break;
			case totalMasked		: return;
			case partialMasked	:
				lastX = x + w;
				lastY = y + h;
				for(int i=y;i<lastY;i++) {
					for(int j=x;j<=lastX;j++) {
						drawPixel(j,i,color);
					}
				}
			break;
		}
	} else {
		Adafruit_HX8357::fillRect(x,y,w,h,color);
	}
}

// **********************************************************
//								adafruit_2050	
// **********************************************************



adafruit_2050::adafruit_2050(int inCS, int inDC, int inRst)
  :displayObj(true,true,false,true,false) {

	theTFT		= NULL;
	cs				= inCS;
	dc				= inDC;
	rst			= inRst;
	lastTouch	= false;
	touchNum		= 0;
}


adafruit_2050::~adafruit_2050(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
}


bool adafruit_2050::begin(void) { 

	theTFT = new maskableHX8357(cs,dc,rst);
	if (theTFT!=NULL) {
		theTFT->begin();										// Doesn't have fail mode here.
  		return true;
  }
  return false;
}


int adafruit_2050::width(void)																							{ return theTFT->width(); }
int adafruit_2050::height(void)																							{ return theTFT->height(); }
void adafruit_2050::startWrite(void)																					{ theTFT->startWrite(); }
void adafruit_2050::endWrite(void)																						{ theTFT->endWrite(); }
void adafruit_2050::setRotation(byte inRotation)																	{ theTFT->setRotation(inRotation); }
void adafruit_2050::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(inColor->getColor16()); }
void adafruit_2050::setTextColor(colorObj* tColor,colorObj* bColor) 										{ theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_2050::setTextSize(byte inSize)																		{ theTFT->setTextSize(inSize); }
void adafruit_2050::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }

rect	adafruit_2050::getTextRect(const char* inText) {

	rect bounds(0,0,0,0);
	int16_t	bX;
	int16_t	bY;
	uint16_t	bW;
	uint16_t	bH;
	
	bX = 0;
	bY = 0;
	bW = 0;
	bH = 0;
	theTFT->getTextBounds(inText,getCursorX(),getCursorY(),&bX,&bY,&bW,&bH);
	bounds.setRect(bX,bY,bW,bH);
	return bounds;
}

void adafruit_2050::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
//void adafruit_2050::setFont(const GFXfont* font)																	{ theTFT->setFontAdafruit(); }
void adafruit_2050::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y)); }
void adafruit_2050::drawText(const char* inText)																	{ theTFT->print(inText); }
void adafruit_2050::fillScreen(colorObj* inColor)																	{ theTFT->fillScreen(inColor->getColor16()); }
void adafruit_2050::fillRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->fillRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_2050::drawRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->drawRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_2050::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_2050::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_2050::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_2050::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_2050::drawCircle(int x,int y,int diam, colorObj* inColor)									{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_2050::drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor)				{ theTFT->drawCircleHelper(gX(x),gY(y),rad,corner,inColor->getColor16()); }
void adafruit_2050::fillCircle(int x,int y,int diam, colorObj* inColor)									{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_2050::drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor) 					{ theTFT->drawTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void adafruit_2050::fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor)					{ theTFT->fillTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void adafruit_2050::drawVLine(int x,int y,int height,colorObj* inColor)									{ theTFT->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_2050::drawHLine(int x,int y,int width,colorObj* inColor)										{ theTFT->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_2050::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void adafruit_2050::drawPixel(int x,int y,colorObj* inColor)													{ theTFT->drawPixel(gX(x),gY(y),inColor->getColor16()); }


point adafruit_2050::getPoint(void) {

	/*
	TS_Point	adaPoint;
	point		lcPoint;

	adaPoint = cTS->getPoint();								// Grab a point from the hardware.
	if (!adaPoint.z) {											// If the z value is zero..
		return lastTouchPt;										// Its a bogus touch! Return the last point.
	}																	//
	switch (theTFT->getRotation()) {							// For each type of rotation..
		case INV_PORTRAIT:										// INV_PORTRAIT is what we get by default.
			lcPoint.x = adaPoint.x;								// Load the x value into our lcPoint.		
			lcPoint.y = adaPoint.y;								// Load the y value into our lcPoint.
		break;														// And we waltz off.
		case INV_LANDSCAPE:										// INV_LANDSCAPE..
			lcPoint.x = adaPoint.y;								// x gets old y.
			lcPoint.y = theTFT->height() - adaPoint.x;	// Load the new y flipped around the y axis.
		break;														// Time to go. Better test this later, looks odd to me.
		case PORTRAIT:												// PORTRAIT I know this one works. I use it every damn day.
			lcPoint.x = theTFT->width() - adaPoint.x;		// Flip around the x axis.
			lcPoint.y = theTFT->height() - adaPoint.y;	// Flip around the y axis.
		break;														// And go!
		case LANDSCAPE:											// LANDSCAPE Another one I need to recheck. Its got some weird stuff in there too!
			lcPoint.x = theTFT->width() - adaPoint.y;		// Flip around the new x axis.
			lcPoint.y = adaPoint.x;								// y gets old x.
		break;														// Pack your bags and move on!
	}																	//
	lastTouchPt = lcPoint;										// Save it for later. (We cover bogus values with this.) I say "we" like anyone else is ever here..
	return lcPoint;												// And return our result!
	*/
}



bool adafruit_2050::touched(void) { return false; }

