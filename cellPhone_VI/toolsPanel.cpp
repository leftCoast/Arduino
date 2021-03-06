#include "cellOS.h"
#include "toolsPanel.h"
#include <cellCommon.h>
#include <cellManager.h>

#define CLOSE_X         0
#define CLOSE_Y         1

#define BATT_X        199
#define BATT_Y        2

#define SIG_X         BATT_X + 15
#define SIG_Y         BATT_Y

#define out     mText->appendText
#define outln   mText->appendText("\n")


int statID = -1;

// *****************************************************
// ******************  toolsPanel  *******************
// *****************************************************


toolsPanel::toolsPanel(void)
  : cellOSPanel(toolsApp) { }


toolsPanel::~toolsPanel(void) { }


void toolsPanel::setup(void) {

  colorObj aColor(LC_NAVY);
  mText = new textView(10,30,220,150);
  mText->setTextColors(&white,&aColor);
  addObj(mText);
  statusTimer.setTime(1500); 
}


void  toolsPanel::showStatus(void) {

  if (statID!=statusReg.statNum) {
    statID = statusReg.statNum;
    mText->setText("");
    outln;
    out("           cellStatus");outln;outln;
    out(" FONA online: ");out(statusReg.FONAOnline);outln;
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
    out(" Error byte : ");out(statusReg.errByte);out("\n");
    out(" Caller ID  : ");out(statusReg.callerID);out("\n");
    out(" Stat No.   : ");out(statusReg.statNum);out("\n");
    out(" In queue   : ");out(ourCellManager.getCount());out("\n");
    out(" Read time  : ");out(statusMS);out("ms\n");
    /*
    out("DEBUGGING\n");
    out("Raw SMS    : [");out(SMSRaw);out("]\n");
    out("SMS PN     : [");out(SMSPN);out("]\n");
    out("SMS Msg    : [");out(SMSMsg);out("]\n");
    */
  }
}


void toolsPanel::loop(void) { 

  if (statusTimer.ding()) {
    showStatus();
    statusTimer.start();
  }
}


void toolsPanel::drawSelf(void) {

  screen->fillScreen(&backColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


void toolsPanel::closing(void) { }
