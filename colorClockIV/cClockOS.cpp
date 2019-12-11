#include "cClockOS.h"
#include "cEditPanel.h"
#include <EEPROM.h>

RTC_DS3231  rtc;
mechButton  ourClicker(POT_BTN);
cClockOS    ourOS;
boolean     haveClock = false;
colorObj    colors[24];



// *****************************************************
//                      nWindow
// *****************************************************


nWindow::nWindow(float range, float baseVal) {
  
  mRange = range;
  setBase(baseVal);
}


nWindow::~nWindow(void) {  }


void nWindow::setBase(float baseVal) {

    mMin = baseVal - mRange;
    mMax = baseVal + mRange;
}


bool nWindow::inRange(float val) { return (val>=mMin && val<=mMax); }



// *****************************************************
//                      randCRect
// *****************************************************



randCRect::randCRect(rect* inRect,colorObj* inColor,int inset)
  : colorRect(inRect,inColor,inset) { mPercent = 50; }

  
randCRect::randCRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset)
  : colorRect(inLocX,inLocY,inWidth,inHeight,inset) { mPercent = 50; }

  
randCRect::~randCRect(void) {  }


void  randCRect::setPercent(float percent) {

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  mPercent = percent;
  setNeedRefresh();
}


void  randCRect::drawSelf(void) {
  
  for (int i=0;i<width;i++) {
    for (int j=0;j<height;j++) {
      if ((random(0,10000)/100.0)<mPercent&&(random(0,10000)/100.0)<mPercent) {
        screen->drawPixel(j,i,this);
      } else {
        screen->drawPixel(j,i,&black);
      }
    }
  }
}


  
// *****************************************************
//                      homeClkPanel
// *****************************************************


homeClkPanel::homeClkPanel(void) : homePanel() {

  dimmer    = NULL;
  smoother  = NULL;
  wind      = NULL;
}


homeClkPanel::~homeClkPanel(void) { 
  
  if (dimmer) delete dimmer;
  if (smoother) delete smoother;
  if (wind) delete wind;
}


void homeClkPanel::setup() {

  randomSeed(analogRead(A1));                         // A1 is unwired port for noise.
  wind = new nWindow(2);
  wind->setBase(analogRead(A0));
  
  mBacground = new randCRect(0,0,128,128,&black);
  addObj(mBacground);
  
  shade = new fontLabel(0+LABEL_OFFSET, 70+LABEL_OFFSET, 80, 108);
  shade->setTextSize(4);
  shade->setJustify(TEXT_RIGHT);
  shade->setColors(&black);
  addObj(shade);
  
  dimmer = new multiMap();
  dimmer->addPoint(0,0);
  dimmer->addPoint(510,1);
  dimmer->addPoint(1020,100);
  
  smoother = new runningAvg(4);
  
  mTBackground = new colorRect(100+73,107,128-75,13);
  mTBackground->setColor(&black);
  addObj(mTBackground);
  
  timeDisp = new label(100+75,110,128-75,12);
  timeDisp->setJustify(TEXT_CENTER);
  timeDisp->setColors(&white);
  addObj(timeDisp);

  showTime = false;
  
  drawClock();

  mParser.addCmd(help,"?");
  mParser.addCmd(setHour,"hour");
  mParser.addCmd(setMin,"minute");
  mParser.addCmd(setSec,"sec");
  mParser.addCmd(reset,"reset");
  mParser.addCmd(cEdit,"cedit");
}


void homeClkPanel::drawClock(void) {
  
  int       index;
  colorObj  aColor;
  char      timeStr[10];
  
  DateTime  now;
  if (haveClock) now = rtc.now();
  index = now.hour();
  if(index>=0&&index<24) {
    aColor.setColor(&(colors[index]));
  } else {
    aColor.setColor(&black);
  }
  mBacground->setColor(&aColor);
  mBacground->setPercent(100-dimPercent);
  shade->setValue(now.hour());
  
  aColor.setColor(&white);
  aColor.blend(&black,.7 * dimPercent);
  timeDisp->setColors(&aColor);
  
  if (showTime) {
    snprintf (timeStr,TEMP_BUFF_SIZE,"%d:%d",now.hour(),now.minute());
    timeDisp->setValue(timeStr);
    mTBackground->setNeedRefresh();
  }
  drawtime = now;
}


void homeClkPanel::checkScreen(void) {
  
  float     rawData;
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
  rawData = smoother->addData(analogRead(A0));    // Toss the reading into the smoother.
  if (!wind->inRange(rawData)) {                  // If the pot value outside the +/- range..
    wind->setBase(rawData);                       // We move the base value to the new reading.
    dimPercent = dimmer->Map(rawData);            // Calculate the new dimming value.            
    needDraw = true;                              // We need a redraw.
  }

  if (ourClicker.clicked()) {
    while(ourClicker.clicked());
    showTime = !showTime;
    if (showTime) {
      timeDisp->x = timeDisp->x-100;
      mTBackground->x = mTBackground->x-100;
    } else {
      timeDisp->x = timeDisp->x+100;
      mTBackground->x = mTBackground->x+100;
    }
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


void homeClkPanel::doReset() {

  DateTime  now;

  colors[0].setColor(LC_RED);
  colors[1].setColor(LC_ORANGE);
  colors[2].setColor(LC_YELLOW);
  colors[3].setColor(LC_GREEN);
  colors[4].setColor(LC_BLUE);
  colors[5].setColor(100,100,0);
  colors[6].setColor(LC_RED);
  colors[7].setColor(LC_ORANGE);
  colors[8].setColor(LC_YELLOW);
  colors[9].setColor(LC_GREEN);
  colors[10].setColor(LC_BLUE);
  colors[11].setColor(100,100,0);   // Brown.
  for(int i=0;i<12;i++) {
    colors[12+i].setColor(&(colors[i]));
  }
  ourOS.saveParams();
   
  if (haveClock) {
    now = rtc.now();
    mBacground->setColor(&(colors[now.hour()]));
    setNeedRefresh();
  }
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
        Serial.println("   hour followed by a number 0..23. Sets the hour.");
        Serial.println("   minute followed by a number 0..59. Sets the minutes.");
        Serial.println("   sec followed by a number 0..59. Sets the seconds.");
        Serial.println("   cedit takes you to the color editor.");
        Serial.println("   reset will reset all colors to defaults.");
      break;
      case setHour  : doSetHour(); break;
      case setMin   : doSetMin(); break;
      case setSec   : doSetSec(); break;
      case cEdit    : nextPanel = colorEditApp; break;
      case reset    : doReset();  break;
      default       : 
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


int cClockOS::begin(void) {

  haveClock = rtc.begin();
  Wire.beginTransmission(0x68);   // address DS3231
  Wire.write(0x0E);               // select register
  Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();

  readParams();
  
  return litlOS::begin();
}

void cClockOS::readParams(void) {

  RGBpack paramBlock[24];
  
  EEPROM.get(0,paramBlock);
  for (int i=0;i<24;i++) {
    colors[i].setColor(&(paramBlock[i]));
  }
}


void cClockOS::saveParams(void)  {
  
  RGBpack paramBlock[24];

  for (int i=0;i<24;i++) {
    paramBlock[i] = colors[i].packColor();
  }
  EEPROM.put(0,paramBlock);
}


panel* cClockOS::createPanel(int panelID) {

  switch (panelID) {
    case homeApp      : return new homeClkPanel(); 
    case colorEditApp : return new cEditPanel();
    default           : return new homeClkPanel();
  }
}
