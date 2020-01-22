#include "cClockOS.h"
#include "cEditPanel.h"
#include <EEPROM.h>

RTC_DS3231  rtc;
mechButton  ourClicker(POT_BTN);
cClockOS    ourOS;
boolean     haveClock = false;
colorObj    colors[24];

// *****************************************************
//                      litleClk
// *****************************************************


litleClk::litleClk(int x,int y) 
  : drawGroup(x,y,45,12) {

 
  mBackColor.setColor(&black);
  
  mText = new label(1,2,45,12);
  mText->setJustify(TEXT_CENTER);
  mText->setColors(&white);
  addObj(mText);
  mShowing = true;
}


litleClk::~litleClk(void) {  }


void litleClk::drawSelf(void) { screen->fillRect(this,&mBackColor); }



void litleClk::setTColor(colorObj* aColor) { mText->setColors(aColor); }


void litleClk::setTime(int hour,int minute) {

  char  timeStr[TEMP_BUFF_SIZE];
  
  snprintf (timeStr,TEMP_BUFF_SIZE,"%d:%d",hour,minute);
  mText->setValue(timeStr);
}


void litleClk::setDisplay(bool trueFalse) {
  
  mShowing = trueFalse;
  if (mShowing) {
    x = x - 128;
    y = y - 128;
  } else {
    x = x + 128;
    y = y + 128;
  }
}

bool litleClk::getDisplay(void) { return mShowing; }



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
//                      clockRect
// *****************************************************



clockRect::clockRect(rect* inRect,colorObj* inColor,int inset)
  : colorRect(inRect,inColor,inset) { mPercent = 50; }

  
clockRect::clockRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset)
  : colorRect(inLocX,inLocY,inWidth,inHeight,inset) { mPercent = 50; }

  
clockRect::~clockRect(void) {  }


void  clockRect::setPercent(float percent) {

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  mPercent = percent;
  setNeedRefresh();
}


void  clockRect::drawSelf(void) {
  
  char text[20];
  int rad;
  mapper radius(0,100,1,181);

  rad = round(radius.Map(mPercent));
  screen->fillScreen(&black);
  screen->fillCircle(64-rad/2,64-rad/2,rad,this);
  /*
  for (int i=0;i<width;i++) {
    for (int j=0;j<height;j++) {
      if ((random(0,10000)/100.0)<mPercent&&(random(0,10000)/100.0)<mPercent) {
        screen->drawPixel(j,i,this);
      } else {
        screen->drawPixel(j,i,&black);
      }
    }
  }
  */
}


  
// *****************************************************
//                      homeClkPanel
// *****************************************************


homeClkPanel::homeClkPanel(void) : homePanel() {

  dimmer    = NULL;
  smoother  = NULL;
  wind      = NULL;
  initPanel = true;
}


homeClkPanel::~homeClkPanel(void) { 
  
  if (dimmer) delete dimmer;
  if (smoother) delete smoother;
  if (wind) delete wind;
}


void homeClkPanel::setup() {

  randomSeed(analogRead(A1));                         // A1 is unwired port for noise.
  
  //kicker.start();
  mBacground = new clockRect(0,0,128,128,&black);
  addObj(mBacground);

  mLittleClk = new litleClk(75,108);
  addObj(mLittleClk);
  mLittleClk->setDisplay(false);
  
  dimmer = new multiMap();
  dimmer->addPoint(0,0);
  dimmer->addPoint(1023,100);
  smoother = new runningAvg(2);
  
  wind = new nWindow(4);
  wind->setBase(-100);                // Given a bogus initial reading it'll update first thing through.
  
  mParser.addCmd(help,"?");
  mParser.addCmd(setHour,"hour");
  mParser.addCmd(setMin,"minute");
  mParser.addCmd(setSec,"sec");
  mParser.addCmd(reset,"reset");
  mParser.addCmd(cEdit,"cedit");
  mParser.addCmd(gColor,"gcolor");
  mParser.addCmd(sColor,"scolor");
}


