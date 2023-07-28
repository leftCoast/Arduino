
/*  Stuff from screen. Maybe we'll do another?
#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "drawObj.h"
#include "label.h"

#define OLED_CS       10   // from the original color clock.
#define OLED_RST      6
#define POT_BTN       4

 
  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    while (1);
  }
  Serial.println("got screen");
  
*/


#include <EEPROM.h>
#include "Wire.h"
#include "RTClib.h"
#include <Adafruit_LEDBackpack.h>
//#include <SparkFunSi4703.h>
#include <Si4703.h>
#include "lilParser.h"
#include "strTools.h"


#define TEMP_BUFF_SIZE  20

#define RAD_RST   2
#define RAD_STC   3
#define I2C_DAT   18
#define I2C_CLK   19

// Select Region 
#define BAND  BAND_US_EU    // Select band frequency range
#define SPACE SPACE_100KHz  // Select band spacing
#define DE    DE_75us       // Select de-emphasis

// Seek Settings
#define SKMODE  SKMODE_STOP // Stop when reaching band limit
#define SEEKTH  00          // Seek RSSI Threshold
#define SKCNT   SKCNT_DIS   // Clicks Number Threshold
#define SKSNR   SKSNR_DIS   // Signal/Noise Ratio

// *****************************************************
//                         main()
// *****************************************************

struct paramType {
   bool  show24;
   int   brightVal;
   int   channel;
   int   volume;
};


RTC_DS3231           rtc;
boolean              haveClock;
DateTime             drawtime;
lilParser            ourParser;
Adafruit_8x8matrix   matrix[4];  // The big 'Ol LED panel.
paramType            params;
Si4703               radio(RAD_RST, I2C_DAT, I2C_CLK, RAD_STC, BAND, SPACE, DE, SKMODE, SEEKTH, SKCNT, SKSNR);

enum  mainComs {
   noCommand,
   help,
   dispTime,
   setHour,
   setMin,
   setSec,
   toggle24,
   see,
   bright,
   ron,
   roff,
   rSetChan,
   rSeekUp,
   rSeekDn,
   rSetVol,
   rReadRDS
};



void setup() {

  readParams();
  for (int i=0;i<4;i++) {
     matrix[i].begin(0x70+i);              // pass in the address
     matrix[i].setRotation(2);
     matrix[i].setTextSize(1);
     matrix[i].setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
     matrix[i].setTextColor(LED_ON);
     matrix[i].setBrightness(params.brightVal);
  }
 
  haveClock = rtc.begin();
  if (haveClock) {                  // If we have the clock, make sure it runs on battery when unplugged.
    Wire.beginTransmission(0x68);   // address DS3231
    Wire.write(0x0E);               // select register
    Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();
    showTime();
  } else {
   Serial.println("No clock :(");
  }

  radio.start();
  radio.setVolume(params.volume);
  radio.setChannel(params.channel);
  
  ourParser.addCmd(help,"?");
  ourParser.addCmd(dispTime,"time");
  ourParser.addCmd(setHour,"hour");
  ourParser.addCmd(setMin,"minute");
  ourParser.addCmd(setSec,"sec"); 
  ourParser.addCmd(toggle24,"24");
  ourParser.addCmd(see,"see");
  ourParser.addCmd(bright,"bright");
  ourParser.addCmd(ron,"on");
  ourParser.addCmd(roff,"off");
  ourParser.addCmd(rSetChan,"tune");
  ourParser.addCmd(rSeekUp,"+");
  ourParser.addCmd(rSeekDn,"-");
  ourParser.addCmd(rSetVol,"vol");
  ourParser.addCmd(rReadRDS,"rds");
}


// ********** param stuff **********


void readParams() { EEPROM.get(0,params); }


void writeParams() { EEPROM.put(0,params); }



// ********** display stuff **********


void matrixPrint(char* inStr,int rOffset) {

   for (int i=0;i<4;i++) {
      matrix[i].clear();
      matrix[i].setCursor((-i*8)-rOffset,0);
      matrix[i].print(inStr);
   }
   for (int i=0;i<4;i++) {
      matrix[i].writeDisplay();
   }
}


char* timeStr(int hour,int minute) {

  if (resizeBuff(TEMP_BUFF_SIZE,&returnStr)) {
     if (!params.show24) {
       if (hour>12) {
         hour = hour - 12;
       } else if (hour==0) {
         hour = 12;
       }
     }
     snprintf (returnStr,TEMP_BUFF_SIZE,"%2d:%2.2d",hour,minute);
  } else {
      heapStr(&returnStr,"oops");
  }
  return returnStr;
}


void showTime(void) {

   char*    theTimeStr;
   DateTime now;

   now = rtc.now();
   theTimeStr = timeStr(now.hour(),now.minute());
   matrixPrint(theTimeStr,-1);
   drawtime = now;
}


bool timeChanged(void) {

   DateTime now;

    now = rtc.now();
    return (drawtime.hour()!=now.hour())||(drawtime.minute()!=now.minute());
}



