#include "soundBoard.h"

// DREQ should be an an Interrupt pin. *** UN-USED IN THIS VERSION ***
#define DREQ 3 


soundBoard::soundBoard(byte boardSetup) 
  : timeObj(SOUNDBOARD_SLEEP_MS) { 
  
    setupType = boardSetup;
    filePath = NULL;
    setError(noErr);
  }


soundBoard::~soundBoard(void) { 
  
    if (musicPlayer) {
      delete musicPlayer;
      musicPlayer = NULL;
    }
    if (filePath) {
      free(filePath);
      filePath = NULL;
    }
  }


boolean soundBoard::begin(void) { 

    start();                    // Whatever happens, lets get the timer rolling.
    switch (setupType) {
      case SOUNDBOARD_SHIELD :
        musicPlayer = new Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, SOUNDBOARD_SD_CS);
      break;
      case SOUNDBOARD_BREAKOUT :
        musicPlayer = new Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, SOUNDBOARD_SD_CS);
      break;
      default : setError(badSetup); return false;
    }
    if (musicPlayer) {
      if (musicPlayer->begin()) {
        hookup();
        return true;
      } else {
        setError(initErr);
      }
    } else {
      setError(mallocErr);
    }
    return false;
  }


boolean soundBoard::setSoundfile(char* inFilePath) {

  if (filePath) {
    free(filePath);
    filePath = NULL;
  }
  if (inFilePath) {
    filePath = malloc(strlen(inFilePath)+1);
    if (filePath) {                               // Good or bad, we've allocated the file path.
      strcpy(filePath,inFilePath);
      File testFile = SD.open(filePath);
      if(testFile) {
        testFile.close();
        return true;
      } else {
        setError(noFileErr);
      }
      free(filePath);                             // If we get here, allocated path was bad.
      filePath = NULL;                            // So we delete it an flag with NULL.
    } else {
      setError(mallocErr);
    }
  } else {
    setError(nullStrErr);
  }
  return false; 
}


boolean soundBoard::command(action inCommand) {

  boolean success;

  setError(noErr);
  success = false;
  switch(inCommand) {
    case play :                                             // User hit play.
      if (musicPlayer->paused()) {                          // And we're paused?
        musicPlayer->pausePlaying(false);                   // Just un-pause.
        success = true;                                     // That's a success.
      } else if (filePath) {                                // If we ain't paused, but have a file.
        success = musicPlayer->startPlayingFile(filePath);  // See if we can start playing this file.
        if (!success) setError(unknownErr);                 // Who knows what broke in there?
      } else {                                              // And if we had no file?
        setError(noFileErr);                                // Flag it.
      }
      break;
    case pause :                              // User hits pause.
      if (musicPlayer->paused()) {            // We're already paused.
        musicPlayer->pausePlaying(false);     // Then just start up again.
      } else if (musicPlayer->playingMusic) { // If we're playing?
        musicPlayer->pausePlaying(true);      // Well, then pause!
      } 
      success = true;                         // Good enough really, call it good.
      break;
    case restart :                                          // User wants a restart.
      if (filePath) {                                       // If we have a file.
        success = musicPlayer->startPlayingFile(filePath);  // See if we can start playing this file.
      } else {                                              // And if we had no file?
        setError(noFileErr);                                // Flag it.
      }
      break;
    default : setError(badCommand);   // We don't know WHAT the user wants!
  }
  return success;
}

          
boolean soundBoard::isPlaying(void) { return musicPlayer->playingMusic; }


void soundBoard::setError(soundBoardErr inErr) { lastErr = inErr; }


soundBoardErr soundBoard::getLastError(void) { return lastErr; }


void soundBoard::idle(void) {

    if (musicPlayer->playingMusic && ding()) {
      noInterrupts();
      musicPlayer->feedBuffer();
      interrupts();
      start();
    }
  }



