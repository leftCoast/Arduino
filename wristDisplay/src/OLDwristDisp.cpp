#include <wristDisp.h>
#include <DFRobot_0995_Obj.h>
#include <idlers.h>
#include <timeObj.h>
#include <strTools.h>
#include <serialStr.h>
#include <runningAvg.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <debug.h>

#define DELEM_STR " \t\n\r"

// **************************************************
// *****************   wristDisp    ***************** 
// **************************************************

timeObj		screenTimer(1000);
bool			newVal = false;
bool			ct_alarm;
bool			op_alarm;
int			rpm;
float			fuelVal;
float			speedVal;
float			depthVal;
float			airPVal;
float			a_pDelta;
colorObj		blueText(LC_LIGHT_BLUE);
serialStr	cmdMgr(&Serial1);



enum valueType {
	speed,
	depth,
	inHg,
	deltaHg,
	overTempAlarm,
	oilPsiAlarm,
	engineRPM,
	fuel
};


valueType newValueType;

void	gotStr(char* cmdStr) {

	char* aStr = NULL;			
	heapStr(&aStr,cmdStr);		// Allocates and stuffs it in.
	char*	token;
	
	//Serial.println(cmdStr);
	token = strtok(aStr,DELEM_STR);
	if (!strcmp(token,"speed")) {
		token = strtok(NULL,DELEM_STR);
		speedVal = atof(token);
		newValueType = speed;
	} else if (!strcmp(token,"depth")) {
		tokend = strtok(NULL,DELEM_STR);
		depthVal = atof(token);
		newValueType = depth;
	} else if (!strcmp(token,"a_pressure")) {
		token = strtok(NULL,DELEM_STR);
		airPVal = atof(token);
		newValueType = inHg;
	} else if (!strcmp(token,"a_pdelta")) {
		token = strtok(NULL,DELEM_STR);
		a_pDelta = atof(token);
		newValueType = deltaHg;
	} else if (!strcmp(token,"ct_alarm")) {
		token = strtok(NULL,DELEM_STR);
		ct_alarm = (bool) atoi(token);
		newValueType = overTempAlarm;
	} else if (!strcmp(token,"op_alarm")) {
		token = strtok(NULL,DELEM_STR);
		op_alarm = (bool) atoi(token);
		newValueType = oilPsiAlarm;
	} else if (!strcmp(token,"rpm")) {
		token = strtok(NULL,DELEM_STR);
		rpm = atoi(token);
		newValueType = engineRPM;
	} else if (!strcmp(token,"fuel")) {
		token = strtok(NULL,DELEM_STR);
		fuelVal = atof(token);
		newValueType = fuel;
	} 
	newVal = true;
	freeStr(&aStr);
}

void startSerial(void) { cmdMgr.setCallback(gotStr); }



wristDisp::wristDisp(int inChipSelect,int inReset,int inBacklight,int inDataCommand) {  

	chipSelect	= inChipSelect;
	reset			= inReset;
	backlight	= inBacklight;
	dataCommand	= inDataCommand;
	currentErr	= noErr;
}


wristDisp::~wristDisp(void) {  }
	
	
bool wristDisp::begin(void) {

	bool success;

	success = false;
	pinMode(backlight,OUTPUT);													// Setup backlight pin.
	analogWrite(backlight,0);                                    	// Turn on backlight.
   screen = (displayObj*) new DFRobot_0995_Obj(chipSelect,reset);	// Create our display object.
   if (screen) {																	// If we allocated one.
      if (screen->begin()) {													// If we can initialize it.
         setupDisp();															// Setup the display for viewing.
         success = true;														// Calling this as a good start.
      } else {																		// Else..
      	currentErr = initErr;												// It wouldn't initialize.
      }																				//
   } else {																			// Else..
   	currentErr = memErr;														// Not enough RAM. Yikes!
   }																					//
   if (success) {
   
   }																					//
   return success;																// return if we were able to fine this thing up.
}


dispErr wristDisp::errMsg(void) { return currentErr; }


void wristDisp::setupDisp() {
	
	rect			dBoxRect(0,0,screen->width(),screen->height()/4);
	int			boxHeight;

	boxHeight = screen->height()/4; 
	screen->setRotation(PORTRAIT);		// Set orientation.
   
   speedBox = new valueBox(&dBoxRect);
   speedBox->setTypeText("Speed");
   speedBox->setUnitText("kn");
   speedBox->setPrecision(1);
   speedBox->setValue(NAN);
   viewList.addObj(speedBox);
   
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   depthBox = new valueBox(&dBoxRect);
   depthBox->setTypeText("Depth");
   depthBox->setUnitText("Ftm");
   depthBox->setPrecision(1);
   depthBox->setValue(NAN);
   viewList.addObj(depthBox);
	
	/*
	dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   COGBox = new valueBox(&dBoxRect);
   COGBox->setTypeText("Bearing");
   COGBox->setUnitText("Deg");
   COGBox->setPrecision(0);
   COGBox->setValue(NAN);
   viewList.addObj(COGBox);
   */
   
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   barometerBox = new valueBox(&dBoxRect);
   barometerBox->setTypeText("Barometer");
   barometerBox->setUnitText("\"Hg");
   barometerBox->setPrecision(2);
   barometerBox->setValue(NAN);
   viewList.addObj(barometerBox);
   
   /*
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   fuelBox = new valueBarBox(&dBoxRect);
   if (fuelBox) {
   	fuelBox->setTypeText("Fuel");
   	fuelBox->setUnitText("%");
   	fuelBox->setValue(5);
   	viewList.addObj(fuelBox);
   }
   */
   
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   enginePanel = new engineBox(&dBoxRect);
   if (enginePanel) {
   	viewList.addObj(enginePanel);
   }
}


