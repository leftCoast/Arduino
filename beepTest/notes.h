#ifndef notes_h
#define notes_h

#include <PulseOut.h>
#include <lists.h>

// *********************
// The notes themselves. How long in Ms is the note's period.

#define C3     7.644675484
#define CD3    7.215527816
#define D3     6.810597289
#define DE3    6.42838776
#define E3     6.067592986 
#define F3     5.727048852
#define FG3    5.405405405
#define G3     5.102040816
#define GA3    4.81579581
#define A3     4.545454545
#define AB3    4.290372404
#define B3     4.049566696

#define C4     3.811156437
#define CD4    3.607763908
#define D4     3.405298645
#define DE4    3.214090573
#define E4     3.033704457 
#define F4     2.86344243
#define FG4    2.702775751
#define G4     2.551020408
#define GA4    2.407897905
#define A4     2.272727273
#define AB4    2.145186202
#define B4     2.024783348

#define C5     1.911132346
#define CD5    
#define D5     
#define DE5    
#define E5      
#define F5     
#define FG5    
#define G5     
#define GA5    
#define A5     
#define AB5    
#define B5    

// *********************
// The written note types. How long in counts does the note last for?

#define   WhN     4     // Whole note.
#define   dHfN    3     // Dotted half note.
#define   HfN     2     // Half note.
#define   dQN     1.5   // Dotted quarter note.
#define   QN      1     // Quarter note.
#define   EthN    0.5   // Eighth note.
#define   SthN    0.25  // Sixteenth note.
#define   ThsN    0.125 // Thirty second note.

#define   DEF_COUNTS_MEASURE  4
#define   DEF_SOUNTS_SEC      2

// The idea behind the note is a small package that holds a "refined" note.
// And controls a voice for a specified time.

#define PULSE_ON_MS .075  // How long to wait with the power on.



// The idea behind the voice is something that can set up a wave form.
// (Square wave) and run this wave in the background.

// The plan is to have a voice for every sound output. 
// Notes will look for an unused voice and set it up to play
// a sound forever or for a specified time.

class voice : public PulseOut, public linkListObj {

  public:
        voice(byte inPinNum,boolean invert);
  
        void    setUpSound(float periodMs);         // Set up the sound we will play.
        void    play(boolean playStop);             // True, playes whatever note is set up. False, stops it.
        boolean isPlaying(void);                    // Are we playing? Does anyone care?
        boolean isTimedPlaying(void);               // Are we timed playing?
        void    timedPlay(float durationMs);        // Playes for this long and stops.
virtual void    idle(void);                         // Our idle time.

        boolean playing;
        boolean timedPlaying;
        timeObj noteTimer;
};

/*
class musicBox {

  public: 
              musicBox(int numNotes);                               // How many note buffers will we need? >= # Voices

        void  addVoice(int inPinNum, boolean inverse=true);         // Add a voice, assume its wired LOW = Sound.
        void  setupSong(int countsPerMeasure,float countsPerSec);   // Setup up our timing.
        void  startSong(void);                                      // Play the lovely melody.
        void  endSong(void);                                        // Stop all this noise!
        note* getNote(void);                                        // Grab a idle note.

        note*         notePtr;
        voice*        voicePtr;
        int           countsPerMeasure;
        float         countsPerSec;
        boolean       playing;
        unsigned long songStart;
}


class note : public timeObj, public linkListObj {

  public:
                note(musicBox* boxPtr);
                
                setNote(int measure,float count, byte noteType, float pitch);
             
        void    setNote(float startMs, float durationMs, float periodMs);
        
  private:
    unsigned long start;      // Absolute start time from song start.
    float        duration;   // How long the note will play for.
    float         period;     // The note. 1/Freq x 1,000. (Period in Ms)
     boolean     waiting;
};
*/

#endif
