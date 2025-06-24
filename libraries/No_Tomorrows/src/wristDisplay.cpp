#include <wristDisplay.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <strTools.h>


#define TFT_CS			10
#define TFT_RST		3
#define TFT_BL			5

#define DBOX_X			0
#define DBOX_Y			0
#define DBOX_W			TFT_WIDTH
#define DBOX_H			TFT_HEIGHT/4

#define PARSE_BUFF	65

offscreen	vPort;
bitmap		gBitmap(DBOX_W,DBOX_H);
colorObj		startColor;
colorObj		endColor;
colorObj		blueText(LC_LIGHT_BLUE);



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
	USBParser = new lilParser(PARSE_BUFF);
	wlessParser = new lilParser(PARSE_BUFF);
	if (!USBParser || !wlessParser) {
		Serial.println("Can't create parsers.");
		while(1);
	}
	addCmd(setSpeedCmd,"setSpeed");
	addCmd(setDepthCmd,"setDepth");
	addCmd(setBearingCmd,"setBearing");
	addCmd(setCOGCmd,"setCOG");
	
	startColor.setColor(&black);
	endColor.setColor(LC_NAVY);
	endColor.blend(&blue,50);
}


void wristDisplay::addCmd(cmds cmd,const char* text) {
	
	USBParser->addCmd(cmd,text);
	wlessParser->addCmd(cmd,text);
}

	
void wristDisplay::setupDisplay(void) {
	
	speed = new valueBox;
	if (speed) {
		speed->setPos(0);
		speed->setTypeText("Speed");
		speed->setUnitText("Kts");
		speed->setPrecision(1);
		speed->setValue(NAN);
		viewList.addObj(speed);
	}
	depth = new valueBox;
	if (depth) {
		depth->setPos(1);
		depth->setTypeText("Depth");
		depth->setUnitText("Ftm");
		depth->setPrecision(1);
		depth->setValue(NAN);
		viewList.addObj(depth);
	}
	bearing = new valueBox;
	if (bearing) {
		bearing->setPos(2);
		bearing->setTypeText("Bearing");
		bearing->setUnitText("Mag");
		bearing->setPrecision(0);
		bearing->setValue(NAN);
		viewList.addObj(bearing);
	}
	cog = new valueBox;
	if (cog) {
		cog->setPos(3);
		cog->setTypeText("COG");
		cog->setUnitText("Mag");
		cog->setPrecision(0);
		cog->setValue(NAN);
		viewList.addObj(cog);
	}
}


void wristDisplay::light(bool onOff) {
	
	if (onOff) {
		analogWrite(TFT_BL,255);
	} else {
		analogWrite(TFT_BL,0);
	}
}


void wristDisplay::loop(void) {

	checkUSBParser();
	checkWlessParser();				
}



// **************************************************
// ************   incoming handelers    *************
// **************************************************


void wristDisplay::checkUSBParser(void) {

	char	inChar;
	int	cmd;
	
	if (Serial.available()) {
		inChar = Serial.read();
		Serial.print(inChar);
		cmd = USBParser->addChar(inChar);
		checkCom(cmd,true);
	}
}

			
void wristDisplay::checkWlessParser(void) {

	char	inChar;
	int	cmd;
	
	if (Serial1.available()) {
		inChar = Serial1.read();
		cmd = wlessParser->addChar(inChar);
		checkCom(cmd,false);
	}
}


void  wristDisplay::checkCom(int cmd,bool debug) {

	switch(cmd) {
		case noCmd				: break;
		case setSpeedCmd 		: handleSetSpeed(debug);	break;
		case setDepthCmd		: handleSetDepth(debug);	break;
		case setBearingCmd	: handleSetBearing(debug);	break;
		case setCOGCmd			: handleSetCOG(debug);		break;
		default					: handleHelp(debug);			break;
	};
}


// Decode, sanity check & send boatspeed value in knots to display
void  wristDisplay::handleSetSpeed(bool debug) {

	float speedVal;
	
	speedVal = NAN;													// Default is : not a number.
	if (debug) {														// If it came in through the USB wire.. (Debugging)
		if (USBParser->numParams()==1) {							// We only expect one param. If this is the case..
			speedVal = atof(USBParser->getNextParam());		// Decode the parameter.
			Serial.print("Setting speed to : ");				// Tell the user about what's up.
			Serial.println(speedVal);								//
		}																	//
	} else {																// Else, it came though the wireless port.
		if (wlessParser->numParams()==1) {						// Again, we only expect one param. If this is the case..
			speedVal = atof(wlessParser->getNextParam());	// Decode the parameter.
		}																	//
	}																		//
	if (!isnan(speedVal)) {											// Sanity, check that we got an actual value.
		if (speedVal>=0 && speedVal<99) {						// It's in range..
			speed->setValue(speedVal);								// Fine, send it in.
			return;														// We are done here, gracefull early exit.
		}																	//
	}																		//
	speed->setValue(NAN);											// If we get here? Tell 'em we got a NAN.
}


