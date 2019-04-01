#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <RCReciver.h>
#include <servo.h>

Adafruit_NeoPixel aLED(1,3,NEO_GRB + NEO_KHZ800);
timeObj sTimer(1000);
servo marco01(9);
int suck;
mapper suck2Servo(472,450,0,100);
bool servoHigh = false;
colorMapper aColorMap(&green,&red);


void setup() {
  Serial.begin(9600);
  aLED.begin();
  sTimer.start();
  servoHigh = false;
}

//472 = 0; 450 = suck hard
void loop() {
  
  colorObj aColor;
  
  idle();
  suck = analogRead(A0);  
  //Serial.println(suck2Servo.Map(suck));
  marco01.setServo(suck2Servo.Map(suck));
  aColor = aColorMap.Map(suck2Servo.Map(suck));
  //aLED.setPixelColor(0,255,255,255);
  aLED.setPixelColor(0  ,aColor.getRed(),aColor.getGreen(),aColor.getBlue());
  //aLED.setPixelColor(1,&aColor);
  aLED.show();
   
}
