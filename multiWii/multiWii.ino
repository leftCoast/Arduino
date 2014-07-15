#include "multiWii.h"

int led =  RED_LED;
int led2 = GREEN_LED;
int led3 = BLUE_LED;
int led4 = AMBER_LED;

void setup() {                

   pinMode(led, OUTPUT);
   pinMode(led2, OUTPUT); 
   pinMode(led3, OUTPUT);
   pinMode(led4, OUTPUT);
}

void loop() {
   digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(led2, HIGH);
   digitalWrite(led3, HIGH);
   digitalWrite(led4, HIGH);
   delay(100);               // wait for a second
   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
   digitalWrite(led2, LOW);
   digitalWrite(led3, LOW);
   digitalWrite(led4, LOW);
   delay(100);               // wait for a second
}

