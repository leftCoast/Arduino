  
#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_SSD1351.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <drawObj.h>
#include <scrKeyboard.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <lineObj.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <quickCom.h>
#include <baseGraphics.h>
#include <displayObj.h>
#include <bmpPipe.h>
#include <screen.h>
#include <litlOS.h>
#include <cellCommon.h>
#include <eventMgr.h>


#include "icons.h"
#include "cellOS.h"
#include "cellManager.h"
#include "contactPanel.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4



void setup() {

  analogWrite(SCREEN_PIN,0);    // Turn off backlight.
  
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  // Next is to bring the comuication hardware online.
  ourCellManager.begin(9600);             // Fire up comunications.
  if (ourCellManager.readErr()!=NO_ERR) { // Did the poor thing fire up?
    while(true);
  }
  
  if (!SD.begin(SD_CS)) {     // Bring the diskdrive online.
    while(true);
  }
  
  addrStarter kicker;                       // Once the disk is online we can..
  kicker.begin(CONTACT_FILE_PATH,false);    // Boot our little black book. false = normal. true? Delete and reset the phonebook.
                                            // kicker should auto-destruct at the end of setup(). As it should.
                                            
  ourEventMgr.begin();                      // Kickstart our event manager.
                                            // Hardware and services are up and running.
  ourOS.begin();                            // Boot OS manager. 
}


void loop() {     // During loop..
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on the the current panel.
}
