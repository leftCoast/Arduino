
#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <timeObj.h>

#define PULSE_OFFSET 200        // In the end, it seems like everything takes too long. By this much.
#define MIN_PULSE    1000
#define MID_PULSE    1500
#define MAX_PULSE    2000


#define ROLL_PIN     A0
#define PITCH_PIN    A1
#define YAW_PIN      A2 
#define THROTTLE_PIN A3
#define AUX_PIN      A4
#define HAT_PIN      A5
#define BEEPER_PIN   8

#define HAT_FWD      1004
#define HAT_BACK     194
#define HAT_LEFT     138
#define HAT_RIGHT    333
#define HAT_WINDOW   5

#define HAT_FWD_LOW     HAT_FWD-HAT_WINDOW
#define HAT_FWD_HIGH    HAT_FWD+HAT_WINDOW
#define HAT_BACK_LOW    HAT_BACK-HAT_WINDOW
#define HAT_BACK_HIGH   HAT_BACK+HAT_WINDOW
#define HAT_LEFT_LOW    HAT_LEFT-HAT_WINDOW
#define HAT_LEFT_HIGH   HAT_LEFT+HAT_WINDOW
#define HAT_RIGHT_LOW   HAT_RIGHT-HAT_WINDOW
#define HAT_RIGHT_HIGH  HAT_RIGHT+HAT_WINDOW

#define DEBOUNCE_TIME   3      // All these are in miliseconds
#define BEEP_TIME       10
#define LONG_BEEP_TIME  25
#define OFF_TIME        150

#define TRIM_AMOUNT     5      // microseconds. About the same amount as the DX5e uses.

//#define IDLE_STATE   HIGH      // digitalWrite is SLOW. Try hardcoding?
//#define ACTIVE_STATE LOW

#define IDLE_STATE   PORTD |= B10000000;
#define ACTIVE_STATE PORTD &= B01111111;

#define TIMING_PULSE 200         // microseconds.
#define OUT_PIN      7
#define REFRESH_TIME 21          // miliseconds.  mili, NOT micro!

#define NUM_CHANNELS 6          // Oddly, DX5e has 6 channels.
#define THROTTLE     0
#define AILERON      1
#define ELEVATOR     2
#define RUDDER       3
#define AUX          4

// ****************************** beeper ****************************** 

class beeper : 
public idler {

public:
  beeper(int inPin,int inOnState=LOW,int inOffState=HIGH);
  ~beeper(void);

  void beep(float inBeepTime=10,float inOffTime=200,int inNumBeeps=1);
  void idle(void);

private:
  int      pin;
  int      onState;
  int      offState;
  int      beepCount;
  boolean  beepOn;
  boolean  init;
  timeObj*  beepTimer;
  timeObj*  offTimer;
};


beeper::beeper(int inPin,int inOnState,int inOffState) {

  beepTimer = NULL;        // Flagged for memory management.
  offTimer = NULL;
  pin = inPin;
  onState = inOnState;
  offState = inOffState;
  beepOn = false;
  init = false;
}


beeper::~beeper(void) {

  if (beepTimer) delete beepTimer;
  if (offTimer) delete offTimer;
}


void beeper::beep(float inBeepTime,float inOffTime,int inNumBeeps) {


  beepCount = inNumBeeps;                    // Grab the number of beeps we are doing.
  if (!init) {                               // Fist call? 
    pinMode(pin, OUTPUT);                    // Setup the pin.
    beepTimer = new timeObj(inBeepTime);     // Create the beepTimer.
    offTimer = new timeObj(inOffTime);       // Create the offTimer.
    hookup();                                // Hookup to recieve idle() calls.
    init = true;                             // No longer first call.
  } 
  else {                                     // Been here before?
    beepTimer->setTime(inBeepTime,false);    // Just set the numbers then.
    offTimer->setTime(inOffTime,false);
  }
  digitalWrite(pin, onState);                //  Fire up the noise.
  beepTimer->start();                        // Start the timer.
  beepOn = true;                             // We're beepin!
}



