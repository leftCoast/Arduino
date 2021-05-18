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
  ourPort   = &Serial;
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


// This is where the raw data comes in, is parsed and we take action. BUT, there is a hack.
// Normally we run Serial to comumicate from the development computer. We read in the text
// Char by char. I wanted to be able to use the handheld and it runs through Serial1. And
// its doing all sorts of background things we don't know about. So the hack is, when the
// handheld sends a text command, it is sent to this objects handleTextCom() method. This
// method flips to the serial1 port and calls this method repeatedly pushing in the text
// char by char. So depending on who is in charge the method of grabbing chars is different.
void textComObj::checkTextCom(char inChar) {

  int   command;

  if (ourPort == &Serial1 || Serial.available()) {  // Ok, little weird. If we are pointing at serial1 or normal serial has some data..
    if (ourPort == &Serial) {                       // We know there's something going on. If we are NOT looking at Serial1..
      inChar = Serial.read();                       // Read out a data charactor.
      ourPort->print(inChar);                       // If using development machine, echo the charactor.
    }
    command = mParser.addChar(inChar);
    switch (command) {
      case noCommand      : break;
      case resetAll       : initParams();    break;
      case showParams     : printParams();   break;
      case showReadings   : printReadings(); break;
      case showGReadings  : ourPort->println("Graphing's disabled."); /* printGReadings(); */ break;
      case setSetPoint    : setDryLimit();   break;
      case setWTime       : setWaterTime();  break;
      case setSTime       : setSoakTime();   break;
      case setName        : setPlantName();  break;
      case setPump        : turnPump();      break;
      case setPercent     : setPWMPercent(); break;
      case setPeriod      : setPWMPeriod();  break;
      case setDryMapLimit : setDry();        break;
      case setMudMapLimit : setMud();        break;
      case logCom         : logCommand();    break;
      case listDir        : listDirectory(); break;
      default             : ourPort->println("Sorry, have no idea what you want.");
    }
  }
}


void textComObj::handleTextCom(char* buff) {
  
  int i;
  
  Print*  savedPort;
  i = 0;                        // Starting at first char..
  savedPort = ourPort;          // Save of the serial port we're using now.
  ourPort = &Serial1;           // Switch to port Serial1.
  while (buff[i] != '\0') {     // Loop through the string.
    checkTextCom(buff[i]);      // Jamming all the chars into the parser.
    i++;
  }
  delay(50);                    // Need the delay so the handheld can get ready (Even at 9600 baud. Sad)
  checkTextCom('\n');           // Top it all off with a newline. (Gets it cooking.)
  ourPort = savedPort;          // Replace the port pointer. Job complete!
}


void textComObj::initParams(void) {

  ourParamObj.floraReset();
  ourDisplay.deleteLog();
  ourPort->println("Params set to defaults.");
}


void textComObj::printParams(void) {

  ourPort->print("Parameters for : "); ourPort->println(ourParamObj.getName());
  ourPort->print("dryLimit    : "); ourPort->print(ourParamObj.getDryLimit()); ourPort->println(" %");
  ourPort->print("waterTime   : "); ourPort->print(ourParamObj.getWaterTime()); ourPort->println(" ms");
  ourPort->print("soakTime    : "); ourPort->print(ourParamObj.getSoakTime()); ourPort->println(" ms");
  ourPort->print("PWM percent : "); ourPort->print(ourParamObj.getPWMPercent()); ourPort->println(" %");
  ourPort->print("PWM period  : "); ourPort->print(ourParamObj.getPWMPeriod()); ourPort->println(" ms");
  ourPort->print("Dry         : "); ourPort->print(ourParamObj.getDry()); ourPort->println(" mf");
  ourPort->print("Mud         : "); ourPort->print(ourParamObj.getMud()); ourPort->println(" mf");
  ourPort->print("Run #       : "); ourPort->print(ourParamObj.getRunNum()); ourPort->println();
  ourPort->println();
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

  //ourPort->print("Temperature : "); ourPort->print(tempC); ourPort->println(" *C");
  ourPort->print("Capacitive  : "); ourPort->print(capread); ourPort->println(" mf");
  ourPort->print("Moisture    : "); ourPort->print((int)moisture); ourPort->println(" %");
  ourPort->print("And we are  : ");
  switch (weAre) {
    case sitting  : ourPort->println("sitting");  break;
    case watering : ourPort->println("watering"); break;
    case soaking  : ourPort->println("soaking");  break;
  }
  ourPort->println();
  if (mAutoRead) readTimer.start();
}


void textComObj::doPrintGReadings(void) {

  //ourPort->print(tempC);ourPort->print(" ");
  //ourPort->print(capread);ourPort->print(" ");
  ourPort->print((int)moisture);
  ourPort->println();
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
    ourPort->print("Moisture set point now set to ");
    ourPort->println(ourParamObj.getDryLimit());
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
    ourPort->print("Watering tmie now set to ");
    ourPort->println(ourParamObj.getWaterTime());
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
    ourPort->print("Soak time now set to ");
    ourPort->println(ourParamObj.getSoakTime());
  }
}


void textComObj::setPlantName(void) {

  char* charBuff;

  if (mParser.numParams()) {
    charBuff = mParser.getParamBuff();
    ourParamObj.setName(charBuff);
    free(charBuff);
    charBuff = ourParamObj.getName();
    ourPort->print("Plant name is now set to ");
    ourPort->println(charBuff);
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
    ourPort->print("Percent now set to ");
    ourPort->println(ourParamObj.getPWMPercent());
  }
}


void textComObj::setPWMPeriod(void) {

  int   newVal;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    ourPort->print("Param buff :"); ourPort->println(paramBuff);
    newVal = atoi (paramBuff);
    ourPort->print("newVal :"); ourPort->println(newVal);
    free(paramBuff);
    ourParamObj.setPWMPeriod(newVal);
    ourPort->print("Period now set to ");
    ourPort->println(ourParamObj.getPWMPeriod());
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
  ourPort->print("Dry limit now set to ");
  ourPort->println(ourParamObj.getDry());
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
  ourPort->print("Mud limit now set to ");
  ourPort->println(ourParamObj.getMud());
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
      ourPort->println("Logfile deleted.");
    }
    else if (!strcmp(paramBuff, "ON")) {
      ourDisplay.setLogging(true);
    }
    else if (!strcmp(paramBuff, "OFF")) {
      ourDisplay.setLogging(false);
    }
    else if (!strcmp(paramBuff, "SHOW")) {
      ourDisplay.showLogfile(ourPort);
    }
    else if (!strcmp(paramBuff, "LINES")) {
      ourDisplay.showLogLines(ourPort);
    }
    else {
      ourPort->println("You can say on, off, show, lines or reset. That's all I understand.");
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
        ourPort->print(entry.name());
        if (entry.isDirectory()) {
          ourPort->println("/");
        } else {
          ourPort->print("\t"); ourPort->println(entry.size(), DEC);
        }
        entry.close();
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  } else {
    ourPort->println("Fail to open file."); // Sadly, instead of returning a NULL, it just crashes.
  }
}


void textComObj::idle(void) {

  if (readTimer.ding() && mAutoRead) {
    doPrintReadings();
    //doPrintGReadings();
  }
}
