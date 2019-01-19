
#include "cellOS.h"

#include <screen.h>
#include <cellCommon.h>
#include <cellManager.h>
#include <colorObj.h>

#include "src/rpnCalc/rpnCalc.h"
#include "src/qGame/qGame.h"
#include "phone.h"
#include "cellListener.h"
#include "contactPanel.h"

#define RAMPUP_START  0
#define RAMPUP_END    1500
#define RAMPDN_START  RAMPUP_END
#define RAMPDN_END    RAMPDN_START + 100

#define STEPTIME  20 //ms

// Starting points, they are tweaked in setup().
colorObj  backColor(LC_NAVY);
colorObj  textColor(LC_WHITE);
colorObj  textSelectColor(LC_YELLOW);
colorObj  textActiveColor(LC_RED);
colorObj  editFieldBColor(LC_NAVY);

colorObj  lightbButtonColor(LC_NAVY);
colorObj  lightbButtonHighlight(LC_NAVY);
colorObj  lightbButtonColorHit(LC_NAVY);
colorObj  darkButtonColor(LC_NAVY);
colorObj  darkButtonColorHit(LC_NAVY);

colorObj  redButtonColor(LC_RED);
colorObj  redButtonHighlight(LC_PINK);
colorObj  greenbuttonColor(LC_DARK_GREEN);
colorObj  greenButtonHighlight(LC_GREEN);

colorObj  battLineColor(LC_CHARCOAL);
colorObj  menuBarColor(LC_RED);

cellOS  ourOS;


// *****************************************************
// ******************   homeScreen  ********************
// *****************************************************

#define HP_ICON_X       3
#define HP_ICON_XSTEP  40
#define HP_ICON_Y     275

#define ICON_BAR_X    0
#define ICON_BAR_Y    270
#define ICON_BAR_W    240
#define ICON_BAR_H    90

#define BATT_X        199
#define BATT_Y        2

#define SIG_X         BATT_X + 15
#define SIG_Y         BATT_Y


//#define out     mText->appendText
//#define outln   mText->appendText("\n")

