#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <neoPixel.h>

neoPixel theLights(4,3);

void setup() {
  theLights.begin();
  theLights.setPixelColor(0,&white);
  theLights.setPixelColor(1,&white);
  theLights.setPixelColor(2,&white);
  theLights.setPixelColor(3,&white);
  theLights.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
