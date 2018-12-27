  
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <quickCom.h>

#include <colorRect.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include <cellCommon.h>

#include "litlOS.h"
#include "editField.h"
#include "textView.h"
#include "keyboard.h"
#include "SMSmanager.h"
#include "icons.h"


#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.
#define SD_CS   4

#define TB_X    10
#define TB_Y    20
#define TB_W    220
#define TB_H    148

#define TF_INSET  3
#define TF_X      TB_X+TF_INSET
#define TF_Y      TB_Y+TF_INSET
#define TF_W      TB_W-(2*TF_INSET)
#define TF_H      TB_H-(2*TF_INSET)

#define EB_X    TB_X
#define EB_Y    TB_Y+TB_H+8
#define EB_W    TB_W
#define EB_H    16

#define ET_INSET  3
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+5
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define BATT_X    200
#define BATT_Y    3

#define out  theTextView->appendText

colorRect*    theTextBase;        // White rect behind the text indow.
textView*     theTextView;        // The text window.
colorRect*    theEditBase;        // White rect behind the text edit field.
editField*    theEditField;       // The text edit field.
SMSmanager*   ourKeyboard;        // Just like it sounds, our texting keyboard.
battPercent*  battIcon;           // Icon thing showing charge state of the battery.

qCMaster      ourComObj;          // Object used to comunicate with the FONA controller.

timeObj       statusTimer(10000);  // We'll check status every 2 1/2 seconds.

cellStatus    statusReg;

enum comStates { offline, standby, comSent };
comStates comState;


enum  danceCards { noOne, statusBoy };
danceCards  currCard;


void setup() {
  
  setupScreen();                        // Fire up the screen. If it fails it'll lock the process.
  comState = offline;                   // Not online yet.
  ourComObj.begin(9600);                // Fire up comunications.
  if (ourComObj.readErr()==NO_ERR) {    // Did the poor thing fire up?
    comState = standby;                 // Communications standing by!
    out("FONA Coms, online.\n");
  } else {
    out("FONA Coms, FAIL.\n");
  }
  if (SD.begin(SD_CS)) {
    out("File system ONLINE.\n");
    
  } else {
    out("File system FAIL.\n");
  }
  currCard = noOne;                     // Free to dance with any and all.
}


void loop() {
  
  idle();

  if (statusTimer.ding()&&comState==standby) {
    if (currCard == noOne) {
      //out("Giving card to status boy.\n");
      currCard = statusBoy;
      statusTimer.start();
    }
  }
  switch (currCard) {
    case noOne      :                           break;
    case statusBoy  : doStatus();               break;
    //case SMSTalker  : ourKeyboard->sendSMS();   break;
  }
}

 

// **************************************************************
// ************************** handlers **************************
// **************************************************************


void setupScreen(void) {

  colorObj aColor;
  
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  
  theTextBase   = new colorRect(TB_X,TB_Y,TB_W,TB_H,2);
  theTextView   = new textView(TF_X,TF_Y,TF_W,TF_H);
  theEditBase   = new colorRect(EB_X,EB_Y,EB_W,EB_H,2);
  theEditField  = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  battIcon      = new battPercent(BATT_X,BATT_Y);
  
  
  viewList.addObj(theTextBase);
  viewList.addObj(theTextView);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  viewList.addObj(battIcon);

  ourKeyboard   = new SMSmanager(theEditField,theTextView);
  theTextView->setTextColors(&black,&white);
  battIcon->setPercent(0);
}