homeScreen::homeScreen(void)
  : homePanel() {

  int   iconX;
  rect  imageRect;

  imageRect.setRect(0,MENU_BAR_H,240,ICON_BAR_Y-MENU_BAR_H);
  mBackImage = new bmpPipe(imageRect);
  mBackImage->openPipe(IMAGE_FILE_PATH);
  iconX = HP_ICON_X;
  phoneIcon = new appIcon(iconX,HP_ICON_Y,phoneApp,"/system/icons/phone.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  textIcon = new appIcon(iconX,HP_ICON_Y,textApp,"/system/icons/text.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  contactIcon = new appIcon(iconX,HP_ICON_Y,contactApp,"/system/icons/contacts.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  calcIcon = new appIcon(iconX,HP_ICON_Y,calcApp,"/system/icons/calc.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  qGameIcon = new appIcon(iconX,HP_ICON_Y,qGameApp,"/system/icons/qGame.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  breakoutIcon = new appIcon(iconX,HP_ICON_Y,breakoutApp,"/system/icons/breakout.bmp");
  
  //colorObj aColor(LC_NAVY);
  //mText = new textView(10,10,220,150);
  //mText->setTextColors(&white,&aColor);
  //addObj(mText);
  statusTimer.setTime(1500);
}

// Nothing to do. The icons will be automatically dumped and deleted.
homeScreen::~homeScreen(void) { if (mBackImage) delete mBackImage; }


void homeScreen::setup(void) { 

  if (calcIcon)     { addObj(calcIcon);     calcIcon->begin(); }
  if (textIcon)     { addObj(textIcon);     textIcon->begin(); }
  if (contactIcon)  { addObj(contactIcon);  contactIcon->begin(); }
  if (qGameIcon)    { addObj(qGameIcon);    qGameIcon->begin(); }
  if (breakoutIcon) { addObj(breakoutIcon); breakoutIcon->begin(); }
  if (phoneIcon)    { addObj(phoneIcon);    phoneIcon->begin(); }

  mBatPct = new battPercent(BATT_X,BATT_Y);
  addObj(mBatPct);
  mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
  mRSSI   = new RSSIicon(SIG_X,SIG_Y);
  addObj(mRSSI);
}


void  homeScreen::showStatus(void) {
  /*
  mText->setText("");
  outln;
  out("           cellStatus");outln;outln;
  out(" Battery V  : ");out(statusReg.batteryVolts);out("mV\n");
  out(" Battery %  : ");out(statusReg.batteryPercent);out("%\n");
  out(" RSSI       : ");out(statusReg.RSSI);outln;
  out(" Net Stat   : ");
  switch (statusReg.networkStat) {
    case 0 : out("Not registered"); break;
    case 1 : out("Reg. (home)"); break;
    case 2 : out("Reg. (searching)"); break;
    case 3 : out("Denied"); break;
    case 4 : out("Unknown"); break;
    case 5 : out("Reg. (roaming)"); break;
    default : out("Undefined"); break;
  }
  outln;  
  out(" Volume     : ");out(statusReg.volume);out("\n");
  out(" CallState  : ");
  switch (statusReg.callStat) {
    case 0  : out("Ready"); break;
    case 1  : out("No Status"); break;
    case 2  : out("Unknown"); break;
    case 3  : out("Ringing In"); break;
    case 4  : out("Ringing Out"); break;
    default : out("Unknown II"); break;
  }
  outln;
  out(" Num SMSs   : ");out(statusReg.numSMSs);out("\n");
  out(" Net Time   : ");out(statusReg.networkTime);out("\n");
  */
}


void homeScreen::loop(void) { 

  if (statusTimer.ding()) {
    mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
    mRSSI->setRSSI(statusReg.RSSI);
    //showStatus();
    statusTimer.start();
  }
}


void homeScreen::drawSelf(void) { 
  //colorObj aColor(LC_NAVY);
  //screen->fillScreen(&aColor);

  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
  //unsigned long startT = millis();
  mBackImage->drawBitmap(0,MENU_BAR_H);
  //unsigned long endT = millis();
  //Serial.println(endT-startT);
  
  screen->fillRect(ICON_BAR_X,ICON_BAR_Y,ICON_BAR_W,ICON_BAR_H,&white);
}



// *****************************************************
// *********************   cellOS   ********************
// *****************************************************

 
cellOS::cellOS(void) {

  mFile = NULL;
  mDimScreen = false;
  mNowTime = 0;
  mEndTime = 0;
}


cellOS::~cellOS(void) { if (mFile) delete mFile; }


int cellOS::begin(void) {

  // Tweak our colors.
  editFieldBColor.blend(&yellow,15  );
  editFieldBColor.blend(&white,2);
  
  lightbButtonColor.blend(&white,20);
  lightbButtonColorHit.blend(&white,90);
  lightbButtonHighlight = lightbButtonColorHit;
  darkButtonColor.blend(&white,10);
  darkButtonColorHit.blend(&white,80);

  //redbuttonColor;
  redButtonHighlight.blend(&white,50);
  greenbuttonColor.blend(&green,50);
  greenButtonHighlight.blend(&white,90);

  //battLineColor = lightbButtonColor;
  menuBarColor.blend(&black,95);
  
  // Fill in the keyboard pallette.
  kbPallette.inputKeyText.setColor(&textSelectColor);
  kbPallette.inputKeyBase.setColor(&lightbButtonColor);
  kbPallette.inputKeyHText.setColor(&white);
  kbPallette.inputKeyHBase.setColor(&lightbButtonColorHit);
    
  kbPallette.contolKeyText.setColor(&white);
  kbPallette.contolKeyBase.setColor(&darkButtonColor);
  kbPallette.contolKeyHText.setColor(&textActiveColor);
  kbPallette.contolKeyHBase.setColor(&darkButtonColorHit);
    
  kbPallette.deleteKeyText.setColor(&textActiveColor);
  kbPallette.deleteKeyBase.setColor(&darkButtonColor);
  kbPallette.deleteKeyHText.setColor(&white);
  kbPallette.deleteKeyHBase.setColor(&darkButtonColorHit);

  mFile = new blockFile(SYS_FILE_PATH);
  if (mFile->isEmpty()) {
    initOSFile();
  }
  mScreenTimer.setTime(STEPTIME);
  screenMap.addPoint(RAMPUP_START,0);
  screenMap.addPoint(RAMPUP_END,255);
  screenMap.addPoint(RAMPDN_START,255);
  screenMap.addPoint(RAMPDN_END,0);
  ourListener.begin(phoneApp);
  bringUp();
  return litlOS::begin();
}


void cellOS::initOSFile(void) {  }

// Used to create our custom panels.
panel* cellOS::createPanel(int panelID) {

  switch (panelID) {
    case homeApp      : return new homeScreen(); 
    case phoneApp     : return new phone();
    case textApp      : nextPanel = HOME_PANEL_ID; return new homeScreen();
    case contactApp   : return new contactPanel();
    case calcApp      : return new rpnCalc();
    case qGameApp     : return new qGame();
    case breakoutApp  : nextPanel = HOME_PANEL_ID; return new homeScreen();
    default           : return NULL;
  }
}

void cellOS::launchPanel(void) {

  hideRedraw();
  litlOS::launchPanel();
}


void cellOS::hideRedraw() {

  for(int i=RAMPDN_START;i<RAMPDN_END;i=i+STEPTIME) {
    analogWrite(SCREEN_PIN,screenMap.Map(i));
    delay(STEPTIME);
  }
  analogWrite(SCREEN_PIN,0);
  mDimScreen = true;
  mNowTime = RAMPUP_START;
  mEndTime = RAMPUP_END;
  mScreenTimer.start();
}

void cellOS::bringUp() {
  
  mDimScreen = true;
  mNowTime = RAMPUP_START;
  mEndTime = RAMPUP_END;
  mScreenTimer.start();
}


// Things we do..
void cellOS::idle(void) { 

  if (mDimScreen) {
    if (mScreenTimer.ding()) {
      analogWrite(SCREEN_PIN,screenMap.Map(mNowTime));
      mNowTime = mNowTime + STEPTIME;
      mDimScreen = mNowTime < mEndTime;
    }
    if (!mDimScreen) analogWrite(SCREEN_PIN,255);
  }
}
