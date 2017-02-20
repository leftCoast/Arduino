 #include "neoPixel.h"


 neoPixel::neoPixel(uint16_t n, uint8_t p, uint8_t t)
   :  Adafruit_NeoPixel(n,p,t) {  }
   
   
 neoPixel::~neoPixel() {  }


void neoPixel::setPixelColor(uint16_t n,colorObj* inColor) { 
    
    if (isRGBType()) {
    	Adafruit_NeoPixel::setPixelColor(n,inColor->getRed(),inColor->getGreen(),inColor->getBlue());
    } else {
			byte r = inColor->getRed();
			byte g = inColor->getGreen();
			byte b = inColor->getBlue();
			byte	minColor = min(r,g);
			minColor = min(minColor,b);
			Adafruit_NeoPixel::setPixelColor(n,r-minColor,g-minColor,b-minColor,minColor);
		}
 }
  
  
colorObj neoPixel::getPixelColor(uint16_t n) {
   
   uint32_t packColor;
   uint8_t* colorPtr;
   colorObj result;
   
   packColor = Adafruit_NeoPixel::getPixelColor(n);
   colorPtr = (uint8_t*) &packColor;
   if (isRGBType()) {
   	 result.setColor(colorPtr[2],colorPtr[1],colorPtr[0]);
   } else {	// 4 color..
   	 result.setColor(colorPtr[2]+colorPtr[3],
   					colorPtr[1]+colorPtr[3],
   					colorPtr[0]+colorPtr[3]);
   }
   return result;
}


void neoPixel::setAll(colorObj* color) {

  for (int i = 0; i < numPixels(); i++) {
  	 setPixelColor(i, color);
  }
}
