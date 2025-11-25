#ifndef toneObj_h
#define toneObj_h

#include <SD.h>
#include <timeObj.h>
#include <lists.h>
#include <idlers.h>

// The notes!
#define REST		0	// Freq. of zero means "rest".

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


#define Q_NOTE			250.0		// Milliseconds

#define H_NOTE			2 * Q_NOTE
#define DH_NOTE		3 * Q_NOTE
#define W_NOTE			4 * Q_NOTE
#define E_NOTE			Q_NOTE/2.0
#define S_NOTE			Q_NOTE/4.0


// **** TO READ : SET YOUR EDITOR TAB TO 3 SPACES, OR THIS'LL LOOK LIKE A MESS. ****

 
// ***************************************************************************************
// Your classic tone playing object. This can play a tone and tell you when (this one)
// tone is finished.
//
// This takes a pin number and will play a tone on this pin. (The pin needs to be able to
// play a tone). Now, added to this, it can tell you when it's tone has ended. An that's
// the important bit. If we have a list of tones along with times that they should last..
// We can play a tune.
//
// Take a tone & time from our list, pop it in here and watch 'till this says it's done,
// them pop in the next tone. 
//
// So we need a list of tones : class tune is our list.
// And we need notes to play  : class note is our note.
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
// This is a note or a rest.
//
// Basically a chunk of time, either playing a note or NOT playing a note. class note is
// designed to be strung into a list by class tune. Then class tune can read down it's 
// list of notes, one by one, feeding notes to class toneObj.
// ***************************************************************************************


class note :	public linkListObj {

	public:
				note(int inHz,float inMs);
	virtual	~note(void);
			
				note* getNext(void);
				void	playSelf(toneObj* inTone,float timeMult);
				
				int	mHz;
				float	mMs;
};


				
// ***************************************************************************************
// And here is the note list, or tune. Single voice.
//
// This object will feed notes, (above) to the toneOBj, (above that) to create your
// melody. You build this list by feeding notes into it through the addNote() method. The
// idea is that when you create one of these tune objects, you'll give it the name of the
// tune it contains. You can have as many saved tunes as you like, as long as you have
// enough memory to hold them.
//
// When you want to play a tune, say.. WinnerTune. You have already loaded in the notes.
// Probably during setup(). And you have already created your toneObj "mrTone" with it's
// pin number.
//
// To create and play a song called "Winner" on pin "tonePin"..
// 
// Create the objects. Most likely global.
//
// toneObj	mrTone(tonePin);
// tune		WinnerTune;
//
//
// Fill the notes, Typically during setup(). for example..
//
// WinnerTune.addNote(NOTE_E6,Q_NOTE);
// WinnerTune.addNote(NOTE_E6,Q_NOTE);
// WinnerTune.addNote(NOTE_C6,E_NOTE);
// WinnerTune.addNote(NOTE_D6,E_NOTE);
//
//
// Start the tune playing..
//
// WinnerTune.startTune(&mrTone);
//	
// NOTE: See the '&' up ^^ there? You are not handing WinnerTune the actual MrTone. You
// are just telling WinnerTune where MrTone is. This is pointers. And some Arduino people
// fear them like nuclear winter. Don't fear the pointers.
//
//
// And in loop(), you could tell when the tune is completed by asking..
//
// WinnerTune.playing();
//
//
// And that's about it.
// 
// No wait, ONE LAST THING. As in ALL Left Coast things. Make the first function call in 
// loop() be  idle(); This will run everything in the background, like these tunes. While
// your code can be doing other things.
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
// MIDItune inherits from tune. This means everything you could do with tune, you can do
// with MIDItune. But, MIDItune adds the ability of building it's list of notes from a
// MIDI file from your SD card. (Single voice) Ain't that fancy? A LOT easer too!
//
// To use this? First you'll need to create your MIDI file with basically a one finger
// tune. How you do that is completely up to you.
//
// Next get the MIDI file onto an SD card that you can read with your Arduino. Again, this
// is something you'll have to solve.
//
// Create your toneObj just like before.
//
// Instead of creating a tune object, create a MIDItune object.
//
// Instead of filling it with notes by hand, call createTune(MIDIFilePath); To create the
// list of tones and rests.
//
// Then play the tune just like you played tune above. I'll look like..
//
// Create the objects..
//
// toneObj mrTone(tonePin);
// MIDItune	WinnerTune;
//
//
// Fill the notes..
//
// WinnerTune.createTune("winner.mid");
//
// 
// Start the tune playing..
//
// WinnerTune.startTune(&mrTone);
//
//
// Check when done..
//
// WinnerTune.playing();
//
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