void beeper::idle(void) {

  if (beepOn) {                                // If we're making noise?
    if (beepTimer->ding()) {                   // Time to stop making noise?
      offTimer->start();                       // Start timing the silence.
      digitalWrite(pin, offState);             // SHUSH!
      beepOn = false;                          // We're no longer making noise.
      beepCount--;                             // Decrement beep counter.
    }
  } 
  else if (offTimer->ding() && beepCount>0) {  // Maybe time to start making noise again?
    digitalWrite(pin, onState);                // Fire up the noise.
    beepTimer->start();                        // Start the timer.
    beepOn = true;                             // We're beepin!
  }
}


timeObj hatTimer(DEBOUNCE_TIME);

int     channels[NUM_CHANNELS]; // Also in microseconds.

enum state { 
  running, calibStick, calibPetals };
state currState = running;

enum hat {
  off, forward, back, left, right };
hat currHat = off;
boolean hatHandled = false;

int pitchTrim = 0;
int rollTrim = 0;

beeper theBeeper(BEEPER_PIN);


multiMap rollMapper;
multiMap pitchMapper;
multiMap yawMapper;
mapper throttleMapper(0,MIN_PULSE,1023,MAX_PULSE);
mapper auxMapper(0,MIN_PULSE,1023,MAX_PULSE);

timeObj outTimer(REFRESH_TIME);

void setupMappers(void) {

  rollMapper.addPoint(1023,MIN_PULSE);
  rollMapper.addPoint(512,MID_PULSE);
  rollMapper.addPoint(0,MAX_PULSE);

  pitchMapper.addPoint(1023,MAX_PULSE);
  pitchMapper.addPoint(512,MID_PULSE);
  pitchMapper.addPoint(0,MIN_PULSE);

  yawMapper.addPoint(0,MIN_PULSE);
  yawMapper.addPoint(512,MID_PULSE);
  yawMapper.addPoint(1023,MAX_PULSE);
}


void setup(void) {

  //Serial.begin(9600);
  setupMappers();

  // Give the array some nice non-crazy defaults.
  for (int i=0;i<NUM_CHANNELS;i++) { 
    channels[i] = MID_PULSE;
  }
  channels[AUX] = MAX_PULSE;

  // Setup utput pin for the signal.
  pinMode(OUT_PIN, OUTPUT);     
  //digitalWrite(OUT_PIN, IDLE_STATE);
  IDLE_STATE;

  // Fire up the timer..
  outTimer.start();

  // The plan is to use states.. Later. So for now, we're "running".
  currState = running;
}

void trimDown(void) {

  pitchTrim -= TRIM_AMOUNT;
  if (pitchTrim!=0) {
    theBeeper.beep(BEEP_TIME);
  } 
  else {
    theBeeper.beep(LONG_BEEP_TIME,OFF_TIME,2);
  }
}


void trimUp(void) {

  pitchTrim += TRIM_AMOUNT;
  if (pitchTrim!=0) {
    theBeeper.beep(BEEP_TIME);
  } 
  else {
    theBeeper.beep(LONG_BEEP_TIME,OFF_TIME,2);
  }
}

void trimLeft(void) {

  rollTrim += TRIM_AMOUNT;
  if (rollTrim!=0){
    theBeeper.beep(BEEP_TIME);
  }
  else{
    theBeeper.beep(LONG_BEEP_TIME,OFF_TIME,2);
  }
}


void trimRight(void) {

  rollTrim -= TRIM_AMOUNT;
  if (rollTrim!=0) {
    theBeeper.beep(BEEP_TIME);
  } 
  else{
    theBeeper.beep(LONG_BEEP_TIME,OFF_TIME,2);
  }
}


void handleHat(hat inHat);
void handleHat(hat inHat) {

  if (inHat!=currHat) {
    hatTimer.start();
    currHat = inHat;
    hatHandled = false;
  } 
  else if (hatTimer.ding() && hatHandled==false) {
    currHat = inHat;
    switch(inHat) {
      case off : break;
      case forward : trimDown(); break;
      case back : trimUp(); break;
      case left : trimLeft(); break;
      case right : trimRight(); break;
    }
    hatHandled = true;
  }
}

