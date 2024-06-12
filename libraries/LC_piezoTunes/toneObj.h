#ifndef toneObj_h
#define toneObj_h

#include <SD.h>
#include <timeObj.h>
#include <lists.h>
#include <idlers.h>

// The notes!
#define REST		0

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A2  55
#define NOTE_AS2 58
#define NOTE_B2  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A3  110
#define NOTE_AS3 117
#define NOTE_B3  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A4  220
#define NOTE_AS4 233
#define NOTE_B4  247
#define NOTE_C4  262	// I think this is middle C.
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A5  440
#define NOTE_AS5 466
#define NOTE_B5  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A6  880
#define NOTE_AS6 932
#define NOTE_B6  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A7  1760
#define NOTE_AS7 1865
#define NOTE_B7  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A8  3520
#define NOTE_AS8 3729
#define NOTE_B8  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


#define Q_NOTE			250.0

#define H_NOTE			2 * Q_NOTE
#define DH_NOTE		3 * Q_NOTE
#define W_NOTE			4 * Q_NOTE
#define E_NOTE			Q_NOTE/2.0
#define S_NOTE			Q_NOTE/4.0

// ***************************************************************************************
// Your classic tone playing object. This can tell you when this one tone is finished.
// ***************************************************************************************

class toneObj : public	timeObj {

	public:
				toneObj(int inPin);
	virtual	~toneObj(void);
				
	virtual	void	play(int inHz,float inMs);
	virtual	bool	isPlaying(void);
				
				int	mPin;
};


// ***************************************************************************************
// Play a tune. basically a list of notes and rests. Use a toneObj above to play them.
// This would be a note..
// ***************************************************************************************

class note :	public linkListObj {

	public:
				note(int inHz,float inMs);
				~note(void);
			
				note* getNext(void);
				void	playSelf(toneObj* inTone,float timeMult);
				
				int	mHz;
				float	mMs;
};

				
// ***************************************************************************************
// And here is the note list, or tune. Single voice.
// ***************************************************************************************

class tune :	public idler,
               public linkList {

   public:
            tune();
   virtual  ~tune(void);

   virtual  void  addNote(int inHz,float inMs);
            void	adjustSpeed(float inTimeMult=1);
            void  startTune(toneObj* inTone,int inIndex=0);
            int  	stopTune(void);
            bool  playing(void);
   virtual  void  idle(void);

            float    mTimeMult;
            toneObj*	mTone;
            note*    mPlaying;
};



// ***************************************************************************************
// Basically the same as a tune but.. Build from a MIDI file. (Single voice)
// ***************************************************************************************


class MIDItune :	public tune {

	public:
				MIDItune(void);
	virtual	~MIDItune(void);

   virtual  void	createTune(const char* filePath);
           
	protected :
				void	addMIDINote(bool Dn,int MIDIKey,float inDeltaMs);
	
				float	absTimeMs;
				float	MsPerTick;
				bool	holding;
				int 	heldNote;
};

#endif