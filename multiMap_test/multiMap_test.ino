#include <multiMap.h>
#include <blinker.h>

#define NUM_MAPPERS 3

multiMap testMapper[NUM_MAPPERS];
blinker  aBlinker;
char command[80];
byte index;

void setup(void) {
  index = 0;
  Serial.begin(9600);
  Serial.setTimeout(100);
  //aBlinker.init();
}

boolean checkMapperNum(int mapperNum) {

  return mapperNum==1 || mapperNum==2 || mapperNum==3;
}


void parseCommand(void) {

  int  value1;
  float value2, value3;


  if (!strcmp("add",command)) {
    value1 = Serial.parseInt();
    value2 = Serial.parseFloat();
    value3 = Serial.parseFloat();
    if (checkMapperNum(value1)) {
      testMapper[value1-1].addPoint(value2,value3);
      Serial.print("Added point : ");
      Serial.print(value2);
      Serial.print(", ");
      Serial.print(value3);
      Serial.print(" to mapper ");
      Serial.println(value1);
    }
  } 
  else if (!strcmp("view",command)) {
    value1 = Serial.parseInt();
    if (checkMapperNum(value1)) {
      testMapper[value1-1].outputList();
    }
  }  
  else if (!strcmp("map",command)) {
    value1 = Serial.parseInt();
    value2 = Serial.parseFloat();
    if (checkMapperNum(value1)) {
      value3 = testMapper[value1-1].Map(value2);
      Serial.print("Mapping : ");
      Serial.print(value2);
      Serial.print(" to : ");
      Serial.print(value3);
      Serial.print(" with mapper ");
      Serial.println(value1);
    }
  } 
  else if (!strcmp("clear",command)) {
    value1 = Serial.parseInt();
    if (checkMapperNum(value1)) {
      testMapper[value1-1].clearMap();  
    }
  } 
  else {
    Serial.println(command);
    Serial.println("add m x y adds a point to mapper m.");
    Serial.println("view m lists the mapper m");
    Serial.println("map m x returns a mapped point from mapper m.");
    Serial.println("clear m empties the mapper m.");
  }
}


void loop(void) {

  byte inByte;

  idle();
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if (isalpha(inByte)) {
      command[index] = inByte;
      index++;
    } 
    else if (index>0) {
      command[index] = '\0';
      parseCommand();
      index = 0;
    }
  }
}







