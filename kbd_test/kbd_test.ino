#include <SD.h>
#include <adafruit_1947_Obj.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <drawObj.h>
#include <screen.h>

#include <drawObj.h>
#include <editLabel.h>
#include <bmpKeyboard.h>


#define SD_CS   4

void setup() {
   
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
  }
  screen->fillScreen(&black);

  if (!SD.begin(SD_CS)) {                   // With icons, we now MUST have an SD card.
    Serial.println("NO SD CARD!");
    Serial.flush();
    screen->setCursor(10,10);
    screen->setTextColor(&white);
    screen->setTextSize(2);
    screen->drawText("No SD card.");
    while(true);
  }
  

  rect  aRect(0,0,24,33);
  bmpPipe bmpFile(&aRect);
  bmpFile.openPipe("/system/icons/keyboard/keyCap24.bmp");
  //bmpFile.showPipe();
  bmpFile.drawImage(10,10);
  
  ourEventMgr.begin();                      // Kickstart our event manager.

  editLabel*  textField = new editLabel(10,100,220,12,"hey!",1);
  screen->drawRect(textField,&yellow);
  viewList.addObj(textField);

  bmpKeyboard* aKeyboard = new bmpKeyboard(textField,false);
  aKeyboard->loadKeys();
  viewList.addObj(aKeyboard);
  
  textField->beginEditing();
  
}



void loop() {
  idle();
  

}
