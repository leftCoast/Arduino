#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          120
#define PIXEL_PIN         2
#define DARK_PERCENT      75

neoPixel lites(NUM_LEDS, PIXEL_PIN);

colorObj backColor;

void setup() {

  //Serial.begin(9600);

  backColor.setColor(&blue);
  backColor.blend(&black, DARK_PERCENT);

  lites.begin();
  lites.setAll(&backColor);
  lites.show();
}


void loop() {
  // put your main code here, to run repeatedly:

}