// Decode, sanity check & send water depth value in fathoms to display
void  wristDisplay::handleSetDepth(bool debug) {

	float depthVal;
	
	depthVal = NAN;													// Default is : not a number.
	if (debug) {														// If it came in through the USB wire.. (Debugging)
		if (USBParser->numParams()==1) {							// We only expect one param. If this is the case..
			depthVal = atof(USBParser->getNextParam());		// Decode the parameter.
			Serial.print("Setting depth to : ");				// Tell the user about what's up.
			Serial.println(depthVal);								//
		}																	//
	} else {																// Else, it came though the wireless port.
		if (wlessParser->numParams()==1) {						// Again, we only expect one param. If this is the case..
			depthVal = atof(wlessParser->getNextParam());	// Decode the parameter.
		}																	//
	}																		//
	if (!isnan(depthVal)) {											// Sanity, check that we got an actual value.
		if (depthVal>=0) {											// It's in range..
			depth->setValue(depthVal);								// Fine, send it in.
			return;														// We are done here, gracefull early exit.
		}																	//
	}																		//
	depth->setValue(NAN);											// If we get here? Tell 'em we got a NAN.
}


// Decode, sanity check & send magnetic bearing to next mark value to display
void  wristDisplay::handleSetBearing(bool debug) {
	
	float bearingVal;
	
	bearingVal = NAN;													// Default is : not a number.
	if (debug) {														// If it came in through the USB wire.. (Debugging)
		if (USBParser->numParams()==1) {							// We only expect one param. If this is the case..
			bearingVal = atof(USBParser->getNextParam());	// Decode the parameter.
			Serial.print("Setting bearing to : ");				// Tell the user about what's up.
			Serial.println(bearingVal);							//
		}																	//
	} else {																// Else, it came though the wireless port.
		if (wlessParser->numParams()==1) {						// Again, we only expect one param. If this is the case..
			bearingVal = atof(wlessParser->getNextParam());	// Decode the parameter.
		}																	//
	}																		//
	if (!isnan(bearingVal)) {										// Sanity, check that we got an actual value.
		if (bearingVal>=0 && bearingVal<360) {					// It's in range..
			bearing->setValue(bearingVal);						// Fine, send it in.
			return;														// We are done here, gracefull early exit.
		}																	//
	}																		//
	bearing->setValue(NAN);											// If we get here? Tell 'em we got a NAN.
}


// Decode, sanity check & send magnetic course over ground value to display
void  wristDisplay::handleSetCOG(bool debug) {
	
	float COGVal;
	
	COGVal = NAN;														// Default is : not a number.
	if (debug) {														// If it came in through the USB wire.. (Debugging)
		if (USBParser->numParams()==1) {							// We only expect one param. If this is the case..
			COGVal = atof(USBParser->getNextParam());			// Decode the parameter.
			Serial.print("Setting COG to : ");					// Tell the user about what's up.
			Serial.println(COGVal);									//
		}																	//
	} else {																// Else, it came though the wireless port.
		if (wlessParser->numParams()==1) {						// Again, we only expect one param. If this is the case..
			COGVal = atof(wlessParser->getNextParam());		// Decode the parameter.
		}																	//
	}																		//
	if (!isnan(COGVal)) {											// Sanity, check that we got an actual value.
		if (COGVal>=0 && COGVal<360) {							// It's in range..
			cog->setValue(COGVal);									// Fine, send it in.
			return;														// We are done here, gracefull early exit.
		}																	//
	}																		//
	cog->setValue(NAN);												// If we get here? Tell 'em we got a NAN.
}	


