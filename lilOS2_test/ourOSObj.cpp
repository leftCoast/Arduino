#include "ourOSObj.h"
#include <rpnCalc.h>
#include <breakout.h>
#include <iconEdit.h>
//#include <starTrek.h>
#include "testApp.h"
#include "homeScr.h"
//#include <debug.h>


#define BEEP_PIN     23                      // The digital pin choosen for the beeper.
#define SCREEN_PIN   25                      // The ananlog pin choosen for the screen backlight.
char systemFolder[]  = "/system/";           // Where we decided to store the systemp folder on our SD card.
char panelFolder[]   = "/system/appFiles/";  // Where we decided to store the app folders on our SD card.


// **************************************
// ************** ourOSObj **************
// **************************************


ourOSObj::ourOSObj(void)
   : lilOS() { }


ourOSObj::~ourOSObj(void) {  }


// The hardware is online, do hookups.
int ourOSObj::begin(void) {
   
   pinMode(BEEP_PIN, OUTPUT);       // Setup The beeper pin.
   digitalWrite(BEEP_PIN, HIGH);    // Means off.
   return lilOS::begin();           // Return result of the inherited
}

//void backlightOn(void) { ourOSPtr->setBrightness(255); }

// We need to write our own panel creation method.
panel* ourOSObj::createPanel(int panelID) {
   
   panel* result;
   
   beep();
   //setBrightness(0);
   switch (panelID) {
      case homeApp      : result = new homeScr();                    break;
      case calcApp      : result = new rpnCalc(this,panelID);        break;
      case iconEditApp : result = new iconEdit(this,panelID);       break;
      case breakoutApp  : result = new breakout(this,panelID);       break;
      //case starTrekApp  : result = new starTrekPanel(this,panelID);  break;
      case testAppID      : result = new testApp(this,panelID);  break;
      default           : result = NULL;
   }
   return(result);
}


// Only WE know how to make it beep.
void ourOSObj::beep(void) { tone(BEEP_PIN, 750,20); }


// Here's the pin if you want to use it yourself.
int ourOSObj::getTonePin(void) {return BEEP_PIN; }


// And how to control the screen brightness.
void ourOSObj::setBrightness(byte brightness) { analogWrite(SCREEN_PIN,brightness); }


char*  ourOSObj::getSystemFolder(void) { return systemFolder; }


// Hand this an appID and get back a pointer to the path of its data folder.
char* ourOSObj::getPanelFolder(int panelID) {
    int numBytes;

   numBytes = strlen(panelFolder) + 10;  // 8 for name, 1 for '/', 1 for '\0'.
   if (resizeBuff(numBytes,&pathBuff)) {
      strcpy(pathBuff,panelFolder);
      switch (panelID) {
         case homeApp      : return NULL;
         case calcApp      : 
            strcat(pathBuff,"rpnCalc/");
            return pathBuff;
         break;
         case iconEditApp  :
            strcat(pathBuff,"iconEdit/");
            return pathBuff;
         break;
         case breakoutApp  :
            strcat(pathBuff,"breakout/");
            return pathBuff;
         break;
         case testAppID  :
            strcat(pathBuff,"testApp/");
            return pathBuff;
         break;
         default           : return NULL;
      }
   }
}
