
#include "cellOS.h"

#include <screen.h>
#include <cellCommon.h>
#include <cellManager.h>
#include <colorObj.h>

#include "src/rpnCalc/rpnCalc.h"
#include "src/qGame/qGame.h"
#include "src/phone/phone.h"
#include "src/cellListener/cellListener.h"
#include "contactPanel.h"
#include "src/breakout/breakout.h"
#include "toolsPanel.h"
#include "textPanel.h"

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
// *****************   std buttons  ********************
// *****************************************************


closeBtn::closeBtn(panel* inPanel)
  : iconButton(CLOSE_X,CLOSE_Y,ICON_PATH_X22,22) {
  
  mPanel = inPanel;
  begin();
 }

closeBtn::~closeBtn(void) {  }

void closeBtn::doAction(void) { mPanel->close(); }

// ******

newBtn::newBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_NEW22,22)  { begin(); }
   
newBtn::~newBtn(void) {  }

// *****

searchBtn::searchBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_SEARCH22,22)  { begin(); }
    
searchBtn::~searchBtn(void) {  }

// *****

textBtn::textBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_TEXT22,22)  { begin(); }
    
textBtn::~textBtn(void) {  }

// *****

callBtn::callBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_PHONE22,22)  { begin(); }

   
callBtn::~callBtn(void) {  }

// *****


trashBtn::trashBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_TRASH22,22)  { begin(); }

trashBtn::~trashBtn(void) {  }



// *****************************************************
// ******************   homeScreen  ********************
// *****************************************************

#define SCR_W         240
#define SCR_H         320

#define ICON_BAR_X    0
#define ICON_BAR_Y    280
#define ICON_BAR_W    SCR_W
#define ICON_BAR_H    SCR_H - ICON_BAR_Y

#define HP_ICON_X       3               // Forgot what HP stood for.. (AH! Home Panel)
#define HP_ICON_XSTEP   50
#define HP_ICON_Y       ICON_BAR_Y + 5

#define APP_ICON_X      3
#define APP_ICON_XSTEP  50
#define APP_ICON_Y      30
#define APP_ICON_YSTEP  40


#define BMP_X         0
#define BMP_Y         MENU_BAR_H
#define BMP_W         SCR_W
#define BMP_H         ICON_BAR_Y - MENU_BAR_H

#define BMP_AW        181
#define BMP_AH        256
#define BMP_AX        (BMP_W - BMP_AW) / 2
#define BMP_AY        BMP_Y + (BMP_H - BMP_AH) / 2



// *********** roundedIconBtn ***********

// Our icon buttons have that "rounded corner" look. Well, we have no mask ability so
// we use this to fake it. Basically we'll hard code the corners off the rectangle.


#define COLOR_BUF_SIZE  4   // When grabbing a color off a bitmap you get 3 or 4 bytes. Go big!
 
roundedIconBtn::roundedIconBtn(int xLoc,int yLoc,int message,char* path) 
  : appIcon(xLoc,yLoc,message,path,32) { }

  
roundedIconBtn::~roundedIconBtn(void) {  }


// This is such the hack. We just look at the local x,y value and if its
// a corner? Just drop out of the funtion. And its all hardcoded for a
// 32x32 icon.
void roundedIconBtn::drawPixel(int gx,int gy,colorObj* pixel) {

  int x = gx - xLoc;
  int y = gy - yLoc;
  
  if ((y==0)||(y==31)) {
    if ((x>=0 && x<3)) return;
    if (x>27) return;
  } else if ((y==1)||(y==30)) {
    if ((x>=0 && x<2)) return;
    if (x>28) return;
  } else if ((y==2)||(y==3)||(y==29)||(y==28)) {
    if (x==0) return;
    if (x==31) return;
  }
  screen->drawPixel(gx,gy,pixel);       // Spat it out to the screen.
}