void  wristDisplay::handleHelp(bool debug) {
	
	if (debug) {
		Serial.println();
		Serial.println("---------------------------------");
		Serial.println("setSpeed speed               : Set boatspeed in Knots.");
		Serial.println("setDepth depth               : Set water depth in Fathoms.");
		Serial.println("setBearing bearing           : Set the bearing to next mark in degrees magnetic.");
		Serial.println("setCOG COG                   : Set the course over ground in degrees magnetic.");
		Serial.println("setEngine fuel RPM oil water : Set engine parameters. Fuel level, RPM, Oil PSI alarm, Water temp alarm.");
		Serial.println("Anything else?               : See this printout.");
		Serial.println();
		Serial.println();
	}
}



// **************************************************
// *****************   OSDataBox    ***************** 
// **************************************************
//
// Base rectangle box that defaults to a color sweep
// background, and is drawn offline.

OSDataBox::OSDataBox(void)
	: drawGroup(DBOX_X,DBOX_Y,DBOX_W,DBOX_H) {
	
	oldVariableArea.setRect(0,0,0,0);	// Empty rect for now.
	newVariableArea.setRect(0,0,0,0);	// Same here..
	position = -1;								// Our position on the screen -1 = Not on screen.	
}


OSDataBox::~OSDataBox(void) {  }
	
	
void OSDataBox::setPos(int newPos) { 

	position = newPos;
	if (position>=0) {
		setNeedRefresh();
	}
}


void OSDataBox::setup(void) { }


void OSDataBox::drawSelf(void) { screen->fillRectGradient(this,&startColor,&endColor); }


void OSDataBox::draw(void) {

	vPort.beginDraw(&gBitmap,0,0);
	drawGroup::draw();
	vPort.endDraw();
	screen->blit(0,position*DBOX_H,&gBitmap);
}
	


// **************************************************
// *****************    valueBox     **************** 
// **************************************************
//
// A box based on ISDataBox with label, units and a
// numeric value.
//


valueBox::valueBox(void)
	: OSDataBox() {
	
	noValueStr = NULL;
	setNoValueStr("---");
	precision	= 1;
	decMult		= 10;
	sawNAN		= false;
	setup();										// We are ASSUMING these will be created dynamically.								
}
	
	
valueBox::~valueBox(void) { if(noValueStr) freeStr(&noValueStr); }


void valueBox::setup(void) {
		
	typeLabel = new fontLabel();
	typeLabel->setFont(&FreeSansBoldOblique12pt7b,-8);
   typeLabel->setColors(&blueText);
   typeLabel->setLocation(10,10);
   typeLabel->setSize(150,20);
   typeLabel->setJustify(TEXT_CENTER);
   addObj(typeLabel);

	valueLabel = new fontLabel();
   valueLabel->setFont(&FreeSans18pt7b,-8);
   valueLabel->setColors(&yellow);
   valueLabel->setLocation(10,45);
   valueLabel->setSize(100,30);
   valueLabel->setValue(10.3);
   valueLabel->setPrecision(precision);
   valueLabel->setJustify(TEXT_CENTER);
   addObj(valueLabel);
   
   unitsLabel = new fontLabel();
   unitsLabel->setFont(&FreeSansBoldOblique12pt7b,-8);
   unitsLabel->setColors(&blueText);
   unitsLabel->setLocation(120,45);
   unitsLabel->setSize(45,30);
   addObj(unitsLabel);
}


void valueBox::setTypeText(const char* inStr) {
	
	if (typeLabel) {
		typeLabel->setValue(inStr);
	}
}


void valueBox::setUnitText(const char* inStr) {
	
	if (unitsLabel) {
		unitsLabel->setValue(inStr);
	}
}


void valueBox::setValue(float inValue) {
	
	int intVal1;
	int intVal2;

	if (valueLabel) {										// If there is no value label we just don't care..
		if (isnan(inValue)) {
			if (!sawNAN) {
				valueLabel->setValue(noValueStr);
				sawNAN = true;
				setNeedRefresh();
			}
		} else {
			if (sawNAN) {
				sawNAN = false;
				value = inValue;
				valueLabel->setValue(value);
				setNeedRefresh();
			} else {
				intVal1 = round(value*decMult);
				intVal2 = round(inValue*decMult);
				if (intVal1!=intVal2) {
					value = inValue;
					valueLabel->setValue(value);
					setNeedRefresh();
				}
			}
		}
	}
}



void valueBox::setPrecision(int inPrecision) {
	
	if (valueLabel) {
		precision = inPrecision;
		decMult = pow(10, precision);
		valueLabel->setPrecision(precision);
		valueLabel->setValue(value);
	}
}


void valueBox::setNoValueStr(const char* inStr) { heapStr(&noValueStr,inStr); }
		
						