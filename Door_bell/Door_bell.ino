#include "MemoryFree.h"
#include <SSD_13XX.h>
#include <Adafruit_VS1053.h>
#include <adafruit_1431_Obj.h>
//#include <Fonts/FreeSerif9pt7b.h>
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
#include "liveText.h"

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
#define BOOT_FILE  "/dbos/boot.mp3"

bmpPipe paper;

soundCard thePlayer(soundCard_BREAKOUT, SOUND_CS, SOUND_DRQ, SOUND_RST);


enum majorStates { idling, ringingDBell, playingSong, editing, buttonClear, adjustVol };
majorStates majorState;
majorStates savedState;

bool  fileLoaded = false;
bool  doneEditing = false;
bool  settingVol = false;

mechButton  doorBellButton(DB_BTN);
mechButton  panelButton(POT_BTN);

runningAvg  potSmoother(5);
mapper      potToVol(0,1024,0,60);
byte vol = 20;

#define ADJ_VOL   "Adjust volume."
#define CLK_SAVE  "Click to save."
#define CLK_MENU  "Click for menu."
#define CLK_MUTE  "Click to mute."

label adjustLbl(25,20,100,8,ADJ_VOL);
label adjustLbl2(25,50,100,8,CLK_SAVE);

liveText fadeMessage(0,30,128,9,40);

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

  fadeMessage.hold();
  fadeMessage.addColor(0,&white);
  fadeMessage.addAColor(3000,&white);
  fadeMessage.addAColor(4000,&black);
  fadeMessage.setJustify(TEXT_CENTER);
  viewList.addObj(&fadeMessage);
  
  Serial.println(F("Starting file system."));
  if (!SD.begin(SOUND_SDCS)) {
    Serial.println(F("File system FAIL."));
  } else {
    Serial.println(F("File system ONLINE."));
  }
  Serial.println("Starting disk cleanup.");
  SDCleaner();
  
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println(F("Background .bmp file OPENED."));
  } else {
    Serial.println(F("Background .bmp file FAIL."));
  }



  Serial.println(F("Starting Sound card."));
  if (thePlayer.begin()) {
    Serial.println(F("Sound card ONLINE."));
    Serial.println(F("Checking soundfile."));
    if (thePlayer.setSoundfile(BOOT_FILE)) {
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
  
  viewList.addObj(&fileList);
  showClkMute();
  majorState = playingSong;
}


boolean validFilename(char* fileName) {

    char* strPtr;
    byte  strLen;
  
    if(fileName[0]=='_') {                   // Starts with a '_'
      strPtr = strstr(fileName,".MP3");
      if (strPtr) {
        strLen = strlen(fileName);
        strLen = strLen-4;
        if(strPtr==&(fileName[strLen])) {  // Ends with a .mp3
          return false;                    // One of apple's files, kill it.
        }
      }
    }
    if(fileName[0]=='~') {                // Starts with a '~'
        return false;                     // Seen these too. kill them.
    }
    strPtr = strstr(fileName,".TRA");     // Has a .tra in it..
    if (strPtr) {
      return false;                       // Kill those too.
    }
    return true;                          // Found nothing, let it pass.
  }


void SDCleaner(void) {

// FSEVEN~1/
// TRASHE~1/
// SPOTLI~1/

  File  wd;
  File  entry;
  char  fileName[15];
  bool  done;
   
  wd = SD.open("/");
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        strcpy(fileName,entry.name());
        entry.close();                      // Enough of you.
        if (!validFilename(fileName)) {
          SD.remove(fileName);              // One of apple's files, kill it.
        }
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  }
}

void clearMessage(void) { screen->fillRect(fadeMessage.x,fadeMessage.y,fadeMessage.width,fadeMessage.height,&black); }

void showClkMenu() {

  fadeMessage.setValue(CLK_MENU); // Set the text.
  clearMessage();
  fadeMessage.release();          // Let 'er rip with a reset.
}


void showClkMute() {

  fadeMessage.release();          // Allow drawing & reset color.
  fadeMessage.setValue(CLK_MUTE); // Set the text.
  clearMessage();
  fadeMessage.draw();             // Force a draw out of it.
  fadeMessage.hold();             // Lock out any redraw.
}


void loop() {

  idle();

  switch (majorState) {
    case idling       : checkIdle();      break;
    case ringingDBell : checkDBell();     break;
    case playingSong  : checkSongPlay();  break;
    case editing      : runEditor();      break;
    case buttonClear  : checkButtons();   break;
    case adjustVol    : checkSetVol();    break;
  }
}


void runVolume(void) {
  
  int potNum = analogRead(POT_ANLG);
  vol = potToVol.Map(potNum);
  thePlayer.setVolume(vol);
}


void checkSetVol() {

  runVolume();
  if (panelButton.clicked()) {
    currentVol = vol;
    writeParamFile();
    thePlayer.command(pause);
    screen->fillScreen(&black);
    showClkMute();
    thePlayer.command(play);
    waitForButtons(ringingDBell);
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
    potVal(potSmoother.addData(analogRead(POT_ANLG)));
  }
}


void endEdit(void) {

    fileList.dumpList();
    if (settingVol) {             // Oh ohh.. special case..
      drawBackground();
      adjustLbl.setColors(&pencil);
      adjustLbl2.setColors(&pencil);
      adjustLbl.draw();
      adjustLbl2.draw();
      thePlayer.command(play);
      fileLoaded = false;
      settingVol = false;
      majorState = adjustVol;
    } else {
      screen->fillScreen(&black);
      if (fileLoaded) {
        showClkMute(); 
        thePlayer.command(play);
        fileLoaded = false;
        majorState = playingSong;
      } else {
        showClkMenu();
        majorState = idling;
      }
    }
  }


void checkIdle(void) {

  if (doorBellButton.clicked()) {
    ringDBell();
  } else if (panelButton.clicked())  {
    fadeMessage.hold();
    startEditor();
    waitForButtons(editing);
  }
}


void ringDBell(void) {

  showClkMute();
  readParamFile();
  thePlayer.setSoundfile(currentSong);
  thePlayer.command(play);
  thePlayer.setVolume(currentVol);
  waitForButtons(ringingDBell);
}


// much like checkSongPlay() but no volume control.
void checkDBell(void) {

  if (panelButton.clicked()) {
    thePlayer.command(fullStop);
    showClkMenu();
    waitForButtons(idling);
  } else if (doorBellButton.clicked()) {
    ringDBell();
  } else if (!thePlayer.isPlaying()) {
    thePlayer.command(fullStop);
    showClkMenu();
    majorState = idling;
  }
}


void checkSongPlay(void) {

  runVolume();
  if (panelButton.clicked()) {
    thePlayer.command(fullStop);
    showClkMenu();
    waitForButtons(idling);
  } else if (doorBellButton.clicked()) {
    ringDBell();
  } else if (!thePlayer.isPlaying()) {
    thePlayer.command(fullStop);
    showClkMenu();
    majorState = idling;
  }
}




