#include <toneObj.h>
#include <Arduino.h>
#include <MIDINotes.h>
#include <strTools.h>
#include <SDTools.h>


//#include "debug.h"


// ***************************************************************************************
// Your classic tone playing object. This can tell you when the tone is finished.
// ***************************************************************************************


toneObj::toneObj(int inPin) {

	mPin = inPin;
	setTime(.001,true);	// A bit of a hack. Currently timeObj has three states, preset, running and expired. This forces expired.
}


toneObj::~toneObj(void) {  }
				
				
void toneObj::play(int inHz,float inMs) {
	
	if (mPin>=0) {
		if (inHz>0) {
			tone(mPin,inHz,round(inMs));
		} else {
			noTone(mPin);
		}
		setTime(inMs,true);
	}
}


bool toneObj::isPlaying(void) { return !ding(); }
				


// ***************************************************************************************
// Play a tune. basically a list of notes and rests. Use a toneObj above to play them.
// This would be a note..
// ***************************************************************************************


note::note(int inHz,float inMs)
	: linkListObj() {
	
	mHz = inHz;
	mMs = inMs;
}


note::~note(void) { }

			
note* note::getNext(void) { return (note*) linkListObj::getNext(); }


void note::playSelf(toneObj* inTone,float timeMult) { inTone->play(mHz,round(mMs*timeMult)); }
				

		
// ***************************************************************************************
// And here is the note list, or tune. Single voice.
// ***************************************************************************************

	
tune::tune(void)
	: linkList() {
	
	mTimeMult 	= 1;
	mPlaying 	= NULL;
	mTone			= NULL;
}


tune::~tune(void) {  }
 
 

void tune::addNote(int inHz,float inMs) {

	hookup();
	note* newNote = new note(inHz,inMs);
	addToEnd(newNote);
}


void tune::adjustSpeed(float inTimeMult) { mTimeMult = inTimeMult; }

	
void tune::startTune(toneObj* inTone,int inIndex) {

	if (inTone && inIndex>=0) {
		mTone = inTone;
		mPlaying = (note*)getByIndex(inIndex);
		if (mPlaying) {
			if (mPlaying) mPlaying->playSelf(mTone,mTimeMult);
		}
	}
}


int tune::stopTune(void) {

	int index;
	
	if (mTone) mTone->play(0,0);
	index = findIndex(mPlaying);
	mPlaying = NULL;
	return index;
}


bool tune::playing(void) { return mPlaying!=NULL; }


void tune::idle(void) {

	if (playing() && mTone) {
		if (!mTone->isPlaying()) {
			mPlaying = mPlaying->getNext();
			if (mPlaying) {
				mPlaying->playSelf(mTone,mTimeMult);
			}
		}
	}
}

      	      
// ***************************************************************************************
// Basically the same as a tune but.. Build from a MIDI file. (Single voice)
// ***************************************************************************************


MIDItune::MIDItune(void)
	: tune() {
	
	absTimeMs	= 0;
	MsPerTick	= 1;
	holding		= false;
}


MIDItune::~MIDItune(void) { }


void show(char* msg,float absTime, float deltaTime) {

	Serial.print(msg);
	Serial.print("\tAbs time : ");
	Serial.print(absTime,1);
	Serial.print("\tDelta time : ");
	Serial.println(deltaTime,1);
}


void MIDItune::addMIDINote(bool Dn,int MIDIKey,float deltaTime) {
	
	if (Dn) {													// If we got a key down event..
		if (holding) {											// If we still have a note down..
			//show("Chopping",absTimeMs,deltaTime);
			addNote(MIDI2Freq(heldNote),deltaTime);	// Single voice, cut off holding note here.
		} else if (deltaTime) {								// If there is a delta time..
			//show("Rest    ",absTimeMs,deltaTime);
			addNote(REST,deltaTime);						// Then we add a rest to fill the time.
		} else {
			//show("starting?",absTimeMs,deltaTime);
		}															//
		heldNote = MIDIKey;									// Update the new holding MIDI note.
		holding = true;										// Make a note of that.
	} else {														// 
		if (holding) {											// If we have a note down..
			//show("Adding  ",absTimeMs,deltaTime);
			addNote(MIDI2Freq(heldNote),deltaTime);	// Add the held note.
			holding = false;
		} else {
			//show("Ignoring",absTimeMs,deltaTime);
		}															//
	}																//
	absTimeMs = absTimeMs+deltaTime;						// In EVERY case we move absTimeMs up.
}


void MIDItune::createTune(const char*	filePath) {

	BYTE_SWAP
	
   File        MIDIFile;
   MIDIHeader  theMIDIHeader;
   trackHeader theTrackHeader;
   eventHeader anEventHeader;
   metaEvent   aMetaEvent;
   byte        aByte;
   uint32_t		temp;
   uint32_t    microsPerBeat;
   float			deltaMs;
   bool        done;
		
	MIDIFile = SD.open(filePath, FILE_READ);
	if (MIDIFile) {
		dumpList();	
		absTimeMs	= 0;
		MsPerTick	= 1;
		holding		= false;										
		readMIDIHeader(&theMIDIHeader, MIDIFile);
		readTrackHeader(&theTrackHeader, MIDIFile);
		done = false;
		while(!done) {
			readEventHeader(&anEventHeader, MIDIFile);
			if (isMetaTag(&anEventHeader)) {
            readMetaEvent(&anEventHeader, &aMetaEvent, MIDIFile);
            switch(aMetaEvent.metaType) {
               case 0x2F : done = true; break;
               case 0x51 :
						microsPerBeat = 0;
                  MIDIFile.read(&aByte, 1);
                  microsPerBeat = aByte;
                  microsPerBeat = microsPerBeat << 16;
                  MIDIFile.read(&aByte, 1);
                  temp = 0;
                  temp = aByte;
                  temp = temp << 8;
                  microsPerBeat = microsPerBeat | temp;
                  MIDIFile.read(&aByte, 1);
                  temp = 0;
                  temp = aByte;
                  microsPerBeat = microsPerBeat | aByte;
                  MsPerTick = (microsPerBeat/1000.0)/theMIDIHeader.timeDiv;
               break;
               default :
                  done = !jumpMetaEvent(&aMetaEvent, MIDIFile);
               break;
            }
         } else {
         	switch(anEventHeader.eventType) {
         		case MIDI_KEY_DN :
						deltaMs = anEventHeader.deltaTime * MsPerTick;
         			addMIDINote(true,anEventHeader.param1,deltaMs);
         		break;	
         		case MIDI_KEY_UP :
         			deltaMs = anEventHeader.deltaTime * MsPerTick;
         			addMIDINote(false,anEventHeader.param1,deltaMs);
         		break;
         	}		
         }
		}
		MIDIFile.close();
	}
}
           