  
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <colorRect.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>


#include "litlOS.h"
#include "editField.h"
#include "textView.h"
#include "keyboard.h"
#include "SMSmanager.h"
#include "icons.h"
#include "quickCom.h"

#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

//240x320

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

colorRect*    theTextBase;
textView*     theTextView;
colorRect*    theEditBase;
editField*    theEditField;
SMSmanager*   ourKeyboard;
battPercent*  battIcon;
qCMaster      ourComObj;

char    billy[] = {"“Billy”"};
byte    inBuff[256]; 
int     inIdex = 0;
timeObj batTimer(20000);
bool    waitBatt;



void setup() {
  colorObj aColor;

  ourComObj.begin(9600);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  //Serial.begin(9600);
  //Serial.println("hello from X teensy!");
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  theTextBase = new colorRect(TB_X,TB_Y,TB_W,TB_H,2);
  theTextView = new textView(TF_X,TF_Y,TF_W,TF_H);
  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H,2);
  theEditField = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);

  battIcon = new battPercent(200,3);
  
  viewList.addObj(theTextBase);
  viewList.addObj(theTextView);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  viewList.addObj(battIcon);

  theTextView->setTextColors(&black,&white);
  battIcon->setPercent(50);
  
  ourKeyboard = new SMSmanager(theEditField,theTextView,&ourComObj);
 
  waitBatt = false;
  batTimer.start();
  theTextView->appendText("");
  battIcon->setPercent(100);
}

void  checkErr(char* weDid) {
  
  byte error = ourComObj.readErr();

  theTextView->appendText("Tried to ");
  theTextView->appendText(weDid);
  theTextView->appendText(" and ");
  if (error) {
    theTextView->appendText("it Failed");
  } else {
    theTextView->appendText("it Seems ok.");
  }
  theTextView->appendText("\n");
}


void loop() {

  byte  bytes;
  byte  battCom = 1;
  
  idle();
  if (batTimer.ding()) {
    checkErr("ding");
    if (ourComObj.sendBuff(&battCom,1,true)) {
      checkErr("Send a buffer");
      waitBatt = true;
      batTimer.start();
    }
  }
  if (waitBatt) {
      bytes = ourComObj.haveBuff();
      
    if (bytes>0) {
      ourComObj.readBuff(inBuff);
      checkErr("read reply buffer.");
      //Serial.print("setting battery percet to : ");Serial.println((byte)inBuff[0]);
      //Serial.println((byte)inBuff[0]);
      char  temp[20];
      snprintf (temp,20,"%d",(int)inBuff[0]);
      theTextView->appendText("setting battery % = ");theTextView->appendText(temp);theTextView->appendText("\n");
      battIcon->setPercent((byte)inBuff[0]);
      waitBatt = false;
    } else {
      //theTextView->appendText((char*)inBuff);
    }
  }
}
