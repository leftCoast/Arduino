
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
textView*   theTextView;
colorRect*  theEditBase;
editField*  theEditField;
SMSmanager* ourKeyboard;

void setup() {
  colorObj aColor;

  Serial.begin(9600);
  for(int i=0;i<100;i++)delay(10);
  //while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  theTextBase = new colorRect(TB_X,TB_Y,TB_W,TB_H,2);
  theTextView = new textView(TF_X,TF_Y,TF_W,TF_H);
  
  theTextView->setText("The North American B-25 Mitchell, a twin-engine bomber that became standard equipment for the Allied air forces in World War II, was perhaps the most versatile aircraft of the war. It became the most heavily armed airplane in the world, was used for high- and low-level bombing, strafing, photoreconnaissance, submarine patrol, and even as a fighter and was distinguished as the aircraft that completed the historic raid over Tokyo in 1942.\nIt required 8,500 original drawings and 195,000 engineering man-hours to produce the first one, but nearly 10,000 were produced from late 1939, when the contract was awarded to North American Aviation, through 1945.\nNamed for famed airpower pioneer Brigadier General William “Billy” Mitchell, it was a twin-tail, mid-wing land monoplane powered by two 1,700-horsepower Wright Cyclone engines.\nNormal bomb capacity was 5,000 pounds (2268 kilograms). Some versions carried 75 mm cannon, machine guns and added firepower of 13 .50-caliber guns in the conventional bombardier's compartment. One version carried eight .50-caliber guns in the nose in an arrangement that provided 14 forward-firing guns.");
  //theTextView->setText("Sometimes\nI see things I don't like to see.\nLike bums on the street, or people trying to get the government to bail them out.");
  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H,2);
  theEditField = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  viewList.addObj(theTextBase);
  viewList.addObj(theTextView);
  theTextView->setTextColors(&black,&white);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  ourKeyboard = new SMSmanager(theEditField,theTextView);
}

  
void loop() { idle(); }
