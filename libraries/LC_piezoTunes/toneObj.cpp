#include "toneObj.h"
#include "Arduino.h"



// ***************************************************************************************
// Your classic tone playing object. This can tell you when the tone is finished.
// ***************************************************************************************


toneObj::toneObj(int inPin) { mPin = inPin; }


toneObj::~toneObj(void) {  }
				
				
void toneObj::play(int inHz,int inMs) {
	
	if (inHz>0) {
		tone(mPin,inHz,inMs);
	} else {
		noTone(mPin);
	}
	setTime(inMs,true);
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

      	      
//
//
//
/*
WriteVarLen(long value) {

	long buffer;
	
	buffer = value & 0x7f;
	while((value >>= 7) > 0) {
		buffer <<= 8;
		buffer |= 0x80;
		buffer += (value & 0x7f);
	}
	while (1) {
 		putc(buffer,outfile);
 		if(buffer & 0x80) {
 			buffer >>= 8;
 		} else
 			break;
 		}
 	}
}


unsigned long ReadVarLen(void) {

	unsigned long	value;
	byte				c;

	if((value = getc(infile)) & 0x80) {
 		value &= 0x7f;
 		do {
  			value = (value << 7) + ((c = getc(infile))) & 0x7f);
  		} while (c & 0x80);
 	}
	return(value);
}
*/