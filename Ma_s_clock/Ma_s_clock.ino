#define OLED_CS       10
#define OLED_RST      6
#define POT_BTN       4

#include "Wire.h"
#include "RTClib.h"
#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "drawObj.h"
#include "label.h"
#include "lilParser.h"
#include "mechButton.h"


// *****************************************************
//                      litleClk
// *****************************************************


class litleClk :  public label {

  public:
          litleClk::litleClk(int x,int y,int w, int h);
  virtual ~litleClk(void);

          void  setTime(int hour,int minute);
          void  setVisable(bool trueFalse);
          bool  getVisable(void);
          void  set24(bool trueFalse);
          bool  get24(void);
  private:
          bool  mVisable;
          bool  m24;
};


litleClk::litleClk(int x,int y,int w, int h) 
  : label(x,y,w,h) {

  setJustify(TEXT_CENTER);
  setColors(&white,&black);
  setTextSize(3);
  mVisable = true;
  m24 = false;
}


litleClk::~litleClk(void) {  }


void litleClk::setTime(int hour,int minute) {

  char  timeStr[TEMP_BUFF_SIZE];

  if (m24) {
    if (hour>12) hour = hour - 12;
  }
  snprintf (timeStr,TEMP_BUFF_SIZE,"%2d:%2.2d",hour,minute);
  setValue(timeStr);
}


void litleClk::setVisable(bool trueFalse) {
  
  mVisable = trueFalse;
  if (mVisable) {
    setColors(&white);
  } else {
    setColors(&black);
  }
}


bool litleClk::getVisable(void) { return mVisable; }


void litleClk::set24(bool trueFalse) {
  
  m24 = trueFalse;
  
 }


bool litleClk::get24(void) { return m24; }


// *****************************************************
//                         main()
// *****************************************************



RTC_DS3231    rtc;
boolean       haveClock;
DateTime      drawtime;
mechButton    ourClicker(POT_BTN);
litleClk*     ourDisplay;
lilParser     ourParser;

enum        mainComs { noCommand, help, setHour, setMin, setSec, toggle24 };


void setup() {

  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    while (1);
  }
  Serial.println("got screen");
  haveClock = rtc.begin();
  if (haveClock) {                  // If we have the clock, make sure it runs on battery when unplugged.
    Wire.beginTransmission(0x68);   // address DS3231
    Wire.write(0x0E);               // select register
    Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();
  }
  ourParser.addCmd(help,"?");
  ourParser.addCmd(setHour,"hour");
  ourParser.addCmd(setMin,"minute");
  ourParser.addCmd(setSec,"sec"); 
  ourParser.addCmd(toggle24,"24");
  
  ourDisplay = new litleClk(20,20,100,24);
  viewList.addObj(ourDisplay);
  screen->fillScreen(&black);

  ourClicker.setCallback(btnClick);
}


void btnClick(void) {

  if (!ourClicker.trueFalse()) {
    ourDisplay->setVisable(!ourDisplay->getVisable());
  }
}


void checkParse(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = ourParser.addChar(inChar);
    switch (command) {                       
      case noCommand      : break;
      case help           : 
        Serial.println("Commands:");
        Serial.println("   hour followed by a number 0..23. Sets the hour.");
        Serial.println("   minute followed by a number 0..59. Sets the minutes.");
        Serial.println("   sec followed by a number 0..59. Sets the seconds.");
        Serial.println("   24 Sets 24 hour mode on/off.");
      break;
      case setHour  : doSetHour();    break;
      case setMin   : doSetMin();     break;
      case setSec   : doSetSec();     break;
      case toggle24 : do24();         break;
      default       : 
        Serial.println("I really don't know what your looking for.");
        Serial.println("Try typing ? for a list of commands.");
      break;
    }
  } 
}


void doSetHour(void) {

  byte  newVal;
  char* paramBuff;
  
  if (ourParser.numParams()) {
    paramBuff = ourParser.getParam();
    newVal = atoi (paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
    ourDisplay->setTime(newVal,now.minute());
    drawtime = now;
  }
}


void doSetMin(void) {

  byte  newVal;
  char* paramBuff;
  
  if (ourParser.numParams()) {
    paramBuff = ourParser.getParam();
    newVal = atoi (paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newVal,now.second()));
    ourDisplay->setTime(now.hour(),newVal);
    drawtime = now;
  }
}


void doSetSec(void) {
  
  byte  newVal;
  char* paramBuff;
  
  if (ourParser.numParams()) {
    paramBuff = ourParser.getParam();
    newVal = atoi (paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),newVal));
    ourDisplay->setTime(now.hour(),now.minute());
    drawtime = now;
  }
}


void do24(void) {

  DateTime  now;
  
  ourDisplay->set24(!ourDisplay->get24());
  if (haveClock) {
    now = rtc.now();
    ourDisplay->setTime(now.hour(),now.minute());
    drawtime = now;
  }
}


void checkScreen(void) {
  
  DateTime  now;
  
  if (haveClock) {
    now = rtc.now();
    
    if((now.minute()!=drawtime.minute())||(now.hour()!=drawtime.hour())) {  // If times have changed..
      ourDisplay->setTime(now.hour(),now.minute());
      drawtime = now;
    }
  }
}



void loop() {
  
  idle();
  checkParse();
  checkScreen();
}
