
#include "cellOS.h"

#include <resizeBuff.h>
#include <screen.h>
#include <cellCommon.h>
#include <cellManager.h>
#include <colorObj.h>
#include <debug.h>

#include "src/rpnCalc/rpnCalc.h"
#include "src/qGame/qGame.h"
#include "src/phone/phone.h"
#include "src/cellListener/cellListener.h"
#include "src/contacts/contactPanel.h"
#include "src/breakout/breakout.h"
#include "toolsPanel.h"
#include "textPanel.h"

#define RAMPUP_START  0
#define RAMPUP_END    1500
#define RAMPDN_START  RAMPUP_END
#define RAMPDN_END    RAMPDN_START + 100

#define CELL_EITEM_INSET  3

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
//colorObj  menuBarColor(LC_RED);

cellOS  ourOS;



// *****************************************************
//                      cellOSPanel
// *****************************************************


cellOSPanel::cellOSPanel(int panelID,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents)
  : panel(panelID,menuBarChoice,inEventSet) {
  
  if (mMenuBar) {
    statusIcon* aStatus = new statusIcon();
    mMenuBar->addObj(aStatus);
  } 
}

  
cellOSPanel::~cellOSPanel(void) {  }



// *****************************************************
//                      formatPN
// *****************************************************

void formatPN(label* numField);

void formatPN(label* numField) {
 
  PNLabel formatter(1,1,1,1,1);
  int     numBytes;
  char*   textBuff;
  
  textBuff = NULL;
  numBytes = numField->getNumChars() + 1;
  if (resizeBuff(numBytes,(uint8_t**)&textBuff)) {
    numField->getText(textBuff);
    formatter.setValue(textBuff);
    resizeBuff(0,(uint8_t**)&textBuff);
    numBytes = formatter.getNumChars() + 1;
    if (resizeBuff(numBytes,(uint8_t**)&textBuff)) {
      formatter.getText(textBuff);
      numField->setValue(textBuff);
      resizeBuff(0,(uint8_t**)&textBuff);
    }
  }
}



contactsBtn::contactsBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_CONTACT22,22)  { begin(); }
  
contactsBtn::~contactsBtn(void) { }

void contactsBtn::doAction(void) { nextPanel = contactApp; }

// *****

textBtn::textBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_TEXT22,22)  { begin(); }
    
textBtn::~textBtn(void) {  }

// *****

callBtn::callBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_PHONE22,22)  { begin(); }

   
callBtn::~callBtn(void) {  }



// *****************************************************
// ******************   statusIcon  ********************
// *****************************************************


statusIcon::statusIcon(void)
  : drawGroup(STATUS_X,STATUS_Y,STATUS_W,STATUS_H) {
    
  mBatPct = new battPercent(0,3);
  addObj(mBatPct);
  mRSSI = new RSSIicon(15,3);
  addObj(mRSSI);
  hookup();
  setStatus();
  setTime(1500);
}


statusIcon::~statusIcon(void) {  }


void statusIcon::setStatus(void) {

  mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
  mRSSI->setRSSI(statusReg.RSSI);
}


void statusIcon::idle(void) {

  if (ding()) {
    setStatus();
    start();
  }
  drawGroup::idle();
}

void statusIcon::drawSelf(void) { }



/*
// *****************************************************
// ******************  cellEditField  ******************
// *****************************************************


cellEditField::cellEditField (rect* inRect,char* inText,keyboard* inKeyboard)
	: drawGroup(inRect,fullClick) {
  
	rect  tRect;
	
	mKeyboard = inKeyboard;
	mEditBase = new colorRect(inRect,&backColor);
	addObj(mEditBase);
	tRect = *inRect;
	tRect.x = 0;
	tRect.y = 0;
	tRect.insetRect(CELL_EITEM_INSET);
	mEditField = new editLabel(&tRect,inText,1);
	mEditField->setColors(&textColor,&backColor);	
	addObj(mEditField);
	setEventSet(fullClick);
}


cellEditField::~cellEditField(void) {  }


void cellEditField::drawSelf(void) { screen->drawRect(this,&blue); }
 

//
// If we get focus, we pass it on to the edit field. When the edit field
// looses focus, it will tell us by calling this this method. SO we don't
// "unset" the edit field's focus, we only set it.
void cellEditField::setFocus(bool setLoose) {
  
  if (setLoose) {
    mEditField->setColors(&textSelectColor,&editFieldBColor);
    mEditBase->setColor(&editFieldBColor);
    if (mKeyboard) {
      mKeyboard->setEditField(mEditField);
    }
    currentFocus = mEditField;      // We manually switch focus to mEditField..
    mEditField->setFocus(true); 		// mEditField will call this with "false" when it looses focus.
  } else {
    mEditField->setColors(&textColor,&backColor);
    mEditBase->setColor(&backColor);
    if (mKeyboard) {
      if (mKeyboard->mEditObj==mEditField) {
        mKeyboard->setEditField(NULL);
      }
    }
  }
}
//


void cellEditField::setFocus(bool setLoose) {
  
	if (setLoose) {
		mEditField->setColors(&textSelectColor,&editFieldBColor);
		mEditBase->setColor(&editFieldBColor);
		mKeyboard->setEditField(mEditField);
		mEditField->beginEditing();
	} else {
		mEditField->setColors(&textColor,&backColor);
    	mEditBase->setColor(&backColor);
      mKeyboard->setEditField(NULL);
	}
}


void cellEditField::doAction(void) { setFocusPtr(this); }


// Not including the \0. You may need to add one.
int cellEditField::getNumChars(void) { return mEditField->getNumChars(); }


void cellEditField::formatAsPN(void) { formatPN(mEditField); }


// You better have added the (1) for the \0.
void cellEditField::getText(char* inBuff) { mEditField->getText(inBuff); }


void  cellEditField::setText(char* inText) { mEditField->setValue(inText); }
*/

