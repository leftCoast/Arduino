#include <wristDisplay.h>


#define TFT_CS		10
#define TFT_RST	3
#define TFT_BL		5
//#define TFT_DC		9

#define DBOX_X		0
#define DBOX_Y		0
#define DBOX_W		TFT_WIDTH
#define DBOX_H		TFT_HEIGHT/4


wristDisplay::wristDisplay(void) {  }


wristDisplay::~wristDisplay(void) {  }
	
void wristDisplay::setup(void) {

	bool success;
	
	success		= false;

	pinMode(TFT_BL,OUTPUT);														// Setup backlight pin.
	light(true);																	// Turn on backlight.
	screen = (displayObj*) new DFRobot_0995_Obj(TFT_CS,TFT_RST);	// Create our display object.
	if (screen) {																	// If we allocated one.
		if (screen->begin()) {													// If we can initialize it.
			setupDisplay();														// Setup the display for viewing.
			success = true;														// Calling this as a good start.
		}
	} if (!success) {
		Serial.println("Initialization error.");
		while(1);
	}
}


void wristDisplay::setupDisplay(void) {


}


void wristDisplay::light(bool onOff) {
	
	if (onOff) {
		analogWrite(TFT_BL,0);
	} else {
		analogWrite(TFT_BL,255);
	}
}


void wristDisplay::loop(void) {
	
	checkUSBParser();
	checkWlessParser();				
}


void wristDisplay::checkUSBParser(void) {

	char	inChar;
	int	cmd;
	
	if (Serial.available()) {
		inChar = Serial.read();
		Serial.print(inChar);
		cmd = USBParser.addChar(inChar);
		switch(cmd) {
		
		};
	}
}

			
void wristDisplay::checkWlessParser(void) {

	char	inChar;
	int	cmd;
	
	if (Serial1.available()) {
		inChar = Serial1.read();
		cmd = wlessParser.addChar(inChar);
		switch(cmd) {
		
		};
	}
}

	
