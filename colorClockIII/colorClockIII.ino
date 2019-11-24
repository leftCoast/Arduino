#include "Wire.h"
#include "RTClib.h"

#include "adafruit_1431_Obj.h"
#include "screen.h"
#include "mechButton.h"
#include "fontLabel.h"
#include "runningAvg.h"
#include "lilParser.h"


#define OLED_CS       10
#define OLED_RST      6
#define LABEL_OFFSET  1
#define PERC_WINDOW   3
#define POT_BTN       4

enum        mainComs { noCommand, help, setHour, setMin, setSec, see };
lilParser   mainParser;
colorObj    colors[12];
fontLabel   num(0, 70, 80, 108);
fontLabel   shade(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
label       timeDisp(75,110,128-75,12);
mapper      dimmer(0,1024,0,100);
runningAvg  smoother(3);
int         dimPercent;
DateTime    drawtime;
RTC_DS3231  rtc;
mechButton  ourClicker(POT_BTN);
bool        showTime;


void setup() {
  
  rtc.begin();
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
  if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    while (1);
  }
  
  screen->fillScreen(&cyan);
  colors[0].setColor(LC_RED);
  colors[1].setColor(LC_ORANGE);
  colors[2].setColor(LC_YELLOW);
  colors[3].setColor(LC_GREEN);
  colors[4].setColor(LC_BLUE);
  colors[5].setColor(LC_PURPLE);
  colors[6].setColor(LC_PINK);
  colors[7].setColor(LC_ORANGE);
  colors[8].setColor(LC_YELLOW);
  colors[9].setColor(LC_GREEN);
  colors[10].setColor(LC_LIGHT_BLUE);
  colors[11].setColor(LC_LAVENDER);

  num.setTextSize(4);
  num.setJustify(TEXT_RIGHT);
  num.setColors(&white);
  viewList.addObj(&num);
  
  shade.setTextSize(4);
  shade.setJustify(TEXT_RIGHT);
  shade.setColors(&black);
  viewList.addObj(&shade);
  viewList.addObj(&timeDisp);
  showTime = false;
  drawClock();

  mainParser.addCmd(help,"?");
  mainParser.addCmd(setHour,"hour");
  mainParser.addCmd(setMin,"minute");
  mainParser.addCmd(setSec,"sec");
  mainParser.addCmd(see,"see");
}


void  drawClock(void) {
  
  int index;
  colorObj  aColor;
  char  timeStr[10];
  
  DateTime now = rtc.now();
  index = now.hour();
  if (index>12) {
    index = index - 12;
  }
  index--;
  aColor.setColor(&(colors[index]));
  aColor.blend(&black,dimPercent);
  screen->fillScreen(&aColor);
  num.setValue(now.hour());
  shade.setValue(now.hour());
  
  if (showTime) {
    snprintf (timeStr,TEMP_BUFF_SIZE,"%d:%d",now.hour(),now.minute());
    timeDisp.setValue(timeStr);
  } else {
    timeDisp.setValue(""); 
  }
  drawtime = now;
}


void checkScreen(void) {
  
  float rawData;
  int   newPercent;
  bool  needDraw;

  needDraw = false;
  DateTime now = rtc.now();
  if (now.hour()!=drawtime.hour()) {
    needDraw = true;
  }
  rawData = smoother.addData(analogRead(A0));
  newPercent = round(dimmer.Map(rawData));
  if (newPercent>dimPercent+PERC_WINDOW ||newPercent<dimPercent-PERC_WINDOW) {
    dimPercent = newPercent;
    needDraw = true;
  }

  if (ourClicker.clicked()) {
    while(ourClicker.clicked());
    showTime = ! showTime;
    needDraw = true;
  }

  if (showTime && now.minute()!=drawtime.minute()) {
    needDraw = true;
  }

  if (needDraw) drawClock();
}


void doSetHour() {

  byte  newVal;
  char* paramBuff;
  
  if (mainParser.numParams()) {
    paramBuff = mainParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
  }
}


void doSetMin() {

  byte  newVal;
  char* paramBuff;
  
  if (mainParser.numParams()) {
    paramBuff = mainParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newVal,now.second()));
  }
}


void doSetSec() {
  
  byte  newVal;
  char* paramBuff;
  
  if (mainParser.numParams()) {
    paramBuff = mainParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),newVal));
  }
}


void doSee() {

  
}

  
void checkParse(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mainParser.addChar(inChar);
    switch (command) {                       
      case noCommand      : break;
      case help           : 
        Serial.println("Commands:");
        Serial.println("   hour followed by a number 0..23.");
        Serial.println("   minute followed by a number 0..59.");
        Serial.println("   sec followed by a number 0..59.");
        Serial.println("   see followed by a number 1..12.");
      break;
      case setHour : doSetHour(); break;
      case setMin : doSetMin(); break;
      case setSec : doSetSec(); break;
      case see    : doSee(); break;
      default     : 
        Serial.println("I really don't know what your looking for.");
        Serial.println("Try typing ? for a list of commands.");
      break;
    }
  } 
}



void loop(void) {

  char inChar;
  
  idle();
  checkScreen();
  checkParse();
}
