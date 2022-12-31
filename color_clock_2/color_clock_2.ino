#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <neoPixel.h>
#include <lilParser.h>
#include <colorObj.h>
#include <blinker.h>

#define NUM_COLORS  24

#define NUM_LEDS    2
#define LED_PIN     3
#define HOUR_LED    1
#define DECMIN_LED  0


enum        mainComs  { noCommand,
                        help,
                        time,
                        stime,
                        sdate,
                        setHour,
                        setMin,
                        setSec,
                        reset,
                        gColor,
                        sColor,
                        gCode,
                        sOffset
                      };

                      
lilParser   mParser;
RTC_PCF8523 rtc;
DateTime    drawtime;

colorObj    colors[NUM_COLORS];
neoPixel    theNeoPixel(NUM_LEDS,LED_PIN);

blinker  aBlinker;

void setup() {

  Serial.begin(57600);  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }
  
  theNeoPixel.begin();
  theNeoPixel.setPixelColor(0,&black);
  theNeoPixel.setPixelColor(1,&black);
  theNeoPixel.show();

  mParser.addCmd(help,"?");
  mParser.addCmd(time,"time");
  mParser.addCmd(stime,"settime");
  mParser.addCmd(sdate,"setdate");
  mParser.addCmd(setHour,"hour");
  mParser.addCmd(setMin,"minute");
  mParser.addCmd(setSec,"sec");
  mParser.addCmd(reset,"reset");
  mParser.addCmd(gColor,"gcolor");
  mParser.addCmd(sColor,"scolor");
  mParser.addCmd(gCode,"gcode");
  mParser.addCmd(sOffset,"soffset");

  aBlinker.setOnOff(true);
  readParams();
  checkTime(true);
}


// **********************************************************************
// Param handleing
// **********************************************************************


void readParams(void) {

  RGBpack paramBlock[NUM_COLORS];
  
  EEPROM.get(0,paramBlock);
  for (int i=0;i<NUM_COLORS;i++) {
    colors[i].setColor(&(paramBlock[i]));
  }
}


void saveParams(void)  {
  
  RGBpack paramBlock[NUM_COLORS];

  for (int i=0;i<NUM_COLORS;i++) {
    paramBlock[i] = colors[i].packColor();
  }
  EEPROM.put(0,paramBlock);
}



// **********************************************************************
// Checking time
// **********************************************************************


// So, taking a page from Dash's code book.. I decide to use my first multi dimensional array as a hash table.
//
//                      0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 <- hour number / index
//                      B  P  R  O  Y  G  B  P  R  O  Y  G  B  P  R  O  Y  G  B  P  R  O  Y  G  <- hour color

byte hashTable[6][24] { 0, 0, 0, 0, 6, 6, 6, 6, 6, 6, 12,12,12,12,12,12,18,18,18,18,18,18,0, 0,    /* blue     */
                        1, 1, 1, 1, 1, 7, 7, 7, 7, 7, 7, 13,13,13,13,13,13,19,19,19,19,19,19,1,    /* purple   */
                        2, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8, 8, 14,14,14,14,14,14,20,20,20,20,20,20,   /* red      */
                        21,3, 3, 3, 3, 3, 3, 9, 9, 9, 9, 9, 9, 15,15,15,15,15,15,21,21,21,21,21,   /* orange   */
                        22,22,4, 4, 4, 4, 4, 4, 10,10,10,10,10,10,16,16,16,16,16,16,22,22,22,22,   /* yellow   */
                        23,23,23,5, 5, 5, 5, 5, 5, 11,11,11,11,11,11,17,17,17,17,17,17,23,23,23    /* green    */
                      };


void checkTime(bool justDoIt) {
  
  DateTime  now;
  byte      decaMin;
  byte      decaMinIndex;
  
  now = rtc.now();
  if((now.minute()!=drawtime.minute())||(now.hour()!=drawtime.hour())||justDoIt) {  // If times have changed, or need to..
    decaMin = now.minute()/10;
    decaMinIndex = hashTable[decaMin][now.hour()];
    theNeoPixel.setPixelColor(HOUR_LED,&(colors[now.hour()]));
    theNeoPixel.setPixelColor(DECMIN_LED,&(colors[decaMinIndex]));
    theNeoPixel.show();
    drawtime = now;
  }
}



// **********************************************************************
// Command parsing stuff
// **********************************************************************


void doShowTime(void) {

   DateTime now = rtc.now();
   Serial.print("\tTime : ");
   Serial.print(now.hour());Serial.print(":");
   Serial.print(now.minute());Serial.print(":");
   Serial.print(now.second());
   Serial.print("    ");
   Serial.print("Date : ");
   Serial.print(now.month());Serial.print("/");
   Serial.print(now.day());Serial.print("/");
   Serial.print(now.year());
}


