#ifndef neoPixel_h
#define neoPixel_h

#include <Adafruit_NeoPixel.h>
#include <colorObj.h>


class neoPixel : public Adafruit_NeoPixel {
   
      public:

  // Constructor: number of LEDs, pin number, LED type.
  // See the ADAfruit_NeoPixel.h for info on these things.
  neoPixel(uint16_t n, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);
  ~neoPixel();
  
  void     setPixelColor(uint16_t n,colorObj* inColor);
  colorObj getPixelColor(uint16_t n);
  void	  setAll(colorObj* color);		// 'Cause its soo handy.
};

#endif