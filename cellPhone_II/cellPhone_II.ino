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
//#include <lilParser.h>
#include "quickCom.h"

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#include "SMSMsgList.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7
//#define S_PORT  Serial1

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
byte  inBuff[255];

                
qCSlave   ourComObj;


void setup() {

  pinMode(FONA_RST, OUTPUT);      // Perform reset on the phone thing.
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  digitalWrite(FONA_RST, LOW);
  delay(10);
  digitalWrite(FONA_RST, HIGH);
  delay(100);

  //Serial.begin(9600);
  //while(!Serial);
  Serial.print("Hello!");
  ourComObj.begin(9600);
  //Serial.begin(115200);
  
  //S_PORT.println(F("Booting up FONA.."));
  fonaSS.begin(4800);
  if (!fona.begin(fonaSS)) {
    //S_PORT.println(F("Couldn't find FONA")); 
    while (1);
  }
  //S_PORT.println(F("FONA online."));
  fona.setAudio(FONA_EXTAUDIO); 
}


void getSignal(void) {
/*
  uint8_t n = fona.getRSSI();
  int8_t  r;

  S_PORT.print(F("RSSI = ")); S_PORT.print(n); S_PORT.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  S_PORT.print(r); S_PORT.println(F(" dBm"));
  */
}


void sendSMS(void) {
  /*
  S_PORT.print(F("Me"));
  S_PORT.print(F(" - "));
  S_PORT.print(inStr);
  if (!fona.sendSMS(currentPN, inStr)) {
    S_PORT.println(F(" - Failed"));
  } else {
    S_PORT.println(" ->");
  }
  */
}


void getCount(void) {
/*
  int numMsgs = fona.getNumSMS();
  if (numMsgs>=0) {
    S_PORT.print("You have ");
    S_PORT.print(numMsgs);
    S_PORT.println(" pending");
  } else {
    S_PORT.println("Don't know, it gave back an error.");
  }*/
}


void getBatteryPercent(void) {

  uint16_t batPct;
/*
  if (fona.getBattPercent(&batPct)) {
    S_PORT.print(F("Battery = "));
    S_PORT.print(batPct);
    S_PORT.println(F("%"));
  } else {
    S_PORT.println("Don't know, the dumb thing gave back an error.");
  }
  */
}


byte getRawBatteryPercent(void) {

  uint16_t   batPct;
  byte  outVal;
  
  if (fona.getBattPercent(&batPct)) {
    outVal = batPct;
    return outVal;
  }
  return 0;
}


void displayMsgs(void) {

  int   numMsgs;
  int   msgRead;
  int   slot;
  char  phoneNum[13];
  int   chars;

  /*
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
          S_PORT.print(currentName);S_PORT.print(" - ");
          S_PORT.println(inStr);
          fona.deleteSMS(slot);
        }
      }
      slot++;
    }
  }
  */
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
  //S_PORT.print("TEXTING : ");
  //S_PORT.println(currentName);  
}


void loop() {

  byte  numBytes;
  
  idle();
  numBytes = ourComObj.haveBuff();
  //checkErr("haveBuff");
  if (numBytes) {
    //Serial.print("Have a message! ");
    if (ourComObj.readBuff(inBuff)) {
      //Serial.print("com # = ");Serial.println((byte)inBuff[0]);
      switch (inBuff[0]) {                    
        case noCommand    : break;
        case hello        : 
          //Serial.println("saying hello!");
          strcpy(inBuff,"Well, hello to you!");
          ourComObj.replyBuff(inBuff,strlen(inBuff)+1);
        break;
        case getBars      :  getSignal(); break;
        case getBatPct    : getBatteryPercent(); break;
        case rawBattVal   : 
          //Serial.println("Returning battery percent");
          inBuff[0] = getRawBatteryPercent();
          ourComObj.replyBuff(inBuff,1);
         break;
        case setTycho     : 
        case setAlex      : 
        case setDan       : 
        case setShelby    : 
        case setJulie     : 
        //case setMe        : changeContact(command); break;
        case sendMsg      : sendSMS(); break;
        case getMsgCount  : getCount(); break;
        default           : 
          strcpy(inBuff,"Oh, Sorry. What was that?");
          ourComObj.replyBuff(inBuff,strlen(inBuff)+1);
        break;
      }
    }
  }
}
