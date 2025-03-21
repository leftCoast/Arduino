#include <wristDisp.h>

#include <DFRobot_0995_Obj.h>
#include <idlers.h>
#include <timeObj.h>
#include <strTools.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
// **************************************************
// *****************   wristDisp    ***************** 
// **************************************************

timeObj	screenTimer(1000);

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
	analogWrite(backlight,0);                                    // Turn on backlight.
   screen = (displayObj*) new DFRobot_0995_Obj(TFT_CS,TFT_RST);	// Create our display object.
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
   screen->fillScreen(&black);			// Black is a good background.
   
   speedBox = new valueBox(&dBoxRect);
   speedBox->setTypeText("Speed");
   speedBox->setUnitText("kn");
   speedBox->setPrecision(1);
   speedBox->setValue(8.49);
   viewList.addObj(speedBox);
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   
   depthBox = new valueBox(&dBoxRect);
   depthBox->setTypeText("Depth");
   depthBox->setUnitText("Ftm");
   depthBox->setPrecision(0);
   depthBox->setValue(32.2);
   viewList.addObj(depthBox);
	dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
    
	
	COGBox = new valueBox(&dBoxRect);
   COGBox->setTypeText("COG");
   COGBox->setUnitText("Deg");
   COGBox->setPrecision(0);
   COGBox->setValue(NAN);
   viewList.addObj(COGBox);
   dBoxRect.setLocation(dBoxRect.x,dBoxRect.y+boxHeight);
   
	fuelBox = new valueBox(&dBoxRect);
   fuelBox->setTypeText("Fuel");
   fuelBox->setUnitText("%");
   fuelBox->setPrecision(0);
   fuelBox->setValue(NAN);
   viewList.addObj(fuelBox);
    
   //screen->drawRect(firstBox,&red);
}


void wristDisp::checkDisp(void) {
    
	if (screenTimer.ding()) {
		for(int i=0;i<256;i++) {
			analogWrite(backlight,i);
			sleep(5);
		}
		screenTimer.reset();
	}
}




// **************************************************
// *****************    dataBox     ***************** 
// **************************************************


colorObj	labelColor;
colorObj	valueColor;
colorObj	unitsColor;

dataBox::dataBox(rect* inRect)
	: drawGroup(1,1,screen->width(),screen->height()) {
	
	if (inRect) {
		setRect(inRect);
	}
	setup();
}
	
	
dataBox::~dataBox(void) {  }


void dataBox::setup(void) { }


void dataBox::drawSelf(void) {
	
	colorObj startColor;
	colorObj endColor;
	//return;
	startColor.setColor(&black);
	endColor.setColor(LC_NAVY);
	endColor.blend(&blue,50);
	screen->fillRectGradient(x,y,width,height,&startColor,&endColor);
}


// **************************************************
// *****************    valueBox     **************** 
// **************************************************


valueBox::valueBox(rect* inRect)
	: dataBox(inRect) {
	
	noValueStr = NULL;
	setNoValueStr("---");
	precision = 1;
	setup();
}
	
	
valueBox::~valueBox(void) {  }


void valueBox::setup(void) {
	
	colorObj	blueText(LC_LIGHT_BLUE);
	
	typeLabel = new fontLabel();
	typeLabel->setFont(&FreeSansBoldOblique12pt7b,-8);
   typeLabel->setColors(&blueText);
   typeLabel->setLocation(10,10);
   typeLabel->setSize(150,20);
   addObj(typeLabel);

	valueLabel = new fontLabel();
   valueLabel->setFont(&FreeSans18pt7b,-8);
   valueLabel->setColors(&yellow);
   valueLabel->setLocation(10,45);
   valueLabel->setSize(100,30);
   valueLabel->setValue(10.3);
   valueLabel->setPrecision(precision);
   addObj(valueLabel);
   
   unitsLabel = new fontLabel();
   unitsLabel->setFont(&FreeSansBoldOblique12pt7b,-8);
   unitsLabel->setColors(&blueText);
   unitsLabel->setLocation(120,45);
   unitsLabel->setSize(30,30);
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
	
	if (valueLabel) {
		value = inValue;
		if (isnan(value)) {
			valueLabel->setValue(noValueStr);
		} else {
			valueLabel->setValue(value);
		}
	}
}


void valueBox::setPrecision(int inPrecision) {
	
	if (valueLabel) {
		precision = inPrecision;
		valueLabel->setPrecision(precision);
		valueLabel->setValue(value);
	}
}


void valueBox::setNoValueStr(const char* inStr) { heapStr(&noValueStr,inStr); }
		
		
		
		
		