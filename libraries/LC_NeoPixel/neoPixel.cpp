 #include "neoPixel.h"

colorObj red(RED);
colorObj blue(BLUE);
colorObj white(WHITE);
colorObj black(BLACK);
colorObj green(GREEN);


 neoPixel::neoPixel(uint16_t n, uint8_t p, uint8_t t)
   :  Adafruit_NeoPixel(n,p,t) {  }
   
   
 neoPixel::~neoPixel() {  }


void neoPixel::setPixelColor(uint16_t n,colorObj* inColor) { 
    
    Adafruit_NeoPixel::setPixelColor(n,inColor->getRed(),inColor->getGreen(),inColor->getBlue());
 }
  
  
colorObj neoPixel::getPixelColor(uint16_t n) {
   
   uint32_t packColor;
   uint8_t*    colorPtr;
   
   packColor = Adafruit_NeoPixel::getPixelColor(n);
   colorPtr = (uint8_t*) &packColor;
   colorObj result(colorPtr[2],colorPtr[1],colorPtr[0]);
   return result;
}

