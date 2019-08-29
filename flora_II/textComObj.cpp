#include "textComObj.h"
#include "parameters.h"
#include "pumpObj.h"
#include "globals.h"

enum commands { noCommand, resetAll, showParams, showReadings, showGReadings, setMudMapLimit, setDryMapLimit, setSetPoint, setWTime, setSTime, setName, setPump, setPercent, setPeriod };


textComObj textComs;

textComObj::textComObj(void) { mAutoRead = false; }


textComObj::~textComObj(void) { }

// Link command bytes to text sstrings that can be typed.
void textComObj::begin(void) {

  hookup();
  mParser.addCmd(resetAll, "reset");
  mParser.addCmd(showParams, "see");
  mParser.addCmd(showReadings, "read");
  mParser.addCmd(showGReadings, "graph");
  mParser.addCmd(setMudMapLimit, "mud");
  mParser.addCmd(setDryMapLimit, "dry");
  mParser.addCmd(setSetPoint, "limit");
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
      case noCommand      : break;
      case resetAll       : initParams();     break;
      case showParams     : printParams();    break;
      case showReadings   : printReadings();  break;
      case showGReadings  : Serial.println("Graphing's disabled."); /* printGReadings(); */ break;
      case setSetPoint    : setDryLimit();    break;
      case setWTime       : setWaterTime();   break;
      case setSTime       : setSoakTime();    break;
      case setName        : setPlantName();   break;
      case setPump        : turnPump();       break;
      case setPercent     : setPWMPercent();  break;
      case setPeriod      : setPWMPeriod();   break;
      case setDryMapLimit : setDry();         break;
      case setMudMapLimit : setMud();         break;
      default             : Serial.println("Sorry, have no idea what you want.");
    }
  }
}


void textComObj::initParams(void) {

  ourParamObj.floraReset();
  Serial.println("Params set to defaults.");
}

          
void textComObj::printParams(void) {

  Serial.print("Parameters for : ");Serial.println(ourParamObj.getName());
  Serial.print("dryLimit  : ");Serial.print(ourParamObj.getDryLimit());Serial.println(" %");
  Serial.print("waterTime : ");Serial.print(ourParamObj.getWaterTime());Serial.println(" ms");
  Serial.print("soakTime  : ");Serial.print(ourParamObj.getSoakTime());Serial.println(" ms");
  Serial.print("percent   : ");Serial.print(ourParamObj.getPWMPercent());Serial.println(" %");
  Serial.print("period    : ");Serial.print(ourParamObj.getPWMPeriod());Serial.println(" ms");
  Serial.print("Dry       : ");Serial.print(ourParamObj.getDry());Serial.println(" mf");
  Serial.print("Mud       : ");Serial.print(ourParamObj.getMud());Serial.println(" mf");
  Serial.println();
}


void textComObj::printReadings(void) {

  int   timeVal;
  char* buff;
  
  timeVal = 0;
  mAutoRead = false;
  if (mParser.numParams()) {
    buff = mParser.getParam();
    timeVal = atoi (buff);
    free(buff);
  }
  readTimer.setTime(timeVal);
  readTimer.start();
  mAutoRead = timeVal>0;
  doPrintReadings();
}


void textComObj::doPrintReadings(void) {
  
    Serial.print("Temperature : ");Serial.print(tempC);Serial.println(" *C");
    Serial.print("Capacitive  : ");Serial.print(capread);Serial.println(" mf");
    Serial.print("Moisture    : ");Serial.print((int)moisture);Serial.println(" %");
    Serial.print("And we are  : ");
    switch (weAre) {
      case sitting  : Serial.println("sitting");  break;
      case watering : Serial.println("watering"); break;
      case soaking  : Serial.println("soaking");  break;
    }
    if (mAutoRead) readTimer.start();
}      


void textComObj::doPrintGReadings(void) {
  
    //Serial.print(tempC);Serial.print(" ");
    //Serial.print(capread);Serial.print(" ");
    Serial.print((int)moisture);
    Serial.println();
    if (mAutoRead) readTimer.start();
}      

void textComObj::printGReadings(void) {

  int   timeVal;
  char* buff;
  
  timeVal = 0;
  mAutoRead = false;
  if (mParser.numParams()) {
    buff = mParser.getParam();
    timeVal = atoi (buff);
  }
  readTimer.setTime(timeVal);
  readTimer.start();
  mAutoRead = timeVal>0;
}


void textComObj::setDryLimit(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    ourParamObj.setDryLimit(newVal);
    Serial.print("Moisture set point now set to ");
    Serial.println(ourParamObj.getDryLimit());
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


void textComObj::setDry(void) {

  char* buff;
  int   dryVal;
  
  if (mParser.numParams()) {
    buff = mParser.getParam();
    dryVal = atoi(buff);
    free(buff);
    ourParamObj.setDry(dryVal);
  } else {
    ourParamObj.setDry(capread);
  }
  Serial.print("Dry limit now set to ");
  Serial.println(ourParamObj.getDry());
}


void textComObj::setMud(void) {

  char* buff;
  int   mudVal;
  
  if (mParser.numParams()) {
    buff = mParser.getParam();
    mudVal = atoi(buff);
    free(buff);
    ourParamObj.setMud(mudVal);
  } else {
    ourParamObj.setMud(capread);
  }
  Serial.print("Mud limit now set to ");
  Serial.println(ourParamObj.getMud());
}


void textComObj::idle(void) {

  if (readTimer.ding()&&mAutoRead) {
    doPrintReadings();
    //doPrintGReadings();
  }
}
