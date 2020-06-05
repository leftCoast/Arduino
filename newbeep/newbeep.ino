#include "timeObj.h"
#include "idlers.h"

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
#define NOTE_C4  262
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


class note :   public linkListObj {

   public:
            note(int freq,float count);
   virtual  ~note(void);

   virtual  int   play(int pin,float msPerCount);
   
            int   mFreq;
            float mCount; 
};


note::note(int freq,float count) {

   mFreq = freq;
   mCount = count;
}


note::~note(void) {  }


int note::play(int pin,float msPerCount) {

   int duration;

   duration = round(mCount*msPerCount);   // Calculate our current duration. (in Ms)
   if (mFreq) {                           // If we have a tone..
      tone(pin,mFreq,duration);           // Lets hear it.
   } else {                               // Else, no tone?
      noTone(pin);                        // Give it a rest!
   }
   return duration;                       // Pass back how long this will take.
}
 

class tune :   public idler,
               public linkList {

   public:
            tune(int pin);
   virtual  ~tune(void);

            void  addNote(int freq,float count);
            void  startTune(float msPerCount);
            void  stopTune(void);
            bool  playing(void);
   virtual  void  idle(void);

            int      mPin;
            float    mMsPerCount;
            note*    mPlaying;
            timeObj  timer;
};


tune::tune(int pin) {
   
   mPin     = pin;
   mPlaying = NULL;
}


tune::~tune(void) {  }


void tune::addNote(int freq,float count) {

   note* newNote;

   hookup();
   newNote = new note(freq,count);
   addToEnd(newNote);
}


void tune::startTune(float msPerCount) {

   note* firstNote;

   firstNote = (note*)getFirst();
   mMsPerCount = msPerCount;
   if (firstNote) {
      timer.setTime(firstNote->play(mPin,mMsPerCount));
      timer.start();
      mPlaying = firstNote;
   }
}


void tune::stopTune(void) {

   noTone(mPin);
   mPlaying = NULL; 
}


bool tune::playing(void) { return mPlaying!=NULL; }


void tune::idle(void) {

   note* nextNote;
   
   if (mPlaying) {
      if (timer.ding()) {
         nextNote = (note*) mPlaying->getNext();
         if (nextNote) {
            timer.setTime(nextNote->play(mPin,mMsPerCount));
            timer.start();
         }
         mPlaying = nextNote;
      }
   }
}



#define BEEP_PIN    23

int ringTone[] =          { NOTE_E5, NOTE_D5, NOTE_F4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_A4, NOTE_C4, NOTE_E4, 0 };
int ringToneDurations[] = { 250,     250,     500,     500,     250,     250,     500,     500,     250,     250,     500,     500,     1000 };

int youGotF[] =   { NOTE_E4,  NOTE_G4, NOTE_E4, NOTE_A5, NOTE_G4, NOTE_E4, 0,       NOTE_A5,  NOTE_C5,   NOTE_A5, NOTE_C5, NOTE_A5, NOTE_G4, 0,       NOTE_A5, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_B5, NOTE_B5, NOTE_C5  };
float youGotC[] = { .25,      .125,    .125,    .25,     .25,     .25,     .75,     .25,      .125,      .125,    .25,     .25,     .25,     .75,     .25,     .25,     .25,     .25,     .25,     .25,     .125     };
tune aTune(BEEP_PIN);
void setup() {

  for (int index=0;index<21;index++) {
      //aTune.addNote(ringTone[index],ringToneDurations[index]); 
      aTune.addNote(youGotF[index],youGotC[index]);
  }
  
  pinMode(BEEP_PIN,OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
}


void loop() {
   idle();
   if (!aTune.playing()) { delay(2000);aTune.startTune(1000); }
 }
