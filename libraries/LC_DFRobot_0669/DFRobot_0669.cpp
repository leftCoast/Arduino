#include <DFRobot_0669.h>

// **********************************************************
//								maskableILI9488
//	ILI9488_t3 with the ability to have a mask added.	
// **********************************************************


maskableILI9488::maskableILI9488(int cs, int rst)
	: ILI9488_t3(&SPI,cs,LC_DC,rst) { }

	
maskableILI9488::~maskableILI9488(void) {  }
	
	
void maskableILI9488::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			ILI9488_t3::drawPixel(x,y,color);
		}
	} else {
		ILI9488_t3::drawPixel(x,y,color);
	}
}

/*
void maskableILI9488::writePixel(int16_t x, int16_t y, uint16_t color) {

	if (gMask) {
		if (gMask->checkPixel(x,y)) {
			ILI9488_t3::writePixel(x,y,color);
		}
	} else {
		ILI9488_t3::writePixel(x,y,color);
	}
}
*/

void maskableILI9488::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

	int	lastX;
	int	lastY;
	
	if (gMask) {
		switch(gMask->checkRect(x,y,w,h)) {
			case unMasked			: ILI9488_t3::fillRect(x,y,w,h,color); break;
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
		ILI9488_t3::fillRect(x,y,w,h,color);
	}
}

// **********************************************************
//								DFRobot_0669	
// **********************************************************



DFRobot_0669::DFRobot_0669(byte inCS,byte inRst)
  :displayObj(true,true,false,true,false) {

	//cTS			= NULL;
	theTFT		= NULL;
	cs				= inCS;
	rst			= inRst;
	lastTouch	= false;
	touchNum		= 0;
}


DFRobot_0669::~DFRobot_0669(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
	/*
	if (cTS!=NULL) {
		delete cTS;
		cTS = NULL;
	}
	*/
}


bool DFRobot_0669::begin(void) { 

	Serial.println("Allocation of TFT");
	Serial.print("CS : ");Serial.println(cs);
	Serial.print("RST : ");Serial.println(rst);
	theTFT = new maskableILI9488(cs,rst);	// Have a go at allocating the display object.
	if (theTFT!=NULL) {							// If we got one?
		Serial.println("Got the TFT");
		Serial.print("theTFT addr : ");Serial.println((uint32_t)theTFT);
		theTFT->begin();
		Serial.println("TFT begin called.");
		return true;					// Hack for no touch. Debugging.
    /*
    cTS = new GT911CapTouch();				// Have a go at allocating a touch object.
    if (cTS!=NULL) {								// We got both now?
    	theTFT->begin();							// Fire up the TFT. Doesn't have fail mode here.
    	if (cTS->begin(40)) {					// 40 comes from example code.
    		return true;							// If everything checks out? We exit with a success.
    	}												//
    }	
    */												//
	}
  	Serial.println("No TFT");												// At this point something went wrong. Recycle everything.
  if (theTFT!=NULL) {							// Have a TFT thing?
	 delete theTFT;								// Delete it.
	 theTFT = NULL;								// Note it.
  }													// 
  /*
  if (cTS!=NULL) {								// Have a touch object?
    delete cTS;									// Delete it.
    cTS = NULL;									// Note it.
  }													//
  */
  return false;									// Return false, we failed.
}


int DFRobot_0669::width(void)																							{ return theTFT->width(); }
int DFRobot_0669::height(void)																						{ return theTFT->height(); }
//void DFRobot_0669::startWrite(void)																				{ }
//void DFRobot_0669::endWrite(void)																					{ }
void DFRobot_0669::setRotation(byte inRotation)																	{ theTFT->setRotation((uint8_t)inRotation); }
void DFRobot_0669::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(inColor->getColor16()); }
void DFRobot_0669::setTextColor(colorObj* tColor,colorObj* bColor) 										{ theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void DFRobot_0669::setTextSize(byte inSize)																		{ theTFT->setTextSize(inSize); }
void DFRobot_0669::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }
void DFRobot_0669::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
void DFRobot_0669::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y)); }
void DFRobot_0669::drawText(const char* inText)																	{ theTFT->print(inText); }
void DFRobot_0669::fillScreen(colorObj* inColor)																{ theTFT->fillScreen(inColor->getColor16()); }
void DFRobot_0669::fillRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->fillRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void DFRobot_0669::drawRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->drawRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void DFRobot_0669::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void DFRobot_0669::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void DFRobot_0669::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void DFRobot_0669::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void DFRobot_0669::drawCircle(int x,int y,int diam, colorObj* inColor)									{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void DFRobot_0669::drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor)				{ theTFT->drawCircleHelper(gX(x),gY(y),rad,corner,inColor->getColor16()); }
void DFRobot_0669::fillCircle(int x,int y,int diam, colorObj* inColor)									{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void DFRobot_0669::drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor) 				{ theTFT->drawTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void DFRobot_0669::fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor)					{ theTFT->fillTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void DFRobot_0669::drawVLine(int x,int y,int height,colorObj* inColor)									{ theTFT->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void DFRobot_0669::drawHLine(int x,int y,int width,colorObj* inColor)									{ theTFT->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void DFRobot_0669::drawLine(int x,int y,int x2,int y2,colorObj* inColor)								{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void DFRobot_0669::drawPixel(int x,int y,colorObj* inColor)													{ theTFT->drawPixel(gX(x),gY(y),inColor->getColor16()); }


point DFRobot_0669::getPoint(void) {

	//TS_Point	GT911Point;
	point		lcPoint;
/*
	GT911Point = cTS->getPoint();								// Grab a point from the hardware.
	if (!GT911Point.z) {											// If the z value is zero..
		return lastTouchPt;										// Its a bogus touch! Return the last point.
	}																	//
	switch (theTFT->getRotation()) {							// For each type of rotation..
		case INV_PORTRAIT:										// INV_PORTRAIT is what we get by default.
			lcPoint.x = GT911Point.x;								// Load the x value into our lcPoint.		
			lcPoint.y = GT911Point.y;								// Load the y value into our lcPoint.
		break;														// And we waltz off.
		case INV_LANDSCAPE:										// INV_LANDSCAPE..
			lcPoint.x = GT911Point.y;								// x gets old y.
			lcPoint.y = theTFT->height() - GT911Point.x;	// Load the new y flipped around the y axis.
		break;														// Time to go. Better test this later, looks odd to me.
		case PORTRAIT:												// PORTRAIT I know this one works. I use it every damn day.
			lcPoint.x = theTFT->width() - GT911Point.x;		// Flip around the x axis.
			lcPoint.y = theTFT->height() - GT911Point.y;	// Flip around the y axis.
		break;														// And go!
		case LANDSCAPE:											// LANDSCAPE Another one I need to recheck. Its got some weird stuff in there too!
			lcPoint.x = theTFT->width() - GT911Point.y;		// Flip around the new x axis.
			lcPoint.y = GT911Point.x;								// y gets old x.
		break;														// Pack your bags and move on!
	}																	//
	lastTouchPt = lcPoint;										// Save it for later. (We cover bogus values with this.) I say "we" like anyone else is ever here..
	*/
	return lcPoint;												// And return our result!

}


bool DFRobot_0669::touched(void) { return false; /*return cTS->touched();*/ }

