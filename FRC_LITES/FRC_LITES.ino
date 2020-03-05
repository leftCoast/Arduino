#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      75
#define SPARKLE_LOW_LIFE  25
#define SPARKLE_LIFE      1000
#define SPARKLE_ODDS      5 // x+1 to one.  IE 1 = 1+1 to 1 or 1:2 chance.
#define SPARKLE_COLORS    25
#define MAX_SPARKLES      120

neoPixel lites(NUM_LEDS,PIXEL_PIN);

void setup(void) {

  colorObj  aColor;
  lites.begin();
  aColor.setColor(LC_BLUE);
  aColor.blend(&green,70);
  aColor.blend(& black,50);
  lites.setAll(&aColor);
  lites.setPixelColor(0,&white);
  lites.setPixelColor(1,&white);
  lites.setPixelColor(2,&white);
  
}

void loop(void) {

  lites.roll();
  lites.show();
  delay(25); 
 }
