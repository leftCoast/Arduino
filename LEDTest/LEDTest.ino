#include <neoPixel.h>


neoPixel theLEDs(8,8);

void setup() {

  colorObj  startColor;

  startColor.setColor(&blue);
  startColor.blend(&black,95);
  theLEDs.begin();
  theLEDs.setAll(&startColor);
  theLEDs.show();
}

void loop() {

  
}