void doSetTime() {

   byte  newHour;
   byte  newMin;
   byte  newSec;
   
   if (mParser.numParams()==3) {
      newHour = atoi(mParser.getParam());
      newMin = atoi(mParser.getParam());
      newSec = atoi(mParser.getParam());
      if ((newHour>=0 && newHour< 24)&&(newMin>=0 && newMin< 59)&&(newSec>=0 && newSec< 59)) {
         DateTime now = rtc.now();
         rtc.adjust(DateTime(now.year(),now.month(),now.day(),newHour,newMin,newSec));
         checkTime(true);
      } else {
         Serial.println("Error: One of your entered parameters is out of whack. I'm too lazy to figure out which one though.");
      }
   } else {
      Serial.println("Error: You need to add three values to this command, hour minutes seconds.");
   }
}


void doSetDate() {

   int   newYear;
   byte  newMonth;
   byte  newDay;
   
   if (mParser.numParams()==3) {
      newYear = atoi(mParser.getParam());
      newMonth = atoi(mParser.getParam());
      newDay = atoi(mParser.getParam());
      if (newMonth>0&&newMonth<=12) {
         DateTime now = rtc.now();
         rtc.adjust(DateTime(newYear,newMonth,newDay,now.hour(),now.minute(),now.second()));
      } else {
         Serial.print("Error: C'mon! What kind of month number is ");
         Serial.print(newMonth);
         Serial.println(" ?");
      }
   } else {
      Serial.println("Error: You need to enter year month day, three values, to set the date.");
   }
}


void doSetHour() {

   byte  newVal;
  
   if (mParser.numParams()) {
      newVal = atoi(mParser.getParam());
      if (newVal>=0&&newVal<24) {
         DateTime now = rtc.now();
         rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
         checkTime(true);
      } else {
         Serial.println("Error: You need to enter an hour vlaue from 0..23 to set an hour.");
      }
   } else {
      Serial.println("Error: You also need to tell me what hour to set the clock..");
   }
}


void doSetMin() {

  byte  newVal;
  
   if (mParser.numParams()) {
      newVal = atoi(mParser.getParam());
      if (newVal>=0&&newVal<60) {
         DateTime now = rtc.now();
         rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newVal,now.second()));
         checkTime(true);
      } else {
         Serial.println("Error: Minutes go from 0..59.");
      }
   } else {
      Serial.println("Error: You forgot to tell me what minute to set this to.");
   }
}


void doSetSec() {
  
   byte  newVal;
  
   if (mParser.numParams()) {
      newVal = atoi(mParser.getParam());
      if (newVal>=0&&newVal<60) {
         DateTime now = rtc.now();
         rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),newVal));
         checkTime(true);
      } else {
         Serial.println("Error: Seconds go from 0..59.");
      }
   } else {
      Serial.println("Error: You forgot to tell me what second to set this to.");
   }
}


void doReset() {

    /*
    Tycho's numbers.
    colors[0].setColor(0,0,3);
    colors[1].setColor(1,0,2);
    colors[2].setColor(2,0,0);
    colors[3].setColor(2,1,0);
    colors[4].setColor(1,1,0);
    colors[5].setColor(0,1,0);
    colors[6].setColor(0,0,5);
    colors[7].setColor(2,0,4);
    colors[8].setColor(4,0,0);
    colors[9].setColor(8,4,0);
    colors[10].setColor(10,10,0);
    colors[11].setColor(0,10,0);
    colors[12].setColor(0,0,20);
    colors[13].setColor(9,0,18);
    colors[14].setColor(10,0,0);
    colors[15].setColor(10,5,0);
    colors[16].setColor(8,8,0);
    colors[17].setColor(0,5,0);
    colors[18].setColor(0,0,10);
    colors[19].setColor(3,0,6);
    colors[20].setColor(3,0,0);
    colors[21].setColor(2,1,0);
    colors[22].setColor(1,1,0);
    colors[23].setColor(0,1,0);
    */
   colors[0].setColor(0,0,255);
   colors[1].setColor(140,0,255);
   colors[2].setColor(255,0,0);
   colors[3].setColor(255,100,0);
   colors[4].setColor(255,255,0);
   colors[5].setColor(0,255,0);
   colors[6].setColor(0,0,255);
   colors[7].setColor(140,0,255);
   colors[8].setColor(255,0,0);
   colors[9].setColor(255,100,0);
   colors[10].setColor(255,255,0);
   colors[11].setColor(0,255,0);
   colors[12].setColor(0,0,255);
   colors[13].setColor(140,0,255);
   colors[14].setColor(255,0,0);
   colors[15].setColor(255,100,0);
   colors[16].setColor(255,255,0);
   colors[17].setColor(0,255,0);
   colors[18].setColor(0,0,255);
   colors[19].setColor(140,0,255);
   colors[20].setColor(255,0,0);
   colors[21].setColor(255,100,0);
   colors[22].setColor(255,255,0);
   colors[23].setColor(0,255,0);
   saveParams();
   checkTime(true);
}


