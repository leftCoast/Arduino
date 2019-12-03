#include "cClockOS.h"



RTC_DS3231  rtc;
mechButton  ourClicker(POT_BTN);
cClockOS    ourOS;
boolean     haveClock = false;

// *****************************************************
//                      randCRect
// *****************************************************



randCRect::randCRect(rect* inRect,colorObj* inColor,int inset)
  : colorRect(inRect,inColor,inset) { mPercent = 50; }

  
randCRect::randCRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset)
  : colorRect(inLocX,inLocY,inWidth,inHeight,inset) { mPercent = 50; }

  
randCRect::~randCRect(void) {  }


void  randCRect::setPercent(int percent) {

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  mPercent = percent;
  setNeedRefresh();
}


void  randCRect::drawSelf(void) {
  
  for (int i=0;i<width;i++) {
    for (int j=0;j<height;j++) {
      if (random(0,99)<mPercent) {
        screen->drawPixel(i,j,this);
      } else {
        screen->drawPixel(i,j,&black);
      }
    }
  }
}



  
// *****************************************************
//                      homeClkPanel
// *****************************************************


homeClkPanel::homeClkPanel(void) : homePanel() {  }


homeClkPanel::~homeClkPanel(void) {  }


void homeClkPanel::setup() {

  mBacground = new randCRect(0,0,128,128,&green);
  addObj(mBacground);
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

  num = new fontLabel(0, 70, 80, 108);
  num->setTextSize(4);
  num->setJustify(TEXT_RIGHT);
  num->setColors(&white);
  addObj(num);

  shade = new fontLabel(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
  shade->setTextSize(4);
  shade->setJustify(TEXT_RIGHT);
  shade->setColors(&black);
  addObj(shade);

  dimmer = new mapper(3,1020,0,100);

  smoother = new runningAvg(3);
  
  mTBackground = new colorRect(73,107,128-75,13);
  mTBackground->setColor(&black);
  addObj(mTBackground);
  //mTBackground->setNeedRefresh(false);
  
  timeDisp = new label(75,110,128-75,12);
  timeDisp->setJustify(TEXT_CENTER);
  timeDisp->setColors(&white);
  addObj(timeDisp);
  //timeDisp->setNeedRefresh(false);
  
  showTime = false;

  mParser.addCmd(help,"?");
  mParser.addCmd(setHour,"hour");
  mParser.addCmd(setMin,"minute");
  mParser.addCmd(setSec,"sec");
  mParser.addCmd(see,"see");
}


void homeClkPanel::drawClock(void) {
  
  int       index;
  colorObj  aColor;
  char      timeStr[10];
  
  DateTime  now;
  if (haveClock) now = rtc.now();
  index = now.hour();
  if (index>12) {
    index = index - 12;
  }
  index--;
  if(index>=0&&index<12) {
    aColor.setColor(&(colors[index]));
  } else {
    aColor.setColor(&black);
  }
  mBacground->setColor(&aColor);
  mBacground->setPercent(100-dimPercent);
  num->setValue(now.hour());
  aColor.setColor(&white);
  aColor.blend(&black,.7 * dimPercent);
  num->setColors(&aColor);
  shade->setValue(now.hour());
  
  aColor.setColor(&white);
  aColor.blend(&black,.7 * dimPercent);
  timeDisp->setColors(&aColor);
  
  if (showTime) {
    snprintf (timeStr,TEMP_BUFF_SIZE,"%d:%d",now.hour(),now.minute());
    timeDisp->setValue(timeStr);
    mTBackground->setNeedRefresh();
  } else {
    timeDisp->setValue(""); 
    mTBackground->setNeedRefresh(false);
  }
  drawtime = now;
}


void homeClkPanel::checkScreen(void) {
  
  float     rawData;
  int       newPercent;
  bool      needDraw;
  DateTime  now;
    
  needDraw = false;
  if (haveClock) now = rtc.now();
  
  if (now.hour()>23) {                  // A quick hack for when there is no clock to read.
    DateTime newHour(now.year(),now.month(),now.day(),23,now.minute(),now.second());
    now = newHour;
  }
  
  if (now.hour()!=drawtime.hour()) {
    needDraw = true;
  }
  rawData = smoother->addData(analogRead(A0));
  randomSeed(analogRead(A0));
  newPercent = round(dimmer->Map(rawData));
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


void homeClkPanel::doSetHour() {

  byte  newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),newVal,now.minute(),now.second()));
  }
}


void homeClkPanel::doSetMin() {

  byte  newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),newVal,now.second()));
  }
}


void homeClkPanel::doSetSec() {
  
  byte  newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(),now.month(),now.day(),now.hour(),now.minute(),newVal));
  }
}


void homeClkPanel::doSee() {

  
}

  
void homeClkPanel::checkParse(void) {

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


// Nothing to do. All the bits take care of themselves.
void homeClkPanel::drawSelf(void) {  }


void homeClkPanel::loop(void) {

  checkScreen();
  checkParse();
}



// *****************************************************
//                       cClockOS   
// *****************************************************


cClockOS::cClockOS(void) : litlOS() { }


cClockOS::~cClockOS(void) {  }


cClockOS::begin(void) {

  haveClock = rtc.begin();
  Wire.beginTransmission(0x68);   // address DS3231
  Wire.write(0x0E);               // select register
  Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
  litlOS::begin();
}


panel* cClockOS::createPanel(int panelID) {

  switch (panelID) {
    case homeApp      : return new homeClkPanel(); 
    case colorEditApp : return new homeClkPanel();//editPanel();
    default           : return new homeClkPanel();
  }
}
