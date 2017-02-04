 #include "neoPixel.h"


 neoPixel::neoPixel(uint16_t n, uint8_t p, uint8_t t)
   :  Adafruit_NeoPixel(n,p,t) {  }
   
   
 neoPixel::~neoPixel() {  }


void neoPixel::setPixelColor(uint16_t n,colorObj* inColor) { 
    
    Adafruit_NeoPixel::setPixelColor(n,inColor->getRed(),inColor->getGreen(),inColor->getBlue());
 }
 
 void neoPixel::setPixelColorW(uint16_t n,colorObj* inColor) { 
    
    byte r = inColor->getRed();
    byte g = inColor->getGreen();
    byte b = inColor->getBlue();
    byte	minColor = min(r,g);
    minColor = min(minColor,b);
    Adafruit_NeoPixel::setPixelColor(n,r-minColor,g-minColor,b-minColor,minColor);
 }
  
  
colorObj neoPixel::getPixelColor(uint16_t n) {
   
   uint32_t packColor;
   uint8_t*    colorPtr;
   
   packColor = Adafruit_NeoPixel::getPixelColor(n);
   colorPtr = (uint8_t*) &packColor;
   colorObj result(colorPtr[2],colorPtr[1],colorPtr[0]);
   return result;
}


colorObj neoPixel::getPixelColorW(uint16_t n) {
   
   uint32_t packColor;
   uint8_t*    colorPtr;
   
   packColor = Adafruit_NeoPixel::getPixelColor(n);
   colorPtr = (uint8_t*) &packColor;
   colorObj result(colorPtr[2]+colorPtr[3],
   								 colorPtr[1]+colorPtr[3],
   								 colorPtr[0]+colorPtr[3]);
   return result;
}


void neoPixel::setAll(colorObj* color) {

  for (int i = 0; i < numPixels(); i++) {
  	 setPixelColor(i, color);
  }
}


void neoPixel::setAllW(colorObj* color) {

  for (int i = 0; i < numPixels(); i++) {
  	 setPixelColorW(i, color);
  }
}