void wristDisp::checkDisp(void) {
    
   
	if (screenTimer.ding()) {
		for(int i=0;i<256;i++) {
			analogWrite(backlight,i);
			sleep(5);
		}
		screenTimer.reset();
	}
	if (newVal) {
		switch(newValueType) {
			case speed	: speedBox->setValue(speedVal);		break;
			case depth	: depthBox->setValue(depthVal);		break;
			case fuel	: fuelBox->setValue(fuelVal);			break;
			case inHg	: 
				Serial.print("*** Sending : ");
				Serial.println(airPVal);
				barometerBox->setValue(airPVal);	break;
			//case deltaHg	: barometerBox->setDValue(airPVal);
		}
		newVal = false;
	}
}




// **************************************************
// *****************    dataBox     ***************** 
// **************************************************


colorObj		labelColor;
colorObj		valueColor;
colorObj		unitsColor;

dataBox::dataBox(rect* inRect)
	: drawGroup(1,1,screen->width(),screen->height()) {
	
	if (inRect) {
		setRect(inRect);
	}
	startColor.setColor(&black);
	endColor.setColor(LC_NAVY);
	endColor.blend(&blue,50);
	setup();
}
	
	
dataBox::~dataBox(void) { }


void dataBox::setup(void) { }
	

void dataBox::drawSelf(void) { screen->fillRectGradient(this,&startColor,&endColor); }


void dataBox::draw(void) {

	bitmap aBitmap(width,height);
	
	vPort.beginDraw(&aBitmap,x,y);
	drawGroup::draw();
	vPort.endDraw();
	screen->blit(x,y,&aBitmap);
}



// **************************************************
// *****************    valueBox     **************** 
// **************************************************


valueBox::valueBox(rect* inRect)
	: dataBox(inRect) {
	
	noValueStr = NULL;
	setNoValueStr("---");
	precision = 1;
	decMult = 10;
	sawNAN = false;
	setup();
}
	
	
valueBox::~valueBox(void) {  }