void roundedIconBtn::drawLine(File bmpFile,int x,int y) {

  colorObj  thePixal;
  uint8_t   buf[COLOR_BUF_SIZE];   
  int       trace;
  int       endTrace;
  
  endTrace = x+sourceRect.width;
  for (trace=x;trace<endTrace; trace++) {       // Ok, trace does x..x+width.
    bmpFile.read(buf,pixBytes);                 // Grab a pixel.
    thePixal.setColor(buf[2],buf[1],buf[0]);    // Load colorObj.
    drawPixel(trace,y,&thePixal);               // Try spatting it our to the screen.
  }
}


void roundedIconBtn::drawBitmap(int x,int y) {

  xLoc = x;
  yLoc = y;
  bmpPipe::drawBitmap(x,y);
}

          
// *********** homeScreen ***********


homeScreen::homeScreen(void)
  : homePanel() {

  int   iconX;
  rect  imageRect;

  imageRect.setRect(0,0,BMP_AW,BMP_AH);
  mBackImage = new bmpPipe(imageRect);
  mBackImage->openPipe(IMAGE_FILE_PATH);
  iconX = HP_ICON_X;
  phoneIcon = new appIcon(iconX,HP_ICON_Y,phoneApp,"/system/icons/call.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  textIcon = new appIcon(iconX,HP_ICON_Y,textApp,"/system/icons/text.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  contactIcon = new appIcon(iconX,HP_ICON_Y,contactApp,"/system/icons/addr.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  calcIcon = new appIcon(iconX,HP_ICON_Y,calcApp,"/system/icons/calc.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  toolsIcon = new appIcon(iconX,HP_ICON_Y,toolsApp,"/system/icons/tools.bmp");

  iconX = APP_ICON_X;
  qGameIcon = new roundedIconBtn(iconX,APP_ICON_Y,qGameApp,"/system/icons/qGame.bmp");
  iconX=iconX+APP_ICON_XSTEP;
  breakoutIcon = new roundedIconBtn(iconX,APP_ICON_Y,breakoutApp,"/system/icons/breakout.bmp");
  
  statusTimer.setTime(1500);
}

// Nothing to do. The icons will be automatically dumped and deleted.
homeScreen::~homeScreen(void) { if (mBackImage) delete mBackImage; }


void homeScreen::setup(void) { 

  if (calcIcon)     { addObj(calcIcon);     calcIcon->begin(); }
  if (textIcon)     { addObj(textIcon);     textIcon->begin(); }
  if (contactIcon)  { addObj(contactIcon);  contactIcon->begin(); }
  if (qGameIcon)    { addObj(qGameIcon);    qGameIcon->begin(); }
  if (toolsIcon)    { addObj(toolsIcon);    toolsIcon->begin(); }
  if (breakoutIcon) { addObj(breakoutIcon); breakoutIcon->begin(); }
  if (phoneIcon)    { addObj(phoneIcon);    phoneIcon->begin(); }

  mBatPct = new battPercent(BATT_X,BATT_Y);
  addObj(mBatPct);
  mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
  mRSSI   = new RSSIicon(SIG_X,SIG_Y);
  addObj(mRSSI);
}


void homeScreen::loop(void) { 

  if (statusTimer.ding()) {
    mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
    mRSSI->setRSSI(statusReg.RSSI);
    statusTimer.start();
  }
}


void homeScreen::drawSelf(void) { 
  colorObj aColor(LC_WHITE);
  screen->fillScreen(&aColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
  mBackImage->drawBitmap(BMP_AX,BMP_AY);
  screen->fillRect(ICON_BAR_X,ICON_BAR_Y,ICON_BAR_W,ICON_BAR_H,&black);
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

  pleaseCall = NULL;  // For the phone panel.
  pleaseText = NULL;  // For the text panel.
  
  bringUp();
  return litlOS::begin();
}


void cellOS::initOSFile(void) {  }

// Used to create our custom panels.
panel* cellOS::createPanel(int panelID) {

  switch (panelID) {
    case homeApp      : return new homeScreen(); 
    case phoneApp     : return new phone();
    case textApp      : return new textPanel();
    case contactApp   : return new contactPanel();
    case calcApp      : return new rpnCalc();
    case qGameApp     : return new qGame();
    case toolsApp     : return new toolsPanel();
    case breakoutApp  : return new breakout();
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
