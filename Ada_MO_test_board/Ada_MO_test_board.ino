#include <Adafruit_NeoPixel.h>

//#include <chainPixels.h>
//#include <liteLine.h>
#include <neoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>

neoPixel liteStick(8,5);
colorObj mix(black);

int f = 2;

void setup() {

  
  //mix = mix.blend(&white,5);
  //mix = mix.blend(&yellow,10);
   
  liteStick.begin();
}

void setStick(colorObj* inColor) {

  /*
   for (int i=0;i<8;i++) {
    liteStick.setPixelColor(i,inColor);
  }
  liteStick.show();
  */
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
}



void loop() {
  
  
 /* 
   
  for (int i=0;i<=100;i++) {
    mix = mix.blend(&red,i);
    setStick(&mix);
    delay(2);
  }
  delay(100);
  for (int i=0;i<=100;i++) {
    mix = mix.blend(&black,i);
    setStick(&mix);
    delay(2);
  }
  delay(100);
  */
  //digitalWrite(8, HIGH);  
  //delay(f);             
  //digitalWrite(8, LOW);    
  //delay(f*10);              
}