// ********** radio stuff **********


int freqToInt(float freq) { return round(freq*100); }


float intToFreq(int radioChannel) { return radioChannel/100.0; }


// ********** parser stuff **********


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
        Serial.println("   hour\t\tFollowed by a number 0..23. Sets the hour.");
        Serial.println("   minute\tFollowed by a number 0..59. Sets the minutes.");
        Serial.println("   sec\t\tFollowed by a number 0..59. Sets the seconds.");
        Serial.println("   24\t\tSets 24 hour mode on/off.");
        Serial.println("   see\t\tFollowed by two numbers formats that time on the display.");
        Serial.println("   bright\tFollowed by one nuumber 0..15 sets the display brightness.");
        Serial.println("   on\t\tTurns on the radio.");
        Serial.println("   off\t\tTurns off the radio. (But vol 0 is quieter)");
        Serial.println("   tune\t\tFollowed by a freqency, tunes that. Blank returns current tuning.");
        Serial.println("   +\t\tSeeks the next station up.");
        Serial.println("   -\t\tSeeks the next station down.");
        Serial.println("   vol\t\tFollowed by 0..15 sets the volume.");
        Serial.println("   rds\t\tReturns the RDS string if possible.");
      break;
      case dispTime : doShowTime();       break;
      case setHour  : doSetHour();        break;
      case setMin   : doSetMin();         break;
      case setSec   : doSetSec();         break;
      case toggle24 : do24();             break;
      case see      : doSee();            break;
      case bright   : doBright();         break;
      case ron       : radio.powerUp();   break;
      case roff      : radio.powerDown(); break;
      case rSetChan  : radioSetChannel(); break;
      case rSeekUp   : radioSeek(true);   break;
      case rSeekDn   : radioSeek(false);  break; 
      case rSetVol   : radioSetVolume();  break;
      case rReadRDS  : radioReadRDS();    break;
      default       : 
        Serial.println("I really don't know what your looking for.");
        Serial.println("Try typing ? for a list of commands.");
      break;
    }
  } 
}


void doShowTime(void) {

   char*    theTimeStr;
   DateTime now;

   now = rtc.now();
   theTimeStr = timeStr(now.hour(),now.minute());
   Serial.println(theTimeStr);
}


void doSetHour(void) {

  byte  newVal;
  char* paramBuff;
  
  if (ourParser.numParams()) {
    paramBuff = ourParser.getParam();
    newVal = atoi (paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
    showTime();
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
    showTime();
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
    showTime();
  }
}


void do24(void) {


   if (params.show24)
      params.show24 = false;
   else
      params.show24 = true;
   Serial.print("24 hour set to ");
   if (params.show24)
      Serial.println("true.");
   else
      Serial.println("false.");
   showTime();
   writeParams();
}


void doSee(void) {

   char* paramBuff;
   int   hour;
   int   minute;
   
   if (ourParser.numParams()==2) {
    paramBuff = ourParser.getParam();
    hour = atoi (paramBuff);
    paramBuff = ourParser.getParam();
    minute = atoi (paramBuff);
    matrixPrint(timeStr(hour,minute),-1);
   }
}

void doBright(void) {

   char* paramBuff;
  
   if (ourParser.numParams()) {
      paramBuff = ourParser.getParam();
      params.brightVal = atoi (paramBuff);
      for (int i=0;i<4;i++) {
         matrix[i].setBrightness(params.brightVal);
      }
      writeParams();
   }
}


void radioSetChannel(void) {
   
   char* paramBuff;
   float freq;
   
   if (ourParser.numParams()==1) {
      paramBuff = ourParser.getParam();
      freq = atof (paramBuff);
      radio.setChannel(freqToInt(freq));    // 4 digit channel number
      params.channel = radio.getChannel();  
      writeParams();
   }
   freq = intToFreq(params.channel);
   Serial.print("Result : ");
   Serial.println(freq,2);
}

   
void radioSeek(bool up) {  

   float freq;
   
   if (up) {
      radio.incChannel();  
   } else {
      radio.decChannel();
   }
   params.channel = radio.getChannel();
   writeParams();
   freq = intToFreq(params.channel);
   Serial.print("Result : ");
   Serial.println(freq,2);
}


void radioSetVolume(void) {

   char* paramBuff;
   int   vol;
   if (ourParser.numParams()==1) {
      paramBuff = ourParser.getParam();
      vol = atoi (paramBuff);
      if (vol>15) vol = 15;
      if (vol<0) vol = 0;
      radio.setVolume(vol);
      params.volume = radio.getVolume();
      writeParams();
   }
   Serial.print("Result : ");
   Serial.println(params.volume);
}


void radioReadRDS(void) {

   if (resizeBuff(20,&returnStr)) {
      //radio.readRDS(returnStr);
      //Serial.println(returnStr);
      Serial.println("not yet");
   }
}


void loop() {
  
  checkParse();
  if (timeChanged()) showTime();
}
