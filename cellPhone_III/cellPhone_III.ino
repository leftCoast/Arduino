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
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include  "colorRect.h"
#include "litlOS.h"
#include "editField.h"
#include "keyboard.h"

#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

//240x320

#define TB_X    10
#define TB_Y    20
#define TB_W    220
#define TB_H    148

#define EB_X    TB_X
#define EB_Y    TB_Y+TB_H+8
#define EB_W    TB_W
#define EB_H    16

#define INSET   3
#define ET_X    EB_X+INSET
#define ET_Y    EB_Y+4
#define ET_W    EB_W-(2*INSET)
#define ET_H    10

colorRect*  theTextBase;
colorRect*  theEditBase;
editField*  theEditField;
keyboard*   ourKeyboard;

void setup() {
  colorObj aColor;

  //Serial.begin(115200);
  //while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  theTextBase = new colorRect(TB_X,TB_Y,TB_W,TB_H,1);
  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H,1);
  theEditField = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  viewList.addObj(theTextBase);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  ourKeyboard = new keyboard(theEditField);
}

  
void loop() {
  idle();

}
