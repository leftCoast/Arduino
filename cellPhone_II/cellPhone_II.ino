#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <lilParser.h>

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#include "SMSMsgList.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

char  alexPN[] = {"13603335200"};
char  danPN[] = {"13609298457"};
char  shelbyPN[] = {"13603332799"};
char  juliePN[] = {"13607084218"};
char  tychoPN[] = {"13603593687"};
char  myPN[] = {"14083400352"};

char* currentPN = &myPN[0];
char  currentName[20] = {"ME!"};
char  inStr[255] = {""};

enum commands { noCommand, hello, getBars, setTycho, setAlex, setDan, setShelby, setJulie, setMe, sendMsg, getMsgCount };
lilParser mParser;

timeObj backgroundTimer(1250);

//SMSMsgList  messages;

void setup() {

  pinMode(FONA_RST, OUTPUT);      // Perform reset on the phone thing.
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  digitalWrite(FONA_RST, LOW);
  delay(10);
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println(F("Booting up FONA.."));
  fonaSS.begin(4800);
  if (!fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA online."));
  fona.setAudio(FONA_EXTAUDIO);
  setupParser();
  while (Serial.available()) {
    char inChar = Serial.read();
  }
  
}


void setupParser(void) {

  mParser.addCmd(hello, "hello");
  mParser.addCmd(getBars, "signal");
  mParser.addCmd(getBars, "bars");
  mParser.addCmd(setTycho, "tycho");
  mParser.addCmd(setAlex, "alex");
  mParser.addCmd(setDan, "dan");
  mParser.addCmd(setShelby, "shelby");
  mParser.addCmd(setJulie, "julie");
  mParser.addCmd(setMe, "me");
  mParser.addCmd(sendMsg, "x");
  mParser.addCmd(getMsgCount,"count");
}


void getSignal(void) {

  uint8_t n = fona.getRSSI();
  int8_t  r;

  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  Serial.print(r); Serial.println(F(" dBm"));
}


void sendSMS(void) {
  
  Serial.print(F("Me"));
  Serial.print(F(" - "));
  Serial.print(inStr);
  if (!fona.sendSMS(currentPN, inStr)) {
    Serial.println(F(" - Failed"));
  } else {
    Serial.println(" ->");
  }
}

void getCount(void) {

  int numMsgs = fona.getNumSMS();
  if (numMsgs>=0) {
    Serial.print("You have ");
    Serial.print(numMsgs);
    Serial.println(" pending");
  } else {
    Serial.println("Don't know, it gave back an error.");
  }
}

/*
void updateMsgList(void) {

  int   numMsgs;
  int   msgRead;
  int   slot;
  char  phoneNum[13];
  int   chars;
  
  inStr[0]='\0';
  numMsgs = fona.getNumSMS();
  if (numMsgs>0) {
    msgRead = 0;
    slot = 1;
    while (msgRead<numMsgs) {
      if (fona.getSMSSender(slot,phoneNum,13)) {
        fona.readSMS(slot,inStr,250,&chars);
        msgRead++;
        messages.addMsg(&phoneNum[1],inStr);
        fona.deleteSMS(slot);
      }
      slot++;
    }
  }
}


void displyMsg(void) {

  SMSMsg* aMessage;
  
  aMessage = messages.findFirst(currentPN);
  if (aMessage) {
    aMessage->getMsg(inStr);
    Serial.print(currentName);Serial.print(" - ");
    Serial.println(inStr);
    delete(aMessage);
  }
}
*/

void displayMsgs(void) {

  int   numMsgs;
  int   msgRead;
  int   slot;
  char  phoneNum[13];
  int   chars;
  
  inStr[0]='\0';
  numMsgs = fona.getNumSMS();
  if (numMsgs>0) {
    msgRead = 0;
    slot = 1;
    while (msgRead<numMsgs) {
      if (fona.getSMSSender(slot,phoneNum,13)) {
        msgRead++;
        if (!strcmp(phoneNum,currentPN)) {
          fona.readSMS(slot,inStr,250,&chars);
          Serial.print(currentName);Serial.print(" - ");
          Serial.println(inStr);
          fona.deleteSMS(slot);
        }
      }
      slot++;
    }
  }
}


void changeContact(int command) {

  switch(command) {
    case setTycho : 
      currentPN = &tychoPN[0]; 
      strcpy(currentName,"Tycho");
    break;
    case setAlex :
      currentPN = &alexPN[0];
      strcpy(currentName,"Alliebob");
    break;
    case setDan :
      currentPN = &danPN[0];
      strcpy(currentName,"Danny D");
    break;
    case setShelby :
      currentPN = &shelbyPN[0];
      strcpy(currentName,"Shelbers");
    break;
    case setJulie :
      currentPN = &juliePN[0];
      strcpy(currentName,"Vern");
    break;
    case setMe :
      currentPN = &myPN[0];
      strcpy(currentName,"ME!");
    break;
  }
  Serial.print("TEXTING : ");
  Serial.println(currentName);  
}


void loop() {

  char  inChar;
  int   command;
  int   index;
  
  if (Serial.available()) {
    inChar = Serial.read();
    
    if (isspace(inChar)) {
      index = 0;
      while(inChar!=EOL) {
        inStr[index++]=inChar;
        if (Serial.available()) {
          inChar = Serial.read();
        }
      }
      inStr[index]  = '\0';
    }
    
    command = mParser.addChar(inChar);
    switch (command) {                    
      case noCommand : break;
      case hello : Serial.println("Well, hello to you!"); break;
      case getBars :  getSignal(); break;
      case setTycho   : 
      case setAlex    : 
      case setDan     : 
      case setShelby  : 
      case setJulie   : 
      case setMe      : changeContact(command); break;
      case sendMsg : sendSMS(); break;
      case getMsgCount  : getCount(); break;
      case -1 : Serial.println("Oh, Sorry. What was that?");
    }
  }
  if (backgroundTimer.ding()) {
    backgroundTimer.start();
    displayMsgs();
  }
}
