
#include "screen.h"
#include "adafruit_1431_Obj.h"
#include "adafruit_684_Obj.h"
#include "adafruit_1947_Obj.h"
#include <LC_SPI.h>

#include <SD.h>

displayObj* screen = NULL;

bool initScreen(byte hardware, byte csPin, byte inRotation) { 
	return initScreen(hardware,csPin, -1,inRotation);
}
	
	
bool initScreen(byte hardware,byte csPin,byte rsPin,byte inRotation) {

	switch (hardware) {
		
		case ADAFRUIT_684 :
			screen = (displayObj*) new adafruit_684_Obj(csPin,rsPin);
			if (screen) {
				if (screen->dispObjBegin()) {
					screen->setRotation(inRotation);
					return true;
				}
			}
			return false;
		break;
		
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
   
    	default : return false;
  	}
}

