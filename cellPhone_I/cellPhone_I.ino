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

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

#define BUFF_BYTES        255
#define REPLY_TIMEOUT_MS  1000  // Give it a second.


class seen {

  public:
                seen(char* inPattern);
  virtual       ~seen(void);
  
  virtual bool  addChar(char inChar);
  virtual bool  result(void);
  virtual void  reset(void);

  char* pattern;
  int   index;
};


seen::seen(char* inPattern) {

  pattern = (char*)malloc(strlen(inPattern)+1);
  if (pattern) {
    strcpy(pattern,inPattern);
  }
  index=0;
}


seen::~seen(void) { if (pattern) free(pattern); }


bool seen::addChar(char inChar) {

  if (pattern[index]=='\0') return true;    // We've already seen it, bail.
  if (inChar==pattern[index]) {
    index++;
    if (pattern[index]=='\0') return true;  // We just saw it! Bail.
  } else {
    index=0;
  }
  return false;                             // We've never seen it.
}


bool seen::result(void) { return pattern[index]=='\0'; }


void seen::reset(void) { index=0;}



seen  seenOK("OK");
seen  seenError("ERROR");

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
//SoftwareSerial *fonaSerial = &fonaSS;


char    charBuff[BUFF_BYTES];
int     index;
uint8_t type;
timeObj replyTimeout;


void setup() {

  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("Boot up FONA"));
  fonaSS.begin(4800);
  if (!fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  replyTimeout.setTime(REPLY_TIMEOUT_MS);
  index = 0;
}

void flushInput(void) {
  
  char  inChar;
  
  while(fonaSS.available()) {
    inChar = fonaSS.read();
  }
}


void cleanInStr(char* buff) {

  int   readIndex;
  int   writeIndex;
  
  readIndex = 2;                  // there is always /r/n to start.
  writeIndex = 0;
  while(buff[readIndex]!='\0') {
    if (buff[readIndex]=='\r') {
      while(buff[readIndex]=='\r'||buff[readIndex]=='\n') {
        readIndex++;
      }
      buff[writeIndex++] = ' ';
    }
    buff[writeIndex++] = buff[readIndex++];
  }
  buff[writeIndex] = '\0';
}


bool sendCom(char* inCom) {

  char  inChar;
  bool  done;

  seenOK.reset();
  seenError.reset();
  fonaSS.print(inCom);                        // Send it to the hardware..
  replyTimeout.start();                       // Start our timeout timer.
  index = 0;
  done = false;
  while(!replyTimeout.ding()&&!done) {
    if (fonaSS.available()) {
      inChar = fonaSS.read();
      done = seenOK.addChar(inChar) || seenError.addChar(inChar);
      charBuff[index++]=inChar;
    }
  }
  charBuff[index]='\0';
  cleanInStr(charBuff);
  Serial.println(charBuff);
  return(seenOK.result());
}


int numSMSMessages(void) {

  if (sendCom("AT+CMGF=1")) {    // Set text mode
    if (sendCom("AT+CPMS?")) {   // Grab the amount data.
      
    }
  }
  
}
void loop() {
  
  char  inChar;
  
  if (Serial.available()) {
    inChar = Serial.read();
    if (inChar!='\n') {
      charBuff[index++]=inChar;
    } else {
      charBuff[index++]='\r';
      charBuff[index++]='\n';
      charBuff[index]='\0';
      Serial.print(charBuff);     // Show we got it.
      flushInput();               // Dump leftovers.
      if (sendCom(charBuff)) {
        Serial.println("Looks good");
      } else {
        Serial.println("Oh ohh, error");
      }
      index = 0; 
    }
  }
}
