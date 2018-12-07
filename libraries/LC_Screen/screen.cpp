
#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "adafruit_1947_Obj.h"
#include <LC_SPI.h>
//#include "SSD_13XX_Obj.h"

#include <SD.h>

displayObj* screen = NULL;

boolean initScreen(byte hardware, byte csPin, byte inRotation) { 
	return initScreen(hardware,csPin, -1,inRotation);
}
	
	
boolean initScreen(byte hardware,byte csPin,byte rsPin,byte inRotation) {

  switch (hardware) {
    case ADAFRUIT_1431 :
      screen = (displayObj*) new adafruit_1431_Obj(csPin,rsPin);
      if (screen) {
      	if (screen->dispObjBegin()) {
        		screen->setRotation(inRotation);
            return true;
      	}
      }
      return false;
      break;
      
    case ADAFRUIT_1947 :
      screen = (displayObj*) new adafruit_1947_Obj(csPin,rsPin);
      if (screen) {
        if (screen->dispObjBegin()) {
            screen->setRotation(inRotation);
            return true;
        }
      }
      return false;
      break;
   
   /* this one caused wiring issues and his colors were messed up 
    case SUMO_TOY_SSD_13XX :
    	screen = (displayObj*) new SSD_13XX_Obj(csPin,rsPin);
    	if (screen) {
        if (screen->dispObjBegin()) {
            screen->setRotation(inRotation);
            return true;
        }
      }
      return false;
      break;
   */
    /* This one causes nothing but compiling issues. And its too big for a UNO. And obsolite anyway.
    case ADAFRUIT_376 :
      screen = (displayObj*) new adafruit_376_Obj();
      if (screen) {
        if (screen->dispObjBegin()) {
        		screen->setRotation(inRotation);
        		return true;
        }
      }
      return false;
      break;
    */
    default : return false;
  }
}

