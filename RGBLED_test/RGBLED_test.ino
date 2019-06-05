#include <lilParser.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

#define TIME_RSHIFT 4

mapper  redMap(0,255,0,75);
mapper  greenMap(0,255,0,180);

class RGBLED  : public colorObj,
                public idler {

  public:
  
          RGBLED(int redPin, int greenPin, int bluePin);
  virtual ~RGBLED(void);

  virtual void  idle(void);

          timeObj startTimer;
          timeObj redTimer;
          timeObj greenTimer;
          timeObj blueTimer;
          int     mRedPin;
          int     mGreenPin;
          int     mBluePin;
          bool     mInit;
};


RGBLED::RGBLED(int redPin, int greenPin, int bluePin) {

  mRedPin     = redPin;
  mGreenPin   = greenPin;
  mBluePin    = bluePin;
  mInit       = false;
  startTimer.setTime(256>>TIME_RSHIFT);
}

RGBLED::~RGBLED(void) { }


void RGBLED::idle(void) {

  byte colorTime;
  
  if (!mInit) {
    pinMode(mRedPin, OUTPUT);
    pinMode(mGreenPin, OUTPUT);
    pinMode(mBluePin, OUTPUT);
    startTimer.start();
    mInit = true;
  } else {
    if (startTimer.ding()) {
      analogWrite(mRedPin,255-redMap.Map(red));
      analogWrite(mGreenPin,255-greenMap.Map(green));
      analogWrite(mBluePin,255-blue);
      startTimer.start();
    }
  }
}


/*
void RGBLED::idle(void) {

  byte colorTime;
  
  if (!mInit) {
    pinMode(mRedPin, OUTPUT);
    pinMode(mGreenPin, OUTPUT);
    pinMode(mBluePin, OUTPUT);
    startTimer.start();
    mInit = true;
  } else {
    if (startTimer.ding()) {
      colorTime = red>>TIME_RSHIFT;
      redTimer.setTime(colorTime);
      if (colorTime) digitalWrite(mRedPin, LOW);
      redTimer.start();

      colorTime = green>>TIME_RSHIFT;
      greenTimer.setTime(colorTime);
      if (colorTime) digitalWrite(mGreenPin, LOW);
      greenTimer.start();

      colorTime = blue>>TIME_RSHIFT;
      blueTimer.setTime(colorTime);
      if (colorTime) digitalWrite(mBluePin, LOW);
      blueTimer.start();
      
      startTimer.start();
    } else {
      if (redTimer.ding()) digitalWrite(mRedPin, HIGH);
      if (greenTimer.ding()) digitalWrite(mGreenPin, HIGH);
      if (blueTimer.ding()) digitalWrite(mBluePin, HIGH);
    }
  }
}
*/

// *******************************

#define COLOR_STEP  0.0001
#define MAX_COLOR_INDEX 4
#define COLOR_STEP_MS   300

RGBLED          theLED(9,10,11);
colorMultiMap   theColors;
float           colorIndex;
timeObj         stepTimer(COLOR_STEP_MS);
lilParser mParser;

enum      commands { noCommand, setOff, setRed, setGreen, setBlue };

void setup() {
  Serial.begin(9600);
  theLED.setColor(LC_GREEN);
  theLED.blend(&blue,50);
  theLED.hookup();

  theColors.addColor(0,&green);
  theColors.addColor(1,&red);
  theColors.addColor(3,&blue);
  theColors.addColor(4,&green);
  stepTimer.start();
  colorIndex = 0;

  mParser.addCmd(setRed, "r");
  mParser.addCmd(setGreen, "g");
  mParser.addCmd(setBlue, "b");
  mParser.addCmd(setOff, "off");
  
  
  pinMode(0,INPUT_PULLUP);
}


void setColor(int color) {

  char* paramBuff;
  int   value;
  byte  r,g,b;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (paramBuff) {
      value = atoi(paramBuff);
      free(paramBuff);
      r = theLED.getRed();
      g = theLED.getGreen();
      b = theLED.getBlue();
      switch(color) {
        case setRed   : theLED.setColor(value,g,b); break;
        case setGreen : theLED.setColor(r,value,b); break;
        case setBlue  : theLED.setColor(r,g,value); break;
      }
      Serial.print("red:");Serial.print(theLED.getRed());
      Serial.print(" green:");Serial.print(theLED.getGreen());
      Serial.print(" blue:");Serial.println(theLED.getBlue());
    }
  }
}


void loop() {
  
  colorObj  aColor;
  char  inChar;
  int   command;
  
  idle();
  if (!digitalRead(0)) { theLED.setColor(LC_NAVY);return; }
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand : break;
      case setRed     :
      case setGreen   :
      case setBlue    : setColor(command); break;
      case setOff     : theLED.setColor(0,0,0);
    }
  } else {
    if (stepTimer.ding()) {
      aColor = theColors.Map(colorIndex);
      theLED.setColor(&aColor);
      colorIndex = colorIndex + COLOR_STEP;
      if (colorIndex>MAX_COLOR_INDEX) colorIndex = 0;
    }
  }
}
