#include <toneObj.h>
#include <MIDINotes.h>
#include <strTools.h>
#include <SDTools.h>


//#include "debug.h"


// ***************************************************************************************
// Your classic tone playing object. This can tell you when it's tone is finished.
// ***************************************************************************************


toneObj::toneObj(int inPin) {

	mPin = inPin;
	setTime(.001,true);	// A bit of a hack. - This forces a quick expired state.
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


/*
// Debugging..
void show(char* msg,float absTime, float deltaTime) {

	Serial.print(msg);
	Serial.print("\tAbs time : ");
	Serial.print(absTime,1);
	Serial.print("\tDelta time : ");
	Serial.println(deltaTime,1);
}
*/


void MIDItune::addMIDINote(bool Dn,int MIDIKey,float deltaTime) {
	
	if (Dn) {														// If we got a key down event..
		if (holding) {												// If we still have a note down..
			addNote(MIDI2Freq(heldNote),deltaTime);		// Single voice, cut off holding note here.
		} else if (deltaTime) {									// Else NOT holding a note. AND there IS a delta time..
			addNote(REST,deltaTime);							// Then we add a rest to fill the time.
		}																//
		heldNote = MIDIKey;										// Update the new holding MIDI note.
		holding = true;											// Make a note of that.
	} else {															// Else we got a lift..
		if (holding) {												// If we have a note down..
			if (MIDIKey==heldNote) {							// If the lift matches what we are holding..
				addNote(MIDI2Freq(heldNote),deltaTime);	// Finish the held note.
				holding = false;									// Clear the holding flag.
				heldNote = 0;										// Clear the holding note.
			}															//
		}																//
	}																	//
	absTimeMs = absTimeMs+deltaTime;							// In EVERY case we move absTimeMs up.
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
		
	SDFileErr = false;																						// Clear the global file error.
	MIDIFile = SD.open(filePath, FILE_READ);															// See if we can open the MIDI file.
	if (MIDIFile) {																							// If we got the MIDI file..
		dumpList();																								// Dump any list of notes we already have.
		absTimeMs	= 0;																						// Time starts at zero.
		MsPerTick	= 1;																						// One is a good default for this.
		holding		= false;																					// Clear the holding flag.			
		if (readMIDIHeader(&theMIDIHeader, MIDIFile)) {												// If we can read the MIDI header..
			if (readTrackHeader(&theTrackHeader, MIDIFile)) {										// If we can read the track header..
				done = false;																					// Ok, but we're not done.
				while(!done && !SDFileErr) {																// Loop 'till we're done, or hit a file error.
					if (readEventHeader(&anEventHeader, MIDIFile)) {								// If we can read an event header..
						if (isMetaTag(&anEventHeader)) {													// If this is a Meta event..
							readMetaEvent(&anEventHeader, &aMetaEvent, MIDIFile);					// Read the meta event.. (sheesh!)
							switch(aMetaEvent.metaType) {													// Let's see if it's a type we need to deal with.
								case 0x2F : done = true; break;											// 0x2F : End of file, we're done!
								case 0x51 :																		// 0x51 : The info. for doing tempo math.
									microsPerBeat = 0;														// Ok, zero out Micros per beat.
									MIDIFile.read(&aByte, 1);												// Grab a byte.
									microsPerBeat = aByte;													// This byte should be micros per beat.
									microsPerBeat = microsPerBeat << 16;								// Well actually The high byte is.
									MIDIFile.read(&aByte, 1);												// So we grab the next byte..
									temp = 0;																	// Zero out temp. (Probably not necessary)
									temp = aByte;																// Stuff that byte into (unsigned long) temp.
									temp = temp << 8;															// Now slide out over 8 bits.
									microsPerBeat = microsPerBeat | temp;								// Stomp these 8 bits into the 2nd byte of this mircos thing.
									MIDIFile.read(&aByte, 1);												// Grab another byte.
									temp = 0;																	// Zero out temp again.
									temp = aByte;																// Stuff this read byte into the low i8 bits of temp.
									microsPerBeat = microsPerBeat | aByte;								// Stuff this value into the low 8 bits of the micros thing.
									MsPerTick = (microsPerBeat/1000.0)/theMIDIHeader.timeDiv;	// Calculate the Ms per tick.
								break;																			// Ok done, let's bolt!
								default :																		// In all other cases..
									done = !jumpMetaEvent(&aMetaEvent, MIDIFile);					// We just jump past the event.
								break;																			// And bail.
							}																						//
						} else {																					// Else? else what? Oh yeah. Else it's NOT a Meta tag.
							switch(anEventHeader.eventType) {											// Check the event type..
								case MIDI_KEY_DN :															// A key down event..
									deltaMs = anEventHeader.deltaTime * MsPerTick;					// Calculate the delta time in milliseconds.
									addMIDINote(true,anEventHeader.param1,deltaMs);					// Pass in the midi note to the assembly people.
								break;																			// Bolt!
								case MIDI_KEY_UP :															// Key up event..
									deltaMs = anEventHeader.deltaTime * MsPerTick;					// Again, calculate the delta time in milliseconds. 
									addMIDINote(false,anEventHeader.param1,deltaMs);				// And pass the midi note off to the assembly people.
								break;																			// And we bolt.
							}																						//
						}																							//
					}																								//
				}																									//
			}																										//
		}																											//
		MIDIFile.close();																						// The last thing we do? Close the file.
	}																												//
}
           