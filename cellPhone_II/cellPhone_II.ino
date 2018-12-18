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
  
  Serial1.begin(9600);
  Serial.begin(115200);
  
  Serial.println(F("Booting up FONA.."));
  fonaSS.begin(4800);
  if (!fona.begin(fonaSS)) {
    Serial1.println(F("Couldn't find FONA")); 
    while (1);
  }
  Serial1.println(F("FONA online."));
  fona.setAudio(FONA_EXTAUDIO);
  setupParser();
  while (Serial1.available()) {
    char inChar = Serial1.read();
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

  Serial1.print(F("RSSI = ")); Serial1.print(n); Serial1.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  Serial1.print(r); Serial1.println(F(" dBm"));
}


void sendSMS(void) {
  
  Serial1.print(F("Me"));
  Serial1.print(F(" - "));
  Serial1.print(inStr);
  if (!fona.sendSMS(currentPN, inStr)) {
    Serial1.println(F(" - Failed"));
  } else {
    Serial1.println(" ->");
  }
}

void getCount(void) {

  int numMsgs = fona.getNumSMS();
  if (numMsgs>=0) {
    Serial1.print("You have ");
    Serial1.print(numMsgs);
    Serial1.println(" pending");
  } else {
    Serial1.println("Don't know, it gave back an error.");
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
    Serial1.print(currentName);Serial1.print(" - ");
    Serial1.println(inStr);
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
          Serial1.print(currentName);Serial1.print(" - ");
          Serial1.println(inStr);
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
  Serial1.print("TEXTING : ");
  Serial1.println(currentName);  
}


void loop() {

  char  inChar;
  int   command;
  int   index;
  
  if (Serial1.available()) {
    inChar = Serial1.read();
    
    Serial.print(inChar);
    
    if (isspace(inChar)) {
      index = 0;
      while(inChar!=EOL) {
        inStr[index++]=inChar;
        if (Serial1.available()) {
          inChar = Serial1.read();
        }
      }
      inStr[index]  = '\0';
    }
    
    command = mParser.addChar(inChar);
    switch (command) {                    
      case noCommand : break;
      case hello : Serial1.println("Well, hello to you!"); break;
      case getBars :  getSignal(); break;
      case setTycho   : 
      case setAlex    : 
      case setDan     : 
      case setShelby  : 
      case setJulie   : 
      case setMe      : changeContact(command); break;
      case sendMsg : sendSMS(); break;
      case getMsgCount  : getCount(); break;
      case -1 : Serial1.println("Oh, Sorry. What was that?");
    }
  }
  if (backgroundTimer.ding()) {
    backgroundTimer.start();
    displayMsgs();
  }
}
