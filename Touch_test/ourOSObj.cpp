#include "ourOSObj.h"
#include <rpnCalc.h>
#include <breakout.h>
#include <iconEdit.h>
#include "homeScr.h"

//For Mega
#define BEEP_PIN     14                      // The digital pin choosen for the beeper.
#define SCREEN_PIN   A0                      // The ananlog pin choosen for the screen backlight.
char systemFolder[]  = "/system/";           // Where we decided to store the systemp folder on our SD card.
char panelFolder[]   = "/system/appFiles/";  // Where we decided to store the app folders on our SD card.

// Our OS object.
ourOSObj  ourOS;

bmpMask  iconMask;

// **************************************
// ************** ourOSObj **************
// **************************************


ourOSObj::ourOSObj(void)
   : lilOS() { }


ourOSObj::~ourOSObj(void) {  }


// The hardware is online, do hookups.
int ourOSObj::begin(void) {
   
   pinMode(BEEP_PIN, OUTPUT);       // Setup The beeper pin.
   digitalWrite(BEEP_PIN, HIGH);    //Means off.
   setBrightness(255);
   iconMask.readFromBMP("/system/icons/standard/mask32.bmp");
   return lilOS::begin();          // Return result of the inherited
}


// We need to write our own panel creation method.
panel* ourOSObj::createPanel(int panelID) {
   
   beep();
   //setBrightness(0);
   switch (panelID) {
      case homeApp      : return new homeScr();
      case calcApp      : return new rpnCalc(this,panelID);
      case breakoutApp  : return new breakout(this,panelID);
      default           : return NULL;
   }
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
     
   strcpy(pathBuff,panelFolder);
   switch (panelID) {
      case homeApp      : return NULL;
      case calcApp      : 
         strcat(pathBuff,"rpnCalc/");
         return pathBuff;
      break;
      case breakoutApp  :
         strcat(pathBuff,"breakout/");
         return pathBuff;
      break; 
      default           : return NULL;
   }
}
