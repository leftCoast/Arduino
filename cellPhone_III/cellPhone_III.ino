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

#include "litlOS.h"
#include "labelButton.h"
#include "editField.h"


#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

#define KEY_WD 23
#define KEY_HT 23
#define KEY_SP  1
#define COL_1 1
#define COL_2 COL_1 + KEY_WD + KEY_SP
#define COL_3 COL_2 + KEY_WD + KEY_SP
#define COL_4 COL_3 + KEY_WD + KEY_SP
#define COL_5 COL_4 + KEY_WD + KEY_SP
#define COL_6 COL_5 + KEY_WD + KEY_SP
#define COL_7 COL_6 + KEY_WD + KEY_SP
#define COL_8 COL_7 + KEY_WD + KEY_SP
#define COL_9 COL_8 + KEY_WD + KEY_SP
#define COL_10 COL_9 + KEY_WD + KEY_SP

#define ROW_1 240

editField hello("Hello world, Nigel");
labelButton qKey("q",COL_1,ROW_1,KEY_WD,KEY_HT);
labelButton wKey("w",COL_2,ROW_1,KEY_WD,KEY_HT);
labelButton eKey("e",COL_3,ROW_1,KEY_WD,KEY_HT);
labelButton rKey("r",COL_4,ROW_1,KEY_WD,KEY_HT);
labelButton tKey("t",COL_5,ROW_1,KEY_WD,KEY_HT);
labelButton yKey("y",COL_6,ROW_1,KEY_WD,KEY_HT);
labelButton uKey("u",COL_7,ROW_1,KEY_WD,KEY_HT);
labelButton iKey("i",COL_8,ROW_1,KEY_WD,KEY_HT);
labelButton oKey("o",COL_9,ROW_1,KEY_WD,KEY_HT);
labelButton pKey("p",COL_10,ROW_1,KEY_WD,KEY_HT);


void setup() {
  colorObj aColor;

  Serial.begin(115200);
  while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  viewList.addObj(&hello);

  viewList.addObj(&qKey);
  viewList.addObj(&wKey);
  viewList.addObj(&eKey);
  viewList.addObj(&rKey);
  viewList.addObj(&tKey);
  viewList.addObj(&yKey);
  viewList.addObj(&uKey);
  viewList.addObj(&iKey);
  viewList.addObj(&oKey);
  viewList.addObj(&pKey);
}

  
void loop() {
  idle();

}