void doGetHourColor(void) {

  int   hourVal;
  
  if (mParser.numParams()==1) {
    hourVal = atoi(mParser.getParam());
  } else {
    hourVal = drawtime.hour();
  }
  if (hourVal>=0&&hourVal<24) {
    Serial.print("Hour ");Serial.print(hourVal);Serial.print(" RGB Value : ");
    Serial.print(colors[hourVal].getRed());
    Serial.print(" ");Serial.print(colors[hourVal].getGreen());
    Serial.print(" ");Serial.println(colors[hourVal].getBlue());
  } else {
    Serial.print("Error: Hour number is out of bounds : ");Serial.println(hourVal);
  }
}


void doSetHourColor(void) {

   int   hourVal;
   int   redVal;
   int   greenVal;
   int   blueVal;
  
   if (mParser.numParams()==4) {
      hourVal = atoi(mParser.getParam());
      if (hourVal>=0 && hourVal<24) {
         redVal = atoi(mParser.getParam());
         greenVal = atoi(mParser.getParam());
         blueVal = atoi(mParser.getParam());
         colors[hourVal].setColor(redVal,greenVal,blueVal);
         saveParams();
         checkTime(true);
      } else {
         Serial.println("Error: Only values between 0..23 please.");
      }
   } else {
      Serial.println("Error: This comand is looking for four values. hour red green blue. (colors got fro 0..255)");
  }
}


void doGetColorList(void) {

   Serial.println();
   Serial.println("// ******** Auto generated code listing ********");
   Serial.println();
   for (byte i=0;i<NUM_COLORS;i++) {
      Serial.print("\tcolors[");
      Serial.print(i);
      Serial.print("].setColor(");
      Serial.print(colors[i].getRed());
      Serial.print(",");
      Serial.print(colors[i].getGreen());
      Serial.print(",");
      Serial.print(colors[i].getBlue());
      Serial.println(");");
   }
   Serial.println();
   Serial.println("// ********         End listing         ********");
}


void doSetOffset() {

   int  mode;
   int  newVal;
  
   if (mParser.numParams()==2) {
      mode = atoi(mParser.getParam());
      newVal = atoi(mParser.getParam());
      if (newVal>=-64 && newVal<64) {
         switch(mode) {
            case 0 : 
               Serial.print("Offset set to ");
               Serial.print(newVal);
               Serial.println(" updated every two hours.");
               rtc.calibrate(PCF8523_OneMinute, newVal);
            break;
            case 1 : 
               Serial.print("Offset set to ");
               Serial.print(newVal);
               Serial.println(" updated once a minute.");
               rtc.calibrate(PCF8523_TwoHours, newVal); break;
            default  : 
               Serial.print("Error: mode needs to be 0 or 1. Not ");
               Serial.print(mode);
               Serial.println(".");
            break;
         }
      } else {
         Serial.print("Error: Offset needs to be between -64 & 63, inclusive. Not ");
         Serial.print(newVal);
         Serial.println(".");
      }
   } else {
      Serial.println("Error: You need two params mode and offset.");
   }
}


void checkParse(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand      : break;
      case help           : 
        Serial.println("Commands:");
        Serial.println("   time prints the currnet time.");
        Serial.println("   settime followed by 3 numbers hour, minute, second, sets the curnent time.");
        Serial.println("   setdate followed by 3 numbers year, month, day, sets the curnent date.");
        Serial.println("   hour followed by a number 0..23. Sets the hour.");
        Serial.println("   minute followed by a number 0..59. Sets the minutes.");
        Serial.println("   sec followed by a number 0..59. Sets the seconds.");
        Serial.println("   reset will reset all colors to defaults.");
        Serial.println("   gcolor reads back the current hour's color.");
        Serial.println("   scolor followed by 4 numbers hour,R,G,B sets that hour's color.");
        Serial.println("   gcode will print a listing of your color settings to be used as the new defualt color list.");
        Serial.println("   soffset followed by mode 0..1 and offset -64..63 will set the calibration offset. Your chip is #PCF8523 see : https://www.nxp.com/docs/en/application-note/AN11247.pdf");
        Serial.println();
      break;
      case time     : doShowTime();   break;
      case stime    : doSetTime();    break;
      case sdate    : doSetDate();    break;
      case setHour  : doSetHour();    break;
      case setMin   : doSetMin();     break;
      case setSec   : doSetSec();     break;
      case reset    : doReset();      break;
      case gColor   : doGetHourColor();   break;
      case sColor   : doSetHourColor();   break;
      case gCode    : doGetColorList();   break;
      case sOffset  : doSetOffset();      break;
      default       : 
        Serial.println("I really don't know what your looking for.");
        Serial.println("Try typing ? for a list of commands.");
        Serial.println();
      break;
    }
  } 
}



// **********************************************************************
// Main loop()
// **********************************************************************


void loop() {     // During loop..

   idle();
   checkTime(false);
   checkParse();
}
