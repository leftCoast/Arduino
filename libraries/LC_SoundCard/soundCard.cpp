#include "soundCard.h"

// DREQ should be an an Interrupt pin. *** UN-USED IN THIS VERSION ***
#define DREQ 3 


soundCard::soundCard(byte boardSetup) 
  : timeObj(soundCard_SLEEP_MS) { 
  
    setupType = boardSetup;
    filePath = NULL;
    volume = 40;						// Hardcoded default from Adafruit.
    setError(noErr);
  }


soundCard::~soundCard(void) { 
  
    if (musicPlayer) {
      delete musicPlayer;
      musicPlayer = NULL;
    }
    if (filePath) {
      free(filePath);
      filePath = NULL;
    }
  }


boolean soundCard::begin(void) { 

    start();                    // Whatever happens, lets get the timer rolling.
    switch (setupType) {
      case soundCard_SHIELD :
        musicPlayer = new Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, soundCard_SD_CS);
      break;
      case soundCard_BREAKOUT :
        musicPlayer = new Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, soundCard_SD_CS);
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


boolean soundCard::setSoundfile(char* inFilePath) {

  if (filePath) {
    free(filePath);
    filePath = NULL;
  }
  if (inFilePath) {
    filePath = (char*)malloc(strlen(inFilePath)+1);
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


boolean soundCard::command(action inCommand) {

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

          
boolean soundCard::isPlaying(void) { return musicPlayer->playingMusic; }


void soundCard::setVolume(byte inVolume) { 

	volume = inVolume;
	musicPlayer->setVolume(inVolume,inVolume);
}


byte soundCard::getVolume(void) { return volume; }


void soundCard::setError(soundCardErr inErr) { lastErr = inErr; }


soundCardErr soundCard::getLastError(void) { return lastErr; }


void soundCard::idle(void) {

    if (musicPlayer->playingMusic && ding()) {
      noInterrupts();
      musicPlayer->feedBuffer();
      interrupts();
      start();
    }
  }



