#include <calculator.h>

calculator mCalc;

void setup() {
  Serial.begin(9600);
  delay(5000);
  mCalc.buttonClick("1");
  mCalc.buttonClick("4");
  mCalc.buttonClick("enter");
  mCalc.buttonClick("2");
  mCalc.buttonClick("x");
  Serial.println(mCalc.getX());
}

void loop() {
  // put your main code here, to run repeatedly:

}
