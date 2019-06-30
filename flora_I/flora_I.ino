#include <Adafruit_DotStar.h>

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

// There is only one pixel on the board
#define NUMPIXELS 1 

//Use these pin definitions for the ItsyBitsy M4
#define DATAPIN    7
#define CLOCKPIN   8

Adafruit_DotStar px(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);



void setup() {

  px.begin(); // Initialize pins for output
  px.show();  // Turn all LEDs off ASAP
  
  pinMode(0, OUTPUT); //
  pinMode(1, OUTPUT); //
  pinMode(2, INPUT_PULLUP);  // Motor button
  pinMode(3, OUTPUT); // IN1 Motor
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT); // IN2Motor
  digitalWrite(4, LOW);
  pinMode(13, OUTPUT); //
  
  Serial.begin(57600);

}

void loop() {

  digitalWrite(13, !digitalRead(2));
  digitalWrite(3, !digitalRead(2));
}
