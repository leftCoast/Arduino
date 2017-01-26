#include <adafruit_376_Obj.h>
#include <TouchScreen.h>


adafruit_376_Obj::adafruit_376_Obj(void)
  :displayObj(true,true,true,true,false) {

  cTS = NULL;
  theTFT = NULL;
}

adafruit_376_Obj::~adafruit_376_Obj(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
	if (cTS!=NULL) {
		delete cTS;
		cTS = NULL;
	}
}


boolean adafruit_376_Obj::dispObjBegin(void) { 

	uint16_t identifier;
	
  theTFT = new Adafruit_TFTLCD();
  if (theTFT!=NULL) {
    cTS = new TouchScreen(XP, YP, XM, YM, 300); // 300 is resistance.
    if (cTS!=NULL) {
    	identifier = theTFT->readID();
  		if(	identifier == 0x9325
  				||identifier == 0x9328
  				||identifier == 0x7575
  			 ) {
    		theTFT->begin(identifier);	// Doesn't have fail mode here.
    		return true;	
    	}
    }
  }
  if (theTFT!=NULL) {
	 delete theTFT;
	 theTFT = NULL;
  }
  if (cTS!=NULL) {
    delete cTS;
    cTS = NULL;
  }
  return false;
}

void adafruit_376_Obj::setRotation(byte inRotation)      { theTFT->setRotation(inRotation); }
void adafruit_376_Obj::setTextColor(colorObj* inColor)   { theTFT->setTextColor(inColor->getColor16()); }
void adafruit_376_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_376_Obj::setTextSize(byte inSize)          { theTFT->setTextSize(inSize); }
void adafruit_376_Obj::setTextWrap(boolean wrap) { theTFT->setTextWrap(wrap); }
void adafruit_376_Obj::setCursor(word inX,word inY) { theTFT->setCursor(inX,inY); }
void adafruit_376_Obj::drawText(char* inText)            { theTFT->println(inText); }
void adafruit_376_Obj::fillScreen(colorObj* inColor)     { theTFT->fillScreen(inColor->getColor16()); }
void adafruit_376_Obj::fillRect(word locX,word locY,word width,word height,colorObj* inColor) { theTFT->fillRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_376_Obj::drawRect(word locX,word locY,word width,word height,colorObj* inColor) { 
	
	Serial.print(F("drawRect() x, y, h, w,color "));Serial.print(locX);Serial.print(", ");Serial.print(locY);
	Serial.print(", ");Serial.print(width);Serial.print(", ");Serial.print(height);
	Serial.print(", ");Serial.println(inColor->getColor16(),BIN);
	theTFT->drawRect(locX, locY, width, height,inColor->getColor16());
}
void adafruit_376_Obj::fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theTFT->fillRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_376_Obj::drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theTFT->drawRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_376_Obj::drawCircle(word locX,word locY,word inDiam, colorObj* inColor) { drawRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_376_Obj::fillCircle(word locX,word locY,word inDiam, colorObj* inColor) { fillRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_376_Obj::drawVLine(word locX,word locY,word height,colorObj* inColor) { theTFT->drawFastVLine(locX,locY,height,inColor->getColor16()); }
void adafruit_376_Obj::drawHLine(word locX,word locY,word width,colorObj* inColor) { theTFT->drawFastHLine(locX,locY,width,inColor->getColor16()); }
void adafruit_376_Obj::drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor) { theTFT->drawLine(locX,locY,locX2,locY2,inColor->getColor16()); }
 
  
void adafruit_376_Obj::drawPixel(word x,word y,colorObj* pColor) { theTFT->drawPixel(x,y,pColor->getColor16()); }
	 
point adafruit_376_Obj::getPoint(void) {

  TSPoint adaPoint;
  point		lcPoint;

  adaPoint = cTS->getPoint();
  switch (theTFT->getRotation()) {
  case INV_PORTRAIT:
    break;
  case INV_LANDSCAPE:
    swap(adaPoint.x,adaPoint.y);
    adaPoint.x = adaPoint.x;
    adaPoint.y = theTFT->height() - adaPoint.y;
    break;
  case PORTRAIT:
    adaPoint.x = theTFT->width() - adaPoint.x;
    adaPoint.y = theTFT->height() - adaPoint.y;
    break;
  case LANDSCAPE:
    swap(adaPoint.x,adaPoint.y);
    adaPoint.x = theTFT->width() - adaPoint.x;
    adaPoint.y = adaPoint.y;
    break;
  }
  lcPoint.x = adaPoint.x;
  lcPoint.y = adaPoint.y;
  return lcPoint;
}


boolean adafruit_376_Obj::touched(void) { 

	TSPoint adaPoint;
	
	digitalWrite(13, HIGH);
  adaPoint = cTS->getPoint();
  digitalWrite(13, LOW);
	return (adaPoint.z > MINPRESSURE && adaPoint.z < MAXPRESSURE);
}

 