hat readHat(void); 
hat readHat(void) {

  int hatVal = analogRead(HAT_PIN);
  if (hatVal>=HAT_FWD_LOW && hatVal<=HAT_FWD_HIGH) return(forward);
  else if (hatVal>=HAT_BACK_LOW && hatVal<=HAT_BACK_HIGH) return(back);
  else if (hatVal>=HAT_LEFT_LOW && hatVal<=HAT_LEFT_HIGH) return(left);
  else if (hatVal>=HAT_RIGHT_LOW && hatVal<=HAT_RIGHT_HIGH) return(right);
  else return(off);
}


// Grab all the analog values from the pins, scale 'em and stuff 'em into the channel array.
void readValues(void) {

  channels[THROTTLE] = round(throttleMapper.Map(analogRead(THROTTLE_PIN)));
  channels[AILERON] = round(rollMapper.Map(analogRead(ROLL_PIN)));
  channels[AILERON] = channels[AILERON] + rollTrim;
  channels[ELEVATOR] = round(pitchMapper.Map(analogRead(PITCH_PIN)));
  channels[ELEVATOR] = channels[ELEVATOR] + pitchTrim;
  channels[RUDDER] = round(yawMapper.Map(analogRead(YAW_PIN)));
  //channels[AUX] = round(auxMapper.Map(analogRead(AUX_PIN)));


  hat hatVal = readHat();
  handleHat(hatVal);
  /*
  Serial.print("THROTTLE  : ");Serial.println(channels[THROTTLE]);
   Serial.print("AILERON   : ");Serial.print(channels[AILERON]);Serial.print("  Raw :");Serial.println(analogRead(ROLL_PIN));
   Serial.print("ELEVATOR  : ");Serial.print(channels[ELEVATOR]);Serial.print("  Raw :");Serial.println(analogRead(PITCH_PIN));
   Serial.print("RUDDER    : ");Serial.print(channels[RUDDER]);Serial.print("  Raw :");Serial.println(analogRead(YAW_PIN));
   Serial.print("AUX       : ");Serial.print(channels[AUX]);Serial.print("  Raw :");Serial.println(analogRead(AUX_PIN));
   Serial.print("HAT       : ");
   switch (hatVal) {
   case off : Serial.print("Off     "); break;
   case forward : Serial.print("Forward "); break;
   case back : Serial.print("Back    "); break;
   case left : Serial.print("Left    "); break;
   case right : Serial.print("Right   "); break;
   }
   Serial.print("  Raw :");Serial.println(analogRead(HAT_PIN));
   Serial.println();
   */
}


// We have the array and the numnber of channels. Stuff 'em out the wire.
// There really is nothing else going on. So, lets just use delays to make 
// life easy for the poor code writer huh?
void sendPulse(int* pulseList,byte numChannels) {

  for (int i=0;i<numChannels;i++) {
    ACTIVE_STATE;     // Wake up!
    delayMicroseconds(TIMING_PULSE);                                // Odd, it wants an offset for data but not for this? What are we missing here?            
    IDLE_STATE;       // Now listen!
    delayMicroseconds(pulseList[i]-PULSE_OFFSET);    // time this!
  }
  ACTIVE_STATE;       // end pulse.
  delayMicroseconds(TIMING_PULSE);               
  IDLE_STATE;
}


//This is called from the main loop if we're currently "running".
void runLoop(void) {

  if (outTimer.ding()) {                   // Is it time yet?
    sendPulse(channels,NUM_CHANNELS);      // Guess so. First Send the pulse out.
    readValues();                          // Now, we know we have some time to kill. Read positioins!
    //showControls();                        // IF we have a light display. Set it now.
    outTimer.stepTime();                   // Reset the timer from the last time it expired.
  }
}


// Main loop..
void loop() {

  idle();                                    // There may be background things. Let 'em run.
  switch (currState) {                       // Handle whatever thing we're doing now.
  case running : 
    runLoop(); 
    break;
    //calibStick : calStickLoop(); break;
    //calibPetals : calPetalsLoop(); break;
  }

}




