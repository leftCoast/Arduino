
#include <Adafruit_NeoPixel.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

neoPixel playToy(15,2);


void setup()
{
 playToy.begin();
 playToy.show();
 delay(500);
playToy.setPixelColor(9,&red); 
playToy.show();
delay(250);
playToy.setPixelColor(10,&blue); 
playToy.show();
delay(250);
playToy.setPixelColor(8,&green); 
playToy.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
