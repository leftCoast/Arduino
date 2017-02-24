
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include <soundCard.h>

soundCard theSoundCard(soundCard_SHIELD);

byte vol = 20; 
void setup() {
  Serial.begin(9600); //while(!Serial);
  Serial.println(F("Small bugs fly fastest!"));
 
  if (!theSoundCard.begin()) { // initialise the music player
     Serial.print(F("theSoundCard.begin() failed with error# "));Serial.println((int)theSoundCard.getLastError());
     while (1);
  }
  Serial.println(F("theSoundCard.begin() succeded!"));
 
  if (!SD.begin(soundCard_SD_CS)) {
    Serial.println(F("SD.begin() failed.."));
    while (1);
  }
  Serial.println(F("SD.begin() succeded!"));

  //if (!theSoundCard.setSoundfile("Hawaii50.mp3")) { //"Hawaii50.mp3"
  if (!theSoundCard.setSoundfile("RadioAct.mp3")) { //"Hawaii50.mp3"
    Serial.print(F("setSoundfile() failed with error# "));Serial.println((int)theSoundCard.getLastError());
    while (1);
  }
  Serial.println(F("setSoundfile() succeded!"));
  
  if (!theSoundCard.command(play)) {
    Serial.print(F("command(play) failed with error# "));Serial.println((int)theSoundCard.getLastError());
    while (1);
  }
  theSoundCard.setVolume(vol);
  Serial.println(F("command(play) succeded!"));
}
  

void loop() {

  char  inChar;
  
  idle();
  if (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar=='u') {
      if (vol>0) vol--;
      theSoundCard.setVolume(vol);
    } else if (inChar=='d') {
      if (vol<255) vol++;
      theSoundCard.setVolume(vol);
    } else if (inChar=='p') {
      theSoundCard.command(pause);
    }  else if (inChar=='r') {
      theSoundCard.command(restart);
    }
  }
  
}

