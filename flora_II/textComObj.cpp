#include "textComObj.h"
#include "parameters.h"
#include "pumpObj.h"


enum commands { noCommand, resetAll, showParams, setSetPoint, setWTime, setSTime, setName, setPump, setPercent, setPeriod };


textComObj textComs;

textComObj::textComObj(void) { }


textComObj::~textComObj(void) { }

// Link command bytes to text sstrings that can be typed.
void textComObj::begin(void) {

  mParser.addCmd(resetAll, "reset");
  mParser.addCmd(showParams, "see");
  mParser.addCmd(setSetPoint, "moisture");
  mParser.addCmd(setWTime, "wtime"); 
  mParser.addCmd(setSTime, "stime");
  mParser.addCmd(setName, "name");
  mParser.addCmd(setPump, "pump");
  mParser.addCmd(setPercent, "percent");
  mParser.addCmd(setPeriod, "period");
}


          
void textComObj::checkTextCom(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand    : break;
      case resetAll     : initParams();     break;
      case showParams   : printParams();    break;
      case setSetPoint  : setMoisture();    break;
      case setWTime     : setWaterTime();   break;
      case setSTime     : setSoakTime();    break;
      case setName      : setPlantName();   break;
      case setPump      : turnPump();       break;
      case setPercent   : setPWMPercent();  break;
      case setPeriod    : setPWMPeriod();   break;
      default           : Serial.println("Sorry, have no idea what you want.");
    }
  }
}


void textComObj::initParams(void) {

  ourParamObj.floraReset();
  Serial.println("Params set to defaults.");
}

          
void textComObj::printParams(void) {

  Serial.print("Parameters for : ");Serial.println(ourParamObj.getName());
  Serial.print("moisture  : ");Serial.print(ourParamObj.getMoisture());Serial.println(" %");
  Serial.print("waterTime : ");Serial.print(ourParamObj.getWaterTime());Serial.println(" ms");
  Serial.print("soakTime  : ");Serial.print(ourParamObj.getSoakTime());Serial.println(" ms");
  Serial.print("percent   : ");Serial.print(ourParamObj.getPWMPercent());Serial.println(" %");
  Serial.print("period    : ");Serial.print(ourParamObj.getPWMPeriod());Serial.println(" ms");
  Serial.println();
}

          
void textComObj::setMoisture(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    ourParamObj.setMoisture(newVal);
    Serial.print("Moisture set point now set to ");
    Serial.println(ourParamObj.getMoisture());
  }
}


void textComObj::setWaterTime(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi(paramBuff);
    free(paramBuff);
    ourParamObj.setWaterTime(newVal);
    Serial.print("Watering tmie now set to ");
    Serial.println(ourParamObj.getWaterTime());
  }
}


void textComObj::setSoakTime(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    ourParamObj.setSoakTime(newVal);
    Serial.print("Soak time now set to ");
    Serial.println(ourParamObj.getSoakTime());
  }
}


void textComObj::setPlantName(void) {

  char* charBuff;
  
  if (mParser.numParams()) {
    charBuff = mParser.getParamBuff();
    ourParamObj.setName(charBuff);
    free(charBuff);
    charBuff = ourParamObj.getName();
    Serial.print("Plant name is now set to ");
    Serial.println(charBuff);
    free(charBuff);
  }
}


void textComObj::setPWMPercent(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    ourParamObj.setPWMPercent(newVal);
    Serial.print("Percent now set to ");
    Serial.println(ourParamObj.getPWMPercent());
  }
}


void textComObj::setPWMPeriod(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    Serial.print("Param buff :");Serial.println(paramBuff);
    newVal = atoi (paramBuff);
    Serial.print("newVal :");Serial.println(newVal);
    free(paramBuff);
    ourParamObj.setPWMPeriod(newVal);
    Serial.print("Period now set to ");
    Serial.println(ourParamObj.getPWMPeriod());
  }
}


void textComObj::turnPump(void) {

  int numChars;
  char* paramBuff;
  
  pumpCom = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    numChars = strlen(paramBuff);
    for (int i=0;i<numChars;i++) {
      paramBuff[i] = toupper(paramBuff[i]);
    }
    if (!strcmp(paramBuff,"ON")) {
      pumpCom = true;
    }
    free(paramBuff);
  }
}