void homeClkPanel::checkDimmer(void) {

  float     rawData;
  colorObj  aColor;
  
  rawData = smoother->addData(analogRead(A0));    // Toss the reading into the smoother.
  if (!wind->inRange(rawData)) {                  // If the pot value outside the +/- range..
    wind->setBase(rawData);                       // We move the base value to the new reading.
    dimPercent = dimmer->Map(rawData);            // Calculate the new dimming value.            
    mBacground->setPercent(100-dimPercent);
    aColor.setColor(&white);
    aColor.blend(&black,.7 * dimPercent);
    mLittleClk->setTColor(&aColor);
    mLittleClk->setNeedRefresh();
  }
  if (ourClicker.clicked()) {
    while(ourClicker.clicked());
    mLittleClk->setDisplay(!mLittleClk->getDisplay());
    this->setNeedRefresh();
  }
}


void homeClkPanel::checkScreen(void) {
  
  DateTime  now;
  int       index;
  
  if (haveClock) {
    now = rtc.now();
    
    if((now.minute()!=drawtime.minute())||(now.hour()!=drawtime.hour())||initPanel) {  // If times have changed..
      mLittleClk->setTime(now.hour(),now.minute());
      if (now.hour()!=drawtime.hour()||initPanel) {
        index = now.hour();
        if(index>=0&&index<24) {
          mBacground->setColor(&(colors[index]));
        } else {
          mBacground->setColor(&green);
        }
      }
      drawtime = now;
      
    }
  }
  initPanel = false;
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

    Wire.beginTransmission(0x68);   // address DS3231
    Wire.write(0x0E);               // select register
    Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();
  
    now = rtc.now();
    mBacground->setColor(&(colors[now.hour()]));
    setNeedRefresh();
  }
}


void homeClkPanel::doGetColor(void) {

  int   hourVal;
  char* paramBuff;
  
  if (mParser.numParams()==1) {
    paramBuff = mParser.getParam();
    hourVal = atoi (paramBuff);
    free(paramBuff);
  } else {
    hourVal = drawtime.hour();
  }
  if (hourVal>=0&&hourVal<24) {
    Serial.print("Hour ");Serial.print(hourVal);Serial.print(" RGB Value : ");
    Serial.print(colors[hourVal].getRed());
    Serial.print(" ");Serial.print(colors[hourVal].getGreen());
    Serial.print(" ");Serial.println(colors[hourVal].getBlue());
  } else {
    Serial.print("Hour is out of bounds : ");Serial.println(hourVal);
  }
}


void homeClkPanel::doSetColor(void) {

  char* paramBuff;
  int   hourVal;
  int   redVal;
  int   greenVal;
  int   blueVal;
  
  if (mParser.numParams()==4) {
    
    paramBuff = mParser.getParam();
    hourVal = atoi (paramBuff);
    free(paramBuff);
    if (hourVal>=0 && hourVal<24) {
      paramBuff = mParser.getParam();
      redVal = atoi (paramBuff);
      free(paramBuff);
  
      paramBuff = mParser.getParam();
      greenVal = atoi (paramBuff);
      free(paramBuff);
  
      paramBuff = mParser.getParam();
      blueVal = atoi (paramBuff);
      free(paramBuff);
      colors[hourVal].setColor(redVal,greenVal,blueVal);
      ourOS.saveParams();
      if (hourVal==drawtime.hour()) {
        mBacground->setColor(&(colors[hourVal]));
      }
    } else {
      Serial.println("No, values between 0..23 please.");
    }
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
        Serial.println("   gcolor reads back the current hour's color.");
        Serial.println("   scolor followed by 3 numbers R,G,B sets the current hour's color.");
      break;
      case setHour  : doSetHour();    break;
      case setMin   : doSetMin();     break;
      case setSec   : doSetSec();     break;
      case cEdit    : nextPanel = colorEditApp; break;
      case reset    : doReset();      break;
      case gColor   : doGetColor();   break;
      case sColor   : doSetColor();   break;
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

  checkDimmer();
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
  if (haveClock) {                  // If we have the clock, make sure it runs on battery when unplugged.
    Wire.beginTransmission(0x68);   // address DS3231
    Wire.write(0x0E);               // select register
    Wire.write(0b00011100);         // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();
  }
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
