#include "textComObj.h"
#include "parameters.h"
#include "pumpObj.h"
#include "globals.h"
#include "UI.h"

enum commands { noCommand,
                resetAll,
                showParams,
                showReadings,
                showGReadings,
                setMudMapLimit,
                setDryMapLimit,
                setSetPoint,
                setWTime,
                setSTime,
                setName,
                setPump,
                setPercent,
                setPeriod,
                logCom,

                listDir
              };


textComObj textComs;

textComObj::textComObj(void) {

  mAutoRead = false;
  mHandheld = false;
}


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
  mParser.addCmd(logCom, "log");

  mParser.addCmd(listDir, "ls");
}


void textComObj::textOut(char aChar) {

  if (!mHandheld) {
    Serial.print(aChar);
  } else {
    Serial1.print(aChar);
  }
}


void textComObj::textOut(char* buff) {

  if (!mHandheld) {
    Serial.print(buff);
  } else {
    Serial1.print(buff);
  }
}

/*
void textComObj::textOut(int val,int format) {

  if (!mHandheld) {
    Serial.print(val,format);
  } else {
    Serial1.print(val,format);
  }
}
*/

void textComObj::textOut(uint32_t val,int format) {

  if (!mHandheld) {
    Serial.print(val,format);
  } else {
    Serial1.print(val,format);
  }
}


void textComObj::textOutln(uint32_t val,int format) {

  if (!mHandheld) {
    Serial.println(val,format);
  } else {
    Serial1.println(val,format);
  }
}


void textComObj::textOutln(char* buff) {

  if (!mHandheld) {
    if (buff) {
      Serial.println(buff);
    } else {
      Serial.println();
    }
  } else {
    if (buff) {
      Serial1.println(buff);
    } else {
      Serial1.println();
    }
  }
}


void textComObj::checkTextCom(char inChar) {

  int   command;

  if (mHandheld || Serial.available()) {
    if (!mHandheld) {
      inChar = Serial.read();
    }
    textOut(inChar);
    //Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {
      case noCommand      : break;
      case resetAll       : initParams();     break;
      case showParams     : printParams();    break;
      case showReadings   : printReadings();  break;
      case showGReadings  : textOutln("Graphing's disabled."); /* printGReadings(); */ break;
      case setSetPoint    : setDryLimit();    break;
      case setWTime       : setWaterTime();   break;
      case setSTime       : setSoakTime();    break;
      case setName        : setPlantName();   break;
      case setPump        : turnPump();       break;
      case setPercent     : setPWMPercent();  break;
      case setPeriod      : setPWMPeriod();   break;
      case setDryMapLimit : setDry();         break;
      case setMudMapLimit : setMud();         break;
      case logCom         : logCommand();     break;
      case listDir        : listDirectory();  break;
      default             : textOutln("Sorry, have no idea what you want.");
    }
  }
}


void textComObj::handleTextCom(char* buff) {
  
  int i;

  i = 0;
  mHandheld = true;
  while (buff[i] != '\0') {
    checkTextCom(buff[i]);
    i++;
  }
  checkTextCom('\n');
  mHandheld = false;
}


void textComObj::initParams(void) {

  ourParamObj.floraReset();
  ourDisplay.deleteLog();
  textOutln("Params set to defaults.");
}


void textComObj::printParams(void) {

  textOut("Parameters for : "); textOutln(ourParamObj.getName());
  textOut("dryLimit  : "); textOut(ourParamObj.getDryLimit()); textOutln(" %");
  textOut("waterTime : "); textOut(ourParamObj.getWaterTime()); textOutln(" ms");
  textOut("soakTime  : "); textOut(ourParamObj.getSoakTime()); textOutln(" ms");
  textOut("percent   : "); textOut(ourParamObj.getPWMPercent()); textOutln(" %");
  textOut("period    : "); textOut(ourParamObj.getPWMPeriod()); textOutln(" ms");
  textOut("Dry       : "); textOut(ourParamObj.getDry()); textOutln(" mf");
  textOut("Mud       : "); textOut(ourParamObj.getMud()); textOutln(" mf");
  textOut("Run #     : "); textOut(ourParamObj.getRunNum()); textOutln();
  textOutln();
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
  mAutoRead = timeVal > 0;
  doPrintReadings();
}


