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

char*  currentPN = &myPN[0];

char  inStr[255] = {""};

enum commands { noCommand, hello, getBars, setTycho, setAlex, setDan, setShelby, setJulie, setMe, sendText, reset };
lilParser mParser;



void setup() {

  pinMode(FONA_RST, OUTPUT);      // Perform reswet on the phone thing.
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  digitalWrite(FONA_RST, LOW);
  delay(10);
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println(F("Booting up FONA"));
  fonaSS.begin(4800);
  if (!fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  while (Serial.available()) {
    char inChar = Serial.read();
  }
  setupParser();
}


void setupParser(void) {

  mParser.addCmd(hello, "hello");
  mParser.addCmd(getBars, "signal");
  //mParser.addCmd(getBars, "bars");
  mParser.addCmd(setTycho, "tycho");
  mParser.addCmd(setAlex, "alex");
  mParser.addCmd(setDan, "dan");
  mParser.addCmd(setShelby, "shelby");
  mParser.addCmd(setJulie, "julie");
  mParser.addCmd(setMe, "Me");
  mParser.addCmd(sendText, "S");
  mParser.addCmd(reset,"reset");
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

  // send an SMS!
  //char sendto[21], message[141];
  
  //flushSerial();
  Serial.print(F("Send to #"));
  Serial.println(currentPN);
  if (!fona.sendSMS(currentPN, inStr)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("Sent!"));
  }
}


void loop() {

  char  inChar;
  int   command;
  int   index;
  
  if (Serial.available()) {
    inChar = Serial.read();
    //Serial.print(inChar);
    
    if (isspace(inChar)) {
      index = 0;
      while(inChar!=EOL) {
        inStr[index++]=inChar;
        if (Serial.available()) {
          inChar = Serial.read();
        }
      }
      inStr[index]  = '\0';
      //Serial.print(inStr);
    }
    
    command = mParser.addChar(inChar);
    switch (command) {                    
      case noCommand : break;
      case hello : Serial.println("Well, hello to you!"); break;
      case getBars :  getSignal(); break;
      case setTycho : currentPN = &tychoPN[0]; Serial.print("TEXTING : Tycho"); break;
      case setAlex : currentPN = &alexPN[0]; Serial.print("TEXTING : Alliebob"); break;
      case setDan : currentPN = &danPN[0]; Serial.print("TEXTING : Danny D"); break;
      case setShelby : currentPN = &shelbyPN[0]; Serial.print("TEXTING : Shelbers"); break;
      case setJulie : currentPN = &juliePN[0]; Serial.print("TEXTING : Vern"); break;
      case setMe : currentPN = &myPN[0]; Serial.print("TEXTING : ME!"); break;
      case sendText : sendSMS(); break;
      case -1 : Serial.println("Oh ohh.. parser error!");
    }
  }
}
