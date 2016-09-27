#ifndef notes_h
#define notes_h

#include <PulseOut.h>
#include <lists.h>

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


class note : public linkListObj {

  public:
             note(int countsPerMeasure,float countsPerSec,int measure,float count, byte noteType, float pitch);
             note(float startMs, float durationMs, float periodMs);

     boolean overlap(note* inNote);

  private:
    unsigned long start;      // Absolute start time from song start.
    unsigned long duration;   // How long the note will play for.
    unsigned long period;     // The note. 1/Freq x 1,000,000.
};


class voice : public PulseOut, public linkList {

  public:
            voice(byte inPinNum);

    void    play(void);             // Play this list of notes.
    void    pause(void);            // Stop this list of notes.
    boolean playing(void);          // Are we playing?
  
    boolean addNote(note* inNote);  // Try to add a note, returns success.
};

#endif
