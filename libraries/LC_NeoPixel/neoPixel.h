#ifndef neoPixel_h
#define neoPixel_h

#include <Adafruit_NeoPixel.h>
#include <colorObj.h>

// This is mainly so you can do the neopixle thing using our colorObj stuff.
// This allows all sorts of color function possibilities. And makes life 
// generally, a lot easier.

class neoPixel : public Adafruit_NeoPixel {
   
      public:

  // Constructor: number of LEDs, pin number, LED type.
  // See the ADAfruit_NeoPixel.h for info on these things.
  neoPixel(uint16_t n, uint8_t p, uint8_t t=NEO_GRB + NEO_KHZ800);
  ~neoPixel();
  
  void     setPixelColor(uint16_t n,colorObj* inColor);
  //void		 setPixelColorW(uint16_t n,colorObj* inColor);
  colorObj getPixelColor(uint16_t n);
  //colorObj getPixelColorW(uint16_t n);
  void	  setAll(colorObj* color);		// 'Cause its soo handy.
  //void	  setAllW(colorObj* color);
};

#endif