cellEditField::cellEditField(rect* inRect,char* inText,keyboard* inKeyboard) 
  : datafield(inRect->x,inRect->y,inRect->width,inRect->height) { 

  editLabel*  editField;
  rect        aRect;

  aRect = *inRect;
  aRect.x = 0;
  aRect.y = 0;
  mEditBase = new colorRect(&aRect,&backColor);
  aRect.insetRect(CELL_EITEM_INSET);
  editField = new editLabel(&aRect,inText,1);
  editField->setExitOnEnter(true);
  editField->setColors(&textColor,&backColor); 
  begin(inKeyboard,editField,mEditBase);
}


cellEditField::~cellEditField(void) { }

  
void cellEditField::drawSelf(void) { screen->drawRect(this,&blue); }


void cellEditField::setThisFocus(bool setLoose) { 

  datafield::setThisFocus(setLoose);
  if (setLoose) {
    mEditField->setColors(&textSelectColor,&editFieldBColor);
    mEditBase->setColor(&editFieldBColor);
  } else {
    mEditField->setColors(&textColor,&backColor);
    mEditBase->setColor(&backColor);
  }
}


void cellEditField::formatAsPN(void) { formatPN(mEditField); }


// Not counting the traqiling null. '\0'
int cellEditField::getNumChars(void) { return mEditField->getNumChars(); }


void cellEditField::getText(char* buff) { return mEditField->getText(buff); }

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


void roundedIconBtn::drawImage(int x,int y) {

  xLoc = x;
  yLoc = y;
  bmpPipe::drawImage(x,y);
}

          
// *********** homeScreen ***********


homeScreen::homeScreen(void)
  : homePanel() {

  int   iconX;
  rect  imageRect;

  imageRect.setRect(0,0,BMP_AW,BMP_AH);
  mBackImage = new bmpPipe(&imageRect);
  mBackImage->openPipe(IMAGE_FILE_PATH);
  iconX = HP_ICON_X;

  phoneIcon = new appIcon(iconX,HP_ICON_Y,phoneApp,"/system/icons/cPhone/phone32.bmp");
  if (phoneIcon) phoneIcon->begin();
  
  iconX=iconX+HP_ICON_XSTEP;
  textIcon = new appIcon(iconX,HP_ICON_Y,textApp,"/system/icons/cPhone/msg32.bmp");
  if (textIcon) textIcon->begin();
  
  iconX=iconX+HP_ICON_XSTEP;
  contactIcon = new appIcon(iconX,HP_ICON_Y,contactApp,"/system/icons/cPhone/cont32.bmp");
  if (contactIcon) contactIcon->begin();
  
  iconX=iconX+HP_ICON_XSTEP;
  calcIcon = new appIcon(iconX,HP_ICON_Y,calcApp,"/system/icons/rpnCalc/calc32.bmp");
  if (calcIcon) calcIcon->begin();
  
  iconX=iconX+HP_ICON_XSTEP;
  toolsIcon = new appIcon(iconX,HP_ICON_Y,toolsApp,"/system/icons/standard/pref32.bmp");
  if (toolsIcon) toolsIcon->begin();
  
  iconX = APP_ICON_X;
  qGameIcon = new roundedIconBtn(iconX,APP_ICON_Y,qGameApp,"/system/icons/qGame/qGame.bmp");
  if (qGameIcon) qGameIcon->begin();
  
  iconX=iconX+APP_ICON_XSTEP;
  breakoutIcon = new roundedIconBtn(iconX,APP_ICON_Y,breakoutApp,"/system/icons/breakout/breakout.bmp");
  if (breakoutIcon) breakoutIcon->begin();
  
}

// Nothing to do. The icons will be automatically dumped and deleted.
homeScreen::~homeScreen(void) { if (mBackImage) delete mBackImage; }


void homeScreen::setup(void) { 

  if (calcIcon)     { addObj(calcIcon); }
  if (textIcon)     { addObj(textIcon); }
  if (contactIcon)  { addObj(contactIcon); }
  if (qGameIcon)    { addObj(qGameIcon); }
  if (toolsIcon)    { addObj(toolsIcon); }
  if (breakoutIcon) { addObj(breakoutIcon); }
  if (phoneIcon)    { addObj(phoneIcon); }
  statusIcon* anIcon = new statusIcon();
  mMenuBar->addObj(anIcon);
}


void homeScreen::loop(void) {  }


void homeScreen::drawSelf(void) { 
  screen->fillScreen(&white);
  mBackImage->drawImage(BMP_AX,BMP_AY);
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

  battLineColor.blend(&white,30);
  menuBarColor.setColor(LC_RED);
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

  pleaseCall      = NULL;  // For the phone panel.
  currentContact  = NULL;  // For the text panel.
  
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
return;
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
