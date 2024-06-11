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
	setTime(.01,true);	// A bit of a hack. Currently timeObj has three states, preset, running and expired. This forces expired.
}


toneObj::~toneObj(void) {  }
				
				
void toneObj::play(int inHz,int inMs) {
	
	if (mPin>=0) {
		if (inHz>0) {
			tone(mPin,inHz,inMs);
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


note::note(int inHz,int inMs)
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
 
 

void tune::addNote(int inHz,int inMs) {

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


MIDItune::MIDItune(const char* inFilePath)
	: tune() {
	
	filePath = NULL;
	heapStr(&filePath,inFilePath);
}


MIDItune::~MIDItune(void) { MIDIFile.close(); }

void MIDItune::createTune(void) {

	BYTE_SWAP
	
   File        MIDIFile;
   MIDIHeader  theMIDIHeader;
   trackHeader theTrackHeader;
   eventHeader anEventHeader;
   metaEvent   aMetaEvent;
   byte        aByte;
   uint32_t		temp;
   uint32_t    microsPerBeat;
   float			MsPerTick;
   bool        done;
   int			DnNote;
   uint32_t		StTime;
   uint32_t		EndTime;
   
   DnNote	= 0;	
   StTime	= 0;
   EndTime	= 0;			
	MIDIFile = SD.open(filePath, FILE_READ);
	if (MIDIFile) {
		
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
         			if (DnNote) {
         				EndTime = StTime + anEventHeader.deltaTime;
         				addNote(MIDI2Freq(DnNote),anEventHeader.deltaTime);
         				DnNote = anEventHeader.param1;
         				StTime = EndTime;
         			} else {
         				EndTime = StTime + anEventHeader.deltaTime;
         				addNote(REST,anEventHeader.deltaTime);
         				DnNote = anEventHeader.param1;
         				StTime = EndTime;
         			}
         		break;	
         		case MIDI_KEY_UP :
         			if (DnNote) {
         				addNote(MIDI2Freq(DnNote),anEventHeader.deltaTime);
         				EndTime = StTime + anEventHeader.deltaTime;
         			}
         		break;
         	}		
         }
		}
		MIDIFile.close();
	}
}
           