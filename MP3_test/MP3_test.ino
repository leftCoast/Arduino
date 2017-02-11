


#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include "soundBoard.h"

soundBoard theSoundBoard(SOUNDBOARD_SHIELD);

void setup() {
  Serial.begin(9600); while(!Serial);
  Serial.println(F("Small bugs fly fastest!"));
 
  if (!theSoundBoard.begin()) { // initialise the music player
     Serial.print(F("theSoundBoard.begin() failed with error# "));Serial.println((int)theSoundBoard.getLastError());
     while (1);
  }
  Serial.println(F("theSoundBoard.begin() succeded!"));
 
  if (!SD.begin(SOUNDBOARD_SD_CS)) {
    Serial.println(F("SD.begin() failed.."));
    while (1);
  }
  Serial.println(F("SD.begin() succeded!"));

  if (!theSoundBoard.setSoundfile("Hawaii50.mp3")) { //"Hawaii50.mp3"
 // if (!theSoundBoard.setSoundfile("RadioAct.mp3")) { //"Hawaii50.mp3"
    Serial.print(F("setSoundfile() failed with error# "));Serial.println((int)theSoundBoard.getLastError());
    while (1);
  }
  Serial.println(F("setSoundfile() succeded!"));

  if (!theSoundBoard.command(play)) {
    Serial.print(F("command(play) failed with error# "));Serial.println((int)theSoundBoard.getLastError());
    while (1);
  }
  Serial.println(F("command(play) succeded!"));
}
  
byte vol = 20;  

void loop() {

  char  inChar;
  
  idle();
  if (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar=='u') {
      if (vol>0) vol--;
      theSoundBoard.setVolume(vol);
    } else if (inChar=='d') {
      if (vol<255) vol++;
      theSoundBoard.setVolume(vol);
    } else if (inChar=='p') {
      theSoundBoard.command(pause);
    }  else if (inChar=='r') {
      theSoundBoard.command(restart);
    }
  }
  
}

