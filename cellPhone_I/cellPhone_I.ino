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

void cleanOutput(char* buff) {

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
      fonaSS.print(charBuff);     // Send it to the hardware..
      replyTimeout.start();       // Start our timeout timer.
      index = 0;
      bool seenOh = false;
      while(!replyTimeout.ding()) {
        if (fonaSS.available()) {
          inChar = fonaSS.read();
          if (inChar=='O') {
            seenOh = true;
          } else if (seenOh && inChar=='K') {
            charBuff[index++]=inChar;
            break;
          }
          charBuff[index++]=inChar;
        }
      }
      charBuff[index]='\0';
      cleanOutput(charBuff);
      Serial.println(charBuff);
      index = 0; 
    }
  }
}
