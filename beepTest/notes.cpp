
#include "notes.h"



// ************* note  ***************

note::note(int countsPerMeasure,float countsPerSec,int measure,float count, float noteType, float pitch)
: linkListObj() {

  float startCount;
  float startMs;

  startCount = ((measure-1) * countsPerMeasure) + count;  // Here's where the note starts in the music.
  startMs = startCount * 1000;                            // And in miliseconds..
  setNote(startMs,noteType,pitch);                        // noteType & Pitch are already in Ms. Just pass 'em in.
}


note::setNote(float startMs, float durationMs, float periodMs) {

  start = round(startMs * 1000);          // Absolute start time from song start.
  duration = round(durationMs * 1000);    // How long the note will play for.
  period = round(periodMs * 1000);        // What it will sound like..
}


// Do WE come AFTER the note passed in?
boolean note::comesAfter(note* inNote) {

      if (start >= inNote->start + inNote->duration) return true; // We are at or beyond in time, return true..
      return false;                                               // Every other case. return false.
}


// Are WE somehow OVERLAPPING the note passed in?
boolean note::overlap(note* inNote) {

      if (start >= inNote->start + inNote->duration) return false; // We are beyond in time return false..
      if (start+duration<=inNote->start) return false;             // We are completly before in time return false..
      return true;                                               // Every other case is overlapped. return true. 
}



// ************* Voice ***************


:// PulseOut wants the pin number, linkList wants to know if we own the notes.
voice::voice(byte inPinNum) : PulseOut(inPinNum,C4), linkList(false) { } 

// Play this list of notes.
void voice::play(void) {

  startTime = micros();
  if (!isEmpty() {          // Have notes.
    playing = true;         // Just note it? Really?
  } else {                  // Else we have no notes..
    playing = false;         // Well we're not. Should be false already though.
  } 
}


// Stop playing, reset to beginning.
void voice::halt(void) {
    
}


// Are we playing?
boolean voice::isPlaying(void) { return playing; }

}

// Try to add a note, returns success.
boolean addNote(note* inNote) {

  note* trace = dynamic_cast note* (theList);
  boolean success = false;
  
  

}

