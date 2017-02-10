


#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#include "soundBoard.h"

soundBoard theSoundBoard(SOUNDBOARD_SHIELD);

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Small bugs fly fastest!");
 
  if (!theSoundBoard.begin()) { // initialise the music player
     Serial.print("theSoundBoard.begin() failed with error# ");Serial.println((int)theSoundBoard.getLastError());
     while (1);
  }
  Serial.println("theSoundBoard.begin() succeded!");
 
  if (!SD.begin(SOUNDBOARD_SD_CS)) {
    Serial.println("SD.begin() failed..");
    while (1);
  }
  Serial.println("SD.begin() succeded!");
  
  if (!theSoundBoard.setSoundfile("Hawaii50.mp3")) {
    Serial.print("setSoundfile() failed with error# ");Serial.println((int)theSoundBoard.getLastError());
    while (1);
  }
  Serial.println("setSoundfile() succeded!");

  if (!theSoundBoard.command(play)) {
    Serial.print("command(play) failed with error# ");Serial.println((int)theSoundBoard.getLastError());
    while (1);
  }
  Serial.println("command(play) succeded!");
}
  
   

void loop() {

  idle();
}