void textComObj::doPrintReadings(void) {

  textOut("Temperature : "); textOut(tempC); textOutln(" *C");
  textOut("Capacitive  : "); textOut(capread); textOutln(" mf");
  textOut("Moisture    : "); textOut((int)moisture); textOutln(" %");
  textOut("And we are  : ");
  switch (weAre) {
    case sitting  : textOutln("sitting");  break;
    case watering : textOutln("watering"); break;
    case soaking  : textOutln("soaking");  break;
  }
  if (mAutoRead) readTimer.start();
}


void textComObj::doPrintGReadings(void) {

  //textOut(tempC);textOut(" ");
  //textOut(capread);textOut(" ");
  textOut((int)moisture);
  textOutln();
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
  mAutoRead = timeVal > 0;
}


void textComObj::setDryLimit(void) {

  int   newVal;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    ourParamObj.setDryLimit(newVal);
    textOut("Moisture set point now set to ");
    textOutln(ourParamObj.getDryLimit());
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
    textOut("Watering tmie now set to ");
    textOutln(ourParamObj.getWaterTime());
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
    textOut("Soak time now set to ");
    textOutln(ourParamObj.getSoakTime());
  }
}


void textComObj::setPlantName(void) {

  char* charBuff;

  if (mParser.numParams()) {
    charBuff = mParser.getParamBuff();
    ourParamObj.setName(charBuff);
    free(charBuff);
    charBuff = ourParamObj.getName();
    textOut("Plant name is now set to ");
    textOutln(charBuff);
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
    textOut("Percent now set to ");
    textOutln(ourParamObj.getPWMPercent());
  }
}


void textComObj::setPWMPeriod(void) {

  int   newVal;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    textOut("Param buff :"); textOutln(paramBuff);
    newVal = atoi (paramBuff);
    textOut("newVal :"); textOutln(newVal);
    free(paramBuff);
    ourParamObj.setPWMPeriod(newVal);
    textOut("Period now set to ");
    textOutln(ourParamObj.getPWMPeriod());
  }
}


void textComObj::turnPump(void) {

  int numChars;
  char* paramBuff;

  pumpCom = false;
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    numChars = strlen(paramBuff);
    for (int i = 0; i < numChars; i++) {
      paramBuff[i] = toupper(paramBuff[i]);
    }
    if (!strcmp(paramBuff, "ON")) {
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
  textOut("Dry limit now set to ");
  textOutln(ourParamObj.getDry());
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
  textOut("Mud limit now set to ");
  textOutln(ourParamObj.getMud());
}


void textComObj::logCommand(void) {

  int   numChars;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    numChars = strlen(paramBuff);
    for (int i = 0; i < numChars; i++) {
      paramBuff[i] = toupper(paramBuff[i]);
    }
    if (!strcmp(paramBuff, "RESET")) {
      ourDisplay.deleteLog();
    }
    else if (!strcmp(paramBuff, "ON")) {
      ourDisplay.setLogging(true);
    }
    else if (!strcmp(paramBuff, "OFF")) {
      ourDisplay.setLogging(false);
    }
    else if (!strcmp(paramBuff, "SHOW")) {
      ourDisplay.showLogfile();
    }
    else if (!strcmp(paramBuff, "LINES")) {
      ourDisplay.showLogLines();
    }
    else {
      textOutln("You can say on, off, show, lines or reset. That's all I understand.");
    }
    free(paramBuff);
  } else {                                    // No params? Power user!
    ourDisplay.setLogging(!ourDisplay.isLogging());
  }
}

void textComObj::listDirectory(void) {

  File  wd;
  File  entry;
  bool  done;

  wd = SD.open("/"); // workingDir
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        textOut(entry.name());
        if (entry.isDirectory()) {
          textOutln("/");
        } else {
          textOut("\t"); textOutln(entry.size(), DEC);
        }
        entry.close();
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  } else {
    textOutln("Fail to open file."); // Sadly, instead of returning a NULL, it just crashes.
  }
}


void textComObj::idle(void) {

  if (readTimer.ding() && mAutoRead) {
    doPrintReadings();
    //doPrintGReadings();
  }
}
