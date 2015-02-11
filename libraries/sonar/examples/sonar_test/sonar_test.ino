#include <sonarObj.h>
#include <idlers.h>

sonarObj sonar(3);      // ONLY PIN 2 or 3 CAN BE USED FOR SONAR.
float distance = -1;    // Flag, it should never be negative.

void setup() {
   
  Serial.begin(9600);
  sonar.hookup();         // Assembles the idler into the idler list.
}

void loop() {

  float temp;

  idle();                           // Lets all idlers do their things.
  temp = sonar.distanceCm();        // Grab the distance.
  if (temp!=distance) {             // If we see a change..
    distance = temp;                // Update the global one.
    Serial.print("distance cm : "); Serial.println(distance);   // Show it.
  }
}


