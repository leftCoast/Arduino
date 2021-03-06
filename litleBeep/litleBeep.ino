#include "squareWave.h"
#include "blinker.h"
#include "toneObj.h"
#include "debug.h"

#define BEEP_PIN    23
#define BEEP_PIN2   22
#define BEEP_PIN3    3
#define NOTE_TIME    200

class voice : public toneObj,
              public blinker {
   public:
          voice(int inPin);
  virtual ~voice(void);
        
  virtual void  play(int inHz,int inMs);
          void  checkVoice(void);

          bool marted;
};


voice::voice(int inPin) 
  : toneObj(inPin),
  blinker(inPin) {  marted = false; }


voice::~voice(void) {  }


void voice::play(int inHz,int inMs) {
  
  toneObj::play(-1,inMs);
  setPeriod(1000.0/inHz);
  setPercent(50);
  setOnOff(inHz>0);
  marted = true;
}


void voice::checkVoice(void) {
  
  if (marted) {
    if (!isPlaying()) {
       setOnOff(false);
       marted = false;
    }
  }
}


class voices :  public idler {

  public:
          voices(void);
  virtual ~voices(void);
        
          void    play(int inHz,int inMs);
          bool    isPlaying(void);
  virtual void    idle(void);
  
          voice*  mVoices[5];
};

  
voices::voices(void) {
  
  mVoices[0] = new voice(23);
  mVoices[1] = new voice(22);
  mVoices[2] = new voice(3);
  mVoices[3] = new voice(5);
  mVoices[4] = new voice(6);
}

voices::~voices(void) { for (int i=0;i<5;i++) delete(mVoices[i]); }


void voices::play(int inHz,int inMs) {

  int i;
    
  hookup();
  i = 0;
  while(mVoices[i]->isPlaying() && i<5) i++;
  if (i<5) { mVoices[i]->play(inHz,inMs); }
}


bool voices::isPlaying(void) {
  
  int i;

  i = 0;
  while(i<5) {
    if (mVoices[i]->isPlaying()) return true;
    i++;
  }
  return false;
}


void  voices::idle(void) {
  
  for(int i=0;i<5;i++) {
    mVoices[i]->checkVoice();
  }
}



voices ourVoices;

void setup() {

   for(int j=0;j<4;j++) {
      //ourVoices.play(NOTE_C3,NOTE_TIME*4);
      for (int i=0;i<4;i++) {
         ourVoices.play(NOTE_C4,NOTE_TIME);
         ourVoices.play(NOTE_E4,NOTE_TIME);
         ourVoices.play(NOTE_G4,NOTE_TIME);
         while(ourVoices.isPlaying()) idle();
         ourVoices.play(-1,NOTE_TIME/4);
         while(ourVoices.isPlaying()) idle();
      }
      while(ourVoices.isPlaying()) idle();
      //ourVoices.play(NOTE_D3,NOTE_TIME*4);
      for (int i=0;i<4;i++) {
         ourVoices.play(NOTE_D4,NOTE_TIME);
         ourVoices.play(NOTE_FS4,NOTE_TIME);
         ourVoices.play(NOTE_G4,NOTE_TIME);
         while(ourVoices.isPlaying()) idle();
         ourVoices.play(-1,NOTE_TIME/4);
         while(ourVoices.isPlaying()) idle();
      }
      //ourVoices.play(NOTE_C3,NOTE_TIME*4);
      for (int i=0;i<4;i++) {
         ourVoices.play(NOTE_C4,NOTE_TIME);
         ourVoices.play(NOTE_E4,NOTE_TIME);
         ourVoices.play(NOTE_G4,NOTE_TIME);
         while(ourVoices.isPlaying()) idle();
         ourVoices.play(-1,NOTE_TIME/4);
         while(ourVoices.isPlaying()) idle();
      }
      delay(NOTE_TIME);
      //ourVoices.play(NOTE_C3,NOTE_TIME*4);
      for (int i=0;i<3;i++) {
         ourVoices.play(NOTE_C4,NOTE_TIME);
         ourVoices.play(NOTE_E4,NOTE_TIME);
         ourVoices.play(NOTE_G4,NOTE_TIME);
         while(ourVoices.isPlaying()) idle();
         ourVoices.play(-1,NOTE_TIME/4);
         while(ourVoices.isPlaying()) idle();
      }
      //delay(NOTE_TIME*4);
   }
}

/*
void beep(blinker* voice,int freq) {

   voice->setPeriod(NOTE_TIME0.0/freq);
   voice->setPercent(50);
   voice->setOnOff(true);
}
*/

void loop() {
   idle();
  if (Serial.available() ) {
    Serial.println("Sorry");
  }
}
