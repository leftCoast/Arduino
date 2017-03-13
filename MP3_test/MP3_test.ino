
#include <SPI.h>
#include <Adafruit_VS1053.h>

#include <soundCard.h>

#define SOUND_CS    20
#define SOUND_SDCS  21
#define SOUND_DRQ   1
#define SOUND_RST   15

//soundCard theSoundCard(soundCard_BREAKOUT);
soundCard theSoundCard(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ,SOUND_RST);
byte vol = 20;
void setup() {
  Serial.begin(9600); while (!Serial);
  Serial.println(F("Small bugs fly fastest!"));

  if (!theSoundCard.begin()) { // initialise the music player
    Serial.print(F("theSoundCard.begin() failed with error# ")); Serial.println((int)theSoundCard.getLastError());
    while (1);
  }
  Serial.println(F("theSoundCard.begin() succeded!"));

  if (!SD.begin(SOUND_SDCS)) {
    Serial.println(F("SD.begin() failed.."));
    while (1);
  }
  Serial.println(F("SD.begin() succeded!"));

  //if (!theSoundCard.setSoundfile("Hawaii50.mp3")) {
  //if (!theSoundCard.setSoundfile("RadioAct.mp3")) {
  //if (!theSoundCard.setSoundfile("SK8ERBOI.mp3")) {
  //if (!theSoundCard.setSoundfile("blueSky.mp3")) {
  //if (!theSoundCard.setSoundfile("phoneLn.mp3")) {
  //if (!theSoundCard.setSoundfile("WNEVRWHR.mp3")) {
  //if (!theSoundCard.setSoundfile("Angel.mp3")) {
  if (!theSoundCard.setSoundfile("Corvette.mp3")) {
    Serial.print(F("setSoundfile() failed with error# ")); Serial.println((int)theSoundCard.getLastError());
    while (1);
  }
  Serial.println(F("setSoundfile() succeded!"));

  if (!theSoundCard.command(play)) {
    Serial.print(F("command(play) failed with error# ")); Serial.println((int)theSoundCard.getLastError());
    while (1);
  }
  theSoundCard.setVolume(vol);
  Serial.println(F("command(play) succeded!"));
}


// Returns the position of END of the pattern, 0 if not found.
unsigned long findInFile(File inFile, byte * pattern, int numChars, unsigned long startPos, unsigned long endPos) {

  unsigned long i;
  int           buffIndex;

  buffIndex = 0;
  for (i = startPos; i <= endPos; i++) {
    if (inFile.seek(i)) {
      if (inFile.peek() == pattern[buffIndex]) {
        buffIndex++;
        if (buffIndex == numChars) {
          return i;
        }
      } else {
        buffIndex = 0;
      }
    } else {  // EOF, we assume..
      return 0;
    }
  }
  return 0;
}


void loop() {

  char  inChar;

  idle();
  if (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == 'u') {
      if (vol > 0) vol--;
      theSoundCard.setVolume(vol);
    } else if (inChar == 'd') {
      if (vol < 255) vol++;
      theSoundCard.setVolume(vol);
    } else if (inChar == 'p') {
      theSoundCard.command(pause);
    }  else if (inChar == 'r') {
      theSoundCard.command(restart);
    }
  }

}

