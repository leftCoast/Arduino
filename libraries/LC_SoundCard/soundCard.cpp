#include "soundCard.h"

#include "screen.h" // debugging

soundCard::soundCard(byte boardSetup,byte inCsPin,byte inDrqPin,byte inResetPin) 
  : timeObj(soundCard_SLEEP_MS) { 
  
    setupType = boardSetup;
    csPin = inCsPin;
    drqPin = inDrqPin;
    resetPin = inResetPin;
    filePath = NULL;
    volume = 40;						// Hardcoded default from Adafruit.
    newSong = false;
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
        musicPlayer = new Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DC, SHIELD_DREQ);
      break;
      case soundCard_BREAKOUT :
        musicPlayer = new Adafruit_VS1053_FilePlayer(resetPin,csPin,LC_DC,drqPin,0);
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
        newSong = true;
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
    case play : 																								// User hit play.
    	if (newSong) {	                                         	// New song loaded?
    		musicPlayer->stopPlaying();															// Call stop to close data file!
    		success = musicPlayer->startPlayingFile(filePath);			// Try playing the new song.
      	if (success) {																					// It worked? Cool!
      		newSong = false;																			// Note it.
      	} else {																								// Didn't work?
      		setError(unknownErr);                 								// Who knows what broke in there?
      	}
      } else if (musicPlayer->paused()) {												// No fresh new song. Been paused.
      	musicPlayer->pausePlaying(false);                 			// Just un-pause.
      	success = true;                                   			// That's a success.
      } else if (filePath) {																		// Not paused, but there's a file.
      	success = musicPlayer->startPlayingFile(filePath);			// Try playing that.
      	if (!success) setError(unknownErr);                 		// No? Again, who knows what broke in there?
      } else {                                              		// And if we had no file?
        setError(noFileErr);                                		// Flag it.
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
    case fullStop :
       musicPlayer->stopPlaying();
       success = true;
    break;
    case restart :                                          // User wants a restart.
      if (filePath) {                                       // If we have a file.
      	musicPlayer->stopPlaying();													// Call stop to close data file!
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
		musicPlayer->feedBuffer();
		start();
	}
}


// A BLOCKING run-'till-you're-done function.
// The idea is that there are very short "beep" 
// & "click" sounds that would be nice for UI
// Development and it would be OK to block for
// a couple ms.
void soundCard::playClip(char* filePath) {

	File soundFile;
	
	if (!musicPlayer->playingMusic) {		// Ok, no one is using the hardware.
		musicPlayer->startPlayingFile(filePath);
		while(musicPlayer->playingMusic) {
			if (musicPlayer->readyForData()) {
				musicPlayer->feedBuffer();
			}
		}
  	}
}


