
#include <SSD_13XX.h>
#include <Adafruit_VS1053.h>
#include <adafruit_1431_Obj.h>
#include <LC_SPI.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <bmpPipe.h>
#include <bmpLabel.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include <soundCard.h>

#include "mechButton.h"
#include "DB_Files.h"
#include "fList.h"

#define SOUND_CS    20
#define SOUND_SDCS  21
#define SOUND_DRQ   1
#define SOUND_RST   15

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   -1    // Not wired

#define POT_BTN     4
#define POT_ANLG    A13

#define DB_BTN      5

#define DISP_BG_FILE  "/dbos/paper.bmp"

bmpPipe paper;
bmpLabel label1(20, 6, 100, 10, "File name 1", &paper);

soundCard thePlayer(soundCard_BREAKOUT, SOUND_CS, SOUND_DRQ, SOUND_RST);
byte vol = 20;

enum majorStates { idling, ringingDBell, playingSong, editing, buttonClear };
majorStates majorState;
majorStates savedState;

bool  fileLoaded;
bool  doneEditing;

mechButton  doorBellButton(DB_BTN);
mechButton  panelButton(POT_BTN);

extern  drawList  fileList;

void setup() {

  //Serial.begin(9600); while (!Serial);
  Serial.println(F("Serial online."));

  Serial.println(F("Starting screen."));
  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    Serial.println(F("Init screen card FAIL."));
  } else {
    screen->fillScreen(&black);
    Serial.println(F("Screen\'s ONLINE."));
  }


  Serial.println(F("Starting file system."));
  if (!SD.begin(SOUND_SDCS)) {
    Serial.println(F("File system FAIL."));
  } else {
    Serial.println(F("File system ONLINE."));
  }

  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println(F("Background .bmp file OPENED."));
  } else {
    Serial.println(F("Background .bmp file FAIL."));
  }

  viewList.addObj(&fileList);

  Serial.println(F("Starting Sound card."));
  if (thePlayer.begin()) {
    Serial.println(F("Sound card ONLINE."));
    Serial.println(F("Checking soundfile."));
    if (thePlayer.setSoundfile("Corvette.mp3")) {
      Serial.println(F("Soundfile check OK."));
      Serial.println(F("Checking playback."));
      if (thePlayer.command(play)) {
        thePlayer.setVolume(vol);
        Serial.println(F("Playback check OK."));
      } else {
        Serial.println(F("Playback check FAIL"));
        Serial.print(F("command(play) failed with error# ")); Serial.println((int)thePlayer.getLastError());
      }
    } else {
      Serial.println(F("Soundfile check FAIL."));
    }
  } else {
    Serial.println(F("Sound card FAIL."));
  }

  majorState = playingSong;
}


void loop() {

  idle();

  switch (majorState) {
    case idling       : checkIdle();      break;
    case ringingDBell : checkSongPlay();  break;
    case playingSong  : checkSongPlay();  break;
    case editing      : runEditor();      break;
    case buttonClear  : checkButtons();   break;
  }
}


void waitForButtons(majorStates nextState) {

  savedState = nextState;
  majorState = buttonClear;
}


void checkButtons(void) {

  if (!doorBellButton.clicked() && !panelButton.clicked()) {
    majorState = savedState;
  }
}


void runEditor(void) {

  if (doorBellButton.clicked()) {
    endEdit();
    ringDBell();
  } else if (panelButton.clicked())  {
    buttonClick();
    waitForButtons(editing);
  }
  else if (doneEditing) {
    endEdit();
    doneEditing = false;
  } else {
    potVal(analogRead(POT_ANLG));
  }
}


void endEdit(void) {

  fileList.dumpList();
  screen->fillScreen(&black);
  if (fileLoaded) {
    thePlayer.command(play);
    fileLoaded = false;
    majorState = playingSong;
  } else {
    majorState = idling;
  }
}


void checkIdle(void) {

  if (doorBellButton.clicked()) {
    ringDBell();
  } else if (panelButton.clicked())  {
    startEditor();
    waitForButtons(editing);
  }
}


void ringDBell(void) {

  readParamFile();
  thePlayer.setSoundfile(currentSong);
  thePlayer.command(play);
  thePlayer.setVolume(currentVol);
  waitForButtons(ringingDBell);
}


void checkSongPlay() {

  if (panelButton.clicked()) {
    thePlayer.command(fullStop);
    waitForButtons(idling);
  } else if (doorBellButton.clicked()) {
    ringDBell();
  } else if (!thePlayer.isPlaying()) {
    thePlayer.command(fullStop);
    majorState = idling;
  }
}




