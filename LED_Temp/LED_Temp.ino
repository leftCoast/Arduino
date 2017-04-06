#include <idlers.h>
#include <timeObj.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

void setup() {
  matrix.begin(0x70);              // pass in the address
  matrix.setRotation(2);

}

void loop() {
  idle();
  
}
