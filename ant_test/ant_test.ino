        

#include "antThorax.h"

#define buttonPin 2

antThorax thorax;

//char command[20];
//byte index;


void setup() { 

  pinMode(buttonPin, INPUT);
  //index = 0;
  //Serial.begin(9600);            // Set up for commands as default.
  //Serial.setTimeout(100);
  //aBlinker.init();
  thorax.startSerial();
  
  // Skip doing this through the serial port.. Lets hardcode it for now..
  thorax.addMapperPoint( 0, 0, 0);
  thorax.addMapperPoint( 0, 5, 0.66);
  thorax.addMapperPoint( 0, 20, 1);
  thorax.addMapperPoint( 0, 25, 1);
  thorax.addMapperPoint( 0, 45, 0.66);
  thorax.addMapperPoint( 0, 55, -0.66);
  thorax.addMapperPoint( 0, 75, -1);
  thorax.addMapperPoint( 0, 95, -0.66);
  thorax.addMapperPoint( 0, 100, 0);

  thorax.addMapperPoint( 1, 15, -1);
  thorax.addMapperPoint( 1, 20, 0.33);
  thorax.addMapperPoint( 1, 25, 0.5);
  thorax.addMapperPoint( 1, 75, 0.5);
  thorax.addMapperPoint( 1, 77.5, -.5);
  thorax.addMapperPoint( 1, 85, -1);

  thorax.addMapperPoint( 2, 15, -1);
  thorax.addMapperPoint( 2, 20, .25);
  thorax.addMapperPoint( 2, 25, 0.5);
  thorax.addMapperPoint( 2, 75, 0.5);
  thorax.addMapperPoint( 2, 77.5, -0.5);
  thorax.addMapperPoint( 2, 85, -1);
}


/*
void commandCheck(void) {

  int  value1;
  float value2, value3;
  byte inByte;

  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (isalpha(inByte)) {
      command[index] = inByte;
      index++;
    } 
    else if (index>0) {
      command[index] = '\0';
      index = 0;
      if (!strcmp("add",command)) {
        value1 = Serial.parseInt();
        value2 = Serial.parseFloat();
        value3 = Serial.parseFloat();
        thorax.addMapperPoint(value1,value2,value3);
        Serial.print("Added point : ");
        Serial.print(value2);
        Serial.print(", ");
        Serial.print(value3);
        Serial.print(" to mapper ");
        Serial.println(value1);
      } 
      else if (!strcmp("view",command)) {
        value1 = Serial.parseInt();
        thorax.mapperPointsToSerial(value1);
      }  
      else if (!strcmp("map",command)) {
        value1 = Serial.parseInt();
        value2 = Serial.parseFloat();
        value3 = thorax.mapPoint(value1,value2);
        Serial.print("Mapping : ");
        Serial.print(value2);
        Serial.print(" to : ");
        Serial.print(value3);
        Serial.print(" with mapper ");
        Serial.println(value1);

      }
      else if (!strcmp("dump",command)) {
        value1 = Serial.parseInt();
        Serial.print("Data for mapper : ");
        Serial.print(value1);
        Serial.println(" as (Time, value)");
        thorax.dumpMapperToSerial(value1);
      } 
      else if (!strcmp("clear",command)) {
        value1 = Serial.parseInt();
        thorax.clearMapper(value1);  
      } 
      else {
        Serial.println(command);
        Serial.println("add m x y adds point to mapper m.");
        Serial.println("view m lists mapper m");
        Serial.println("map m x returns a mapped point from mapper m.");
        Serial.println("clear m empties mapper m.");
        Serial.println("dump m returns the curve for mapper m.");
      }
    }
  }
}



void loop() {

  aBlinker.idle();
  if (!digitalRead(buttonPin)) {
    if (thorax.stepState != walkingFwd
      && thorax.stepState != startingToWalk) {    // If we are not walking..
      Serial.end();                          // kill the serial port.
      thorax.startSerial();                  // Let the thorax have the port..
    }
    thorax.walkFwd();
  } 
  else {
    if (thorax.stepState == walkingFwd) {
      thorax.park();
      thorax.stopSerial();            // Done with servos for now..
      Serial.begin(9600);            // Set up for commands.
      Serial.setTimeout(100);
    } 
    else
      commandCheck();
  }
}
*/

void loop() {

  if (!digitalRead(buttonPin)) {
    thorax.walkFwd();
  } 
  else {
    //thorax.fold();
    thorax.park();
  }
}































