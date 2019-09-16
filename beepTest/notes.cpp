
#include "notes.h"

// ***********************************
// ***********************************
// ***********************************
// ************* Voice ***************
// ***********************************
// ***********************************
// ***********************************


// PulseOut wants the pin number.
voice::voice(byte inPinNum,bool inverse)
  : PulseOut(inPinNum, C4,inverse),
  linkListObj() {

  playing = false;
  timedPlaying = false;
}


// Set up the sound we will play.
void voice::setUpSound(float periodMs) {

  setTime(periodMs,false);        // Set frequency.
  setWidth(PULSE_ON_MS);          // Set the length of the power pulse to speaker
}


// True, playes whatever note is set up. False, stops it.
void voice::play(bool playStop) {
  playing = playStop;
  if (playing) start();
}


// Are we playing? Does anyone care?
boolean voice::isPlaying(void) {
  return playing;
}


// Are we timed playing?
boolean voice::isTimedPlaying(void) {
  return timedPlaying;
}


// Playes for this long and stops.
void voice::timedPlay(float durationMs) {

  noteTimer.setTime(durationMs);
  timedPlaying = true;
  play(true);
}


// Our idle time.
void voice::idle(void) {

  if (timedPlaying) {
     if (noteTimer.ding()) {
      play(false);
      timedPlaying = false;
    }
  }
  if (playing) PulseOut::idle();
}


// ***********************************
// ***********************************
// ***********************************
// *********** musicBox  *************
// ***********************************
// ***********************************
// ***********************************

/*
// How many note buffers will we need? >= # Voices
musicBox::musicBox(int numNotes) {                              

  note* newNote;
  
  voicePtr = NULL;
  countsPerMeasure = DEF_COUNTS_MEASURE;
  countsPerSec = DEF_SOUNTS_SEC;
  playing = false;
  songStart = 0;
  notePtr = new note(this);         // You always get one note.
  for(int i=1;i<numNotes;i++) {
    newNote = new note(this);
    notePtr->addToEnd((listObj*)newNote);
  }
}


// Add a voice, assume its wired LOW = Sound.        
void  musicBox::addVoice(int pinNum, boolean inverse) {

  voice*  newVoice = new voice(pinNum,inverse);

  if (voicePtr) {
    voicePtr->addToEnd((listObj*)newVoice);
  } else {
    voicePtr = newVoice;
  }
}


// Setup up our timing.
void  musicBox::setupSong(int inCountsPerMeasure,float inCountsPerSec) { 
       
  countsPerMeasure = inCountsPerMeasure;
  countsPerSec = inCountsPerSec;
}


// Play the lovely melody.       
void  musicBox::startSong(void) { 
   songStart = micros();
   playing = true;
}


// Stop all this noise!
void  musicBox::endSong(void) { playing = false; }


// Grab a idle note. If posible.
note* musicBox::getNote(void) {

  note* trace = notePtr;             // Pointer to the list of notes.
  done = false;                      // Not done yet..
  while(!done)  {                    // While we have non NULL and that one is waiting..
    if (trace) {                     // We have a note?
      done = !trace->waiting();      // Done if it's not waiting.
      if (!done) {                   // Not done?
        trace = trace->nextNote();   // There is a note, so go to the next note on the list.
      }
    }
  return(trace);                    // However it ends, NULL or non waiting, pass it back.
}


// ***********************************
// ***********************************
// ***********************************
// ************* note  ***************
// ***********************************
// ***********************************
// ***********************************


// Create a note by stating things in music notation.
note::note(musicBox* inBox)
  : linkListObj() {


  

int countsPerMeasure, float countsPerSec, int measure, float count, float noteType, float pitch
note::setNote(float startMs, float durationMs, float periodMs) {

startCount = ((measure - 1) * countsPerMeasure) + count; // Here's where the note starts in the music.
  startMs = startCount * 1000;                            // And in miliseconds..
  setNote(startMs, noteType, pitch);                      // noteType & Pitch are already in Ms. Just pass 'em in.
}


  start = round(startMs * 1000);          // Absolute start time from song start.
  duration = round(durationMs * 1000);    // How long the note will play for.
  period = periodMs;                      // What it will sound like..
}


// Do WE come AFTER the note passed in?
boolean note::comesAfter(note* inNote) {

  if (start >= inNote->start + inNote->duration) return true; // We are at or beyond in time, return true..
  return false;                                               // Every other case. return false.
}


// Are WE somehow OVERLAPPING the note passed in?
boolean note::overlap(note* inNote) {

  if (start >= inNote->start + inNote->duration) return false;  // We are beyond in time return false..
  if (start + duration <= inNote->start) return false;          // We are completly before in time return false..
  return true;                                                  // Every other case is overlapped. return true.
}

// We need to know if this note should be sounding now.
boolean active(unsigned long startTime) {

  unsigned long now = micros();
  unsigned long absStart = startTime + start;
  return ((absStart <= now) && (absStart + duration > micros));
}


// Is our time passed? Is it time to be killed off?
boolean note::passed(unsigned long startTime) {
  return startTime + start + duration < micros();
}


boolean note::addNote(note* inNote) {

  boolean success = false;
  if (inNote != NULL) {                                  // The'll try anyhting I tell ya'.
    if (inNote->comesAfter(this) {                      // Clearly after us. Not overlapping.
    if (next = NULL) {                                 // We are the last note in the list.
        inNote->linkAfter(this);                        // Then the new note is the last.
        success = true;                                 // Its a success!
      } else {                                          // Not the last note in the list.
        note* nextNote = dynamic_cast note * (next);    // Find the next note in our list.
        success = nextNote->addNote(inNote);            // And do the recersive call.
      }
    }
  }
  return success;                                       // All said and done, return success.
}


float note::getPeriod(void) { return period; }

*/