void valueBox::setup(void) {
	
	//colorObj	blueText(LC_LIGHT_BLUE);
	
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



void valueBox::setPrecision(int inPrecision) {
	
	if (valueLabel) {
		precision = inPrecision;
		decMult = pow(10, precision);
		valueLabel->setPrecision(precision);
		valueLabel->setValue(value);
	}
}


void valueBox::setNoValueStr(const char* inStr) { heapStr(&noValueStr,inStr); }
		
		
		
// **************************************************
// *****************   valueBarBox   **************** 
// **************************************************	
	
#define	BAR_X	25
#define	BAR_Y	40
#define	BAR_W TFT_WIDTH-50
#define	BAR_H 20

	
valueBarBox::valueBarBox(rect* inRect)
	: dataBox(inRect) {  
	
	setup();
}
	
	
valueBarBox::~valueBarBox(void) {  }

	
void	valueBarBox::setTypeText(const char* inStr) {

	if (typeLabel) {
			typeLabel->setValue(inStr);
			setNeedRefresh();
		}
	}
	

// We don't do units on this one so we do nothing.	
void	valueBarBox::setUnitText(const char* inStr) { }


void	valueBarBox::setValue(float inValue) {

	valueBar->setValue(inValue);
	setNeedRefresh();
}


void	valueBarBox::setup(void) { 

	rect		location;
	
	typeLabel = new fontLabel();
	typeLabel->setFont(&FreeSansBoldOblique12pt7b,-8);
   typeLabel->setColors(&blueText);
   typeLabel->setLocation(10,10);
   typeLabel->setSize(150,20);
   typeLabel->setJustify(TEXT_CENTER);
   addObj(typeLabel);
   
   location.setRect(BAR_X-1,BAR_Y,BAR_W,BAR_H);
   valueBar = new fuelBargraph(&location,leftRight); 
   valueBar->addColor(0,&red);
   valueBar->addColor(10,&red);
   valueBar->addColor(12.5,&yellow);
   valueBar->addColor(15,&yellow);
   valueBar->addColor(25,&green);
   valueBar->addColor(100,&green);
   addObj(valueBar);

   theE = new fontLabel();
   theE->setFont(&FreeSansBoldOblique12pt7b,0);
   theE->setColors(&blueText);
   theE->setLocation(0,BAR_Y);
   theE->setSize(20,20);
   theE->setValue("E");
   addObj(theE);
   
   theF = new fontLabel();
   theF->setFont(&FreeSansBoldOblique12pt7b,0);
   theF->setColors(&blueText);
   theF->setLocation(BAR_X+BAR_W,BAR_Y);
   theF->setSize(20,20);
   theF->setValue("F");
   addObj(theF);
};
	

	
// **************************************************
// *****************  fuelBargraph   **************** 
// **************************************************	


fuelBargraph::fuelBargraph(rect* inRect,orientation inOrientation)
	: colorBargraph(inRect,inOrientation) {  }
	
fuelBargraph::~fuelBargraph(void) {  }	
	

void  fuelBargraph::drawSelf(void) {

	colorObj	scaleColor(LC_LIGHT_BLUE);
	float		lineHeight;
	float		qWidth;
	point		lineEnds[3];
	
	lineHeight = (height*0.75);
	qWidth = width/4.0;
	lineEnds[0].x = round(x+qWidth);
	lineEnds[1].x = round(x+qWidth*2);
	lineEnds[2].x = round(x+qWidth*3);
	lineEnds[0].y = round(y+height-(lineHeight/2.0));
	lineEnds[1].y = round(y+height-lineHeight);
	lineEnds[2].y = round(y+height-(lineHeight/2.0));
	colorBargraph::drawSelf();
	screen->drawRect(this,&scaleColor);
	
	if (lineEnds[0].x<=drawRect.x+drawRect.width) {
		screen->drawVLine(lineEnds[0].x,lineEnds[0].y,lineHeight/2.0,&black);
	} else {
		screen->drawVLine(lineEnds[0].x,lineEnds[0].y,lineHeight/2.0,&scaleColor);
	}
	if (lineEnds[1].x<=drawRect.x+drawRect.width) {
		screen->drawVLine(lineEnds[1].x,lineEnds[1].y,lineHeight,&black);
	} else {
		screen->drawVLine(lineEnds[1].x,lineEnds[1].y,lineHeight,&scaleColor);
	}
	if (lineEnds[2].x<=drawRect.x+drawRect.width) {
		screen->drawVLine(lineEnds[2].x,lineEnds[2].y,lineHeight/2.0,&black);
	} else {
		screen->drawVLine(lineEnds[2].x,lineEnds[2].y,lineHeight/2.0,&scaleColor);
	}
	
}



// **************************************************
// *******************  engineBox  ****************** 
// **************************************************	


idiotLight::idiotLight(rect* inRect)
	: drawObj(inRect) { active = false; }
	
	
idiotLight::~idiotLight(void) {  }
	
	
void idiotLight::setActive(bool onOff) {

		active = onOff;
		setNeedRefresh();
}



tempLight::tempLight(rect* inRect)
	: idiotLight(inRect) {  }
	
	
tempLight::~tempLight(void) {  }


void tempLight::drawSelf(void) {

	int	radius;
	int	radiusII;
	int	centerX;
	int	centerXII;
	int	centerY;
	int	centerYII;
	rect	aRect;
	
	radius		= width/2;
	radiusII		= THERMO_W2/2;
	centerX		= x+radius;
	centerY		= y+height-radius;
	centerYII	= y + radiusII;
	screen->fillCircle(centerX,centerY,radius,&blueText);
	aRect.x = centerX-radiusII;
	aRect.y = centerYII;
	aRect.width = THERMO_W2;
	aRect.height = centerY - centerYII;
	screen->fillRect(&aRect,&blueText);
	screen->fillCircle(centerX,centerYII,radiusII,&blueText);
}


oilLight::oilLight(rect* inRect)
	: idiotLight(inRect) {  }
	
	
oilLight::~oilLight(void) {  }


void oilLight::drawSelf(void) {

	int	radius;
	int	centerX;
	int	centerY;
	point	pt0;
	point	pt1;
	point	pt2;
	
	radius		= width/2;
	centerX		= x+radius;
	centerY		= y+height-radius;
	pt0.x = x;
	pt0.y = centerY;
	pt1.x = centerX;
	pt1.y = y;
	pt2.x = x + width;
	pt2.y = centerY;
	screen->fillCircle(centerX,centerY,radius,&blueText);
	screen->fillTriangle(&pt1,&pt0,&pt2,&blueText);
}

	
	
engineBox::engineBox(rect* inRect)
	: dataBox(inRect) { setup(); }
	
	
engineBox::~engineBox(void) { }


void engineBox::setup(void) {
//ST
	rect	aRect;
	
	aRect.setRect(THERMO_X,THERMO_Y,THERMO_W,THERMO_H);
	ourTempLight = new tempLight(&aRect);
	addObj(ourTempLight);
	aRect.setRect(OIL_X,OIL_Y,OIL_W,OIL_H);
	ourOilLight = new oilLight(&aRect);
	addObj(ourOilLight);
}

	