void doStatus(void) {

  byte  ourCom;
  byte  numBytes;

  ourCom = getStatus;
  switch(comState) {
    case offline    : currCard = noOne; break;
    case standby    : 
      ourComObj.readErr();                          // Clear out old errors.
      ourComObj.sendBuff(&ourCom,1,true);           // Attempt a send.
      if (ourComObj.readErr()) {                    // Trouble?
        comState = offline;                         // Its busted!
        currCard = noOne;                           // Toss the card.
      } else {
        comState = comSent;                         // We set it.
      }
      break;
    case comSent    :                               // The command has been sent, check for reply.
      if (ourComObj.readErr()) {                    // Anything go wrong in there?
        out("had error.\n");
        comState = offline;                         // Its broken!
        currCard = noOne;                           // toss out now usless dance card.
      } else {
        numBytes = ourComObj.haveBuff();            // If its there, it'll give back its size.
        if (numBytes>0) {                           // We got byte count?
          if (numBytes==sizeof(cellStatus)) {       // We got the right byte count?
            ourComObj.readBuff((byte*)&statusReg);  // Stuff the data into the struct.
            updateInfo();                           // Deal with having new data.
          } else {                                  // Wrong number of bytes?
            ourComObj.dumpBuff();                   // Who knows what it is, get rid of it.
          }
          comState = standby;                       // Well we're done with it.
          currCard = noOne;                         // realease the machine.
        }
      }
      break;
    default : break;
  }
}

     
void updateInfo(void) {

  battIcon->setPercent((byte)statusReg.batteryPercent);

  
  /*
  /out("Setting battery voltage : ");out(statusReg.batteryVolts);out("mV\n");
  out("Setting battery percent : ");out(statusReg.batteryPercent);out("%\n");
  out("RSSI                    : ");out(statusReg.RSSI);out("\n");
  out("Net Stat : ");
  switch (statusReg.networkStat) {
    case 0 : out("Not registered"); break;
    case 1 : out("Reg. (home)"); break;
    case 2 : out("Reg. (searching)"); break;
    case 3 : out("Denied"); break;
    case 4 : out("Unknown"); break;
    case 5 : out("Reg. (roaming)"); break;
    default : out("Undefined"); break;
  }
  out("\n");
  out("volume                  : ");out(statusReg.volume);out("\n");
  out("callStat                : ");out(statusReg.callStat);out("\n");
  out("numSMSs                 : ");out(statusReg.numSMSs);out("\n");
  out("networkTime             : ");out(statusReg.networkTime);out("\n");
  */

}

bool  sendComNBuff(byte aCom,byte* aBuff,byte numBytes,bool reply) {

  byte* outBuff;

  outBuff = (byte*)malloc(numBytes+1);              // Clear out some space.
  if (outBuff) {                                    // If we got the space.
    outBuff[0] = aCom;                              // Char 0 is the command.
    for(byte i=0;i<numBytes;i++) {                  // Loop to fill in the buffer part.
      outBuff[i+1] = aBuff[i];
    }
    ourComObj.readErr();                            // Clear out old errors.
    ourComObj.sendBuff(outBuff,numBytes+1,reply);   // Attempt a send.
    free(outBuff);                                  // Recycle the buffer.
    return ourComObj.readErr()==NO_ERR;              // No error means we were able to send it out.
  }
  return false;                                     // If we got here something went wrong.
}


void  doSendSMS(void) {

/*
  byte  ourCom;
  byte  numBytes;

  ourCom = sendSNS;
  switch(comState) {
    case offline    : currCard = noOne; break;
    case standby    :
      out("Me - ");                         // Reformat the output. Starting with, Me -
      out(mOutBuff);                        // And the output sting.
      if (sendComNBuff(sendSNS,ourKeyboard->mOutBuff,ourKeyboard->mNumBytes,true)) { // Send ok?
        comState = comSent;                         // We set it.
      } else {
        comState = offline;                         // Its busted!
        currCard = noOne;                           // Toss the card.
      }
    break;
    case comSent    :                               // The command has been sent,
  }
  mComObj.sendBuff(&ourCom,1,true);           // Attempt a send.
  if (!fona.sendSMS("14083400352", mOutBuff)) {
    out(" - Failed\n"));
  } else {
    out(" ->\n");
  }
  currCard = noOne;
  */
}
