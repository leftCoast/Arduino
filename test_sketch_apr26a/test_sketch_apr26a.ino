#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <blinker.h>
#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>


#include <Adafruit_NeoPixel.h>

neoPixel lightbar(8, 3);
colorObj dimmer;



void setup() {
  lightbar.begin();
  pinMode(13, OUTPUT);
  dimmer.setColor (&blue);
  dimmer.blend(&red, 30);
  BlueLight();
}

void BlueLight (void) {

  int i;

  for (i = 0; i <= 7; i++) {
    lightbar.setPixelColor(i, &dimmer);
  }
  lightbar.show();
}


void blink100times (void) {
  int i;
  for (i = 1; i <= 10; i++) {
    digitalWrite(13, HIGH);
    lightbar.setAll(&dimmer);
    lightbar.show();
    delay(50);
    digitalWrite(13, LOW);
    lightbar.setAll(&black);
    lightbar.show();
    delay(50);
  }
}

void loop() {
  blink100times();
  //while (true);
}
