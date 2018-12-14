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

colorRect*  theTextBase;
textView*   theTextField;
colorRect*  theEditBase;
editField*  theEditField;
SMSmanager* ourKeyboard;

void setup() {
  colorObj aColor;

  Serial.begin(9600);
  delay(10);
  while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  theTextBase = new colorRect(TB_X,TB_Y,TB_W,TB_H,2);
  theTextBase->setColor(LC_CYAN);
  theTextField = new textView(TF_X,TF_Y,TF_W,TF_H);
  theTextField->setText("Well here's a big ass bunch of text. Lets see if it will actually wrap or not? I'm so bloody tired of writing this code that's just a bit beyond what I can inderstand.");
  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H,2);
  theEditField = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  viewList.addObj(theTextBase);
  viewList.addObj(theTextField);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  ourKeyboard = new SMSmanager(theEditField,theTextField);
}

  
void loop() { idle(); }
