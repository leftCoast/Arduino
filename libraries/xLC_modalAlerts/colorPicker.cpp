#include <colorPicker.h>
//#include <debug.h>


#define CP_X		20
#define CP_Y		25
#define CP_W		240 - 2 * CP_X
#define CP_H		200

#define OK_X		CP_W - 42
#define OK_Y		CP_H - 40
#define CANCEL_X	10
#define CANCEL_Y	OK_Y

#define SLIDE_RED_X	40
#define SLIDE_RED_Y	40
#define SLIDE_RED_W	CP_W - 2 * SLIDE_RED_X
#define SLIDE_RED_H	20

#define SLIDE_GREEN_X	SLIDE_RED_X
#define SLIDE_GREEN_Y	SLIDE_RED_Y + 30
#define SLIDE_GREEN_W	SLIDE_RED_W
#define SLIDE_GREEN_H	SLIDE_RED_H

#define SLIDE_BLUE_X	SLIDE_RED_X
#define SLIDE_BLUE_Y	SLIDE_GREEN_Y + 30
#define SLIDE_BLUE_W	SLIDE_RED_W
#define SLIDE_BLUE_H	SLIDE_RED_H

#define SWATCH_X		84		// CP_W / 2 - 16
#define SWATCH_Y		SLIDE_BLUE_Y + 40
#define SWATCH_W		32
#define SWATCH_H		32

#define REDRAW_MS		100

colorPicker::colorPicker(listener* inListener)
	: alertObj("Choose a color",inListener,noIconAlert,true,true) {

	this->setRect(CP_X,CP_Y,CP_W,CP_H);
	okBtn->setLocation(OK_X,OK_Y);
	cancelBtn->setLocation(CANCEL_X,CANCEL_Y);
	hookup();
	percentToByte.setValues(0,100,0,255);
	byteToPercent.setValues(0,255,0,100);
	changeTimer.setTime(REDRAW_MS);
	savedRed		= 127;
	savedGreen	= 127;
	savedBlue	= 127;
	redSlider = new slider(SLIDE_RED_X,SLIDE_RED_Y,SLIDE_RED_W,SLIDE_RED_H);
	greenSlider = new slider(SLIDE_GREEN_X,SLIDE_GREEN_Y,SLIDE_GREEN_W,SLIDE_GREEN_H);
	blueSlider = new slider(SLIDE_BLUE_X,SLIDE_BLUE_Y,SLIDE_BLUE_W,SLIDE_BLUE_H);
	if (redSlider&&greenSlider&&blueSlider) {
		redSlider->knob->setColor(&red);
		greenSlider->knob->setColor(&green);
		blueSlider->knob->setColor(&blue);
		addObj(redSlider);
		addObj(greenSlider);
		addObj(blueSlider);
		swatch = new colorRect(SWATCH_X,SWATCH_Y,SWATCH_W,SWATCH_H,&green);
		if (swatch) {
			swatch->setInset(-1);
			addObj(swatch);
		}
	}
}


colorPicker::~colorPicker(void) {  }


void colorPicker::setColor(colorObj* inColor) {

	savedRed		= byteToPercent.map(inColor->getRed());
	savedGreen	= byteToPercent.map(inColor->getGreen());
	savedBlue	= byteToPercent.map(inColor->getBlue());		
	redSlider->setValue(savedRed);
	greenSlider->setValue(savedGreen);
	blueSlider->setValue(savedBlue);
}


void colorPicker::idle(void) {

	byte redVal;
	byte greenVal;
	byte blueVal;
	
	if (changeTimer.ding()) {
		redVal = round(percentToByte.map(redSlider->getValue()));
		greenVal = round(percentToByte.map(greenSlider->getValue()));
		blueVal = round(percentToByte.map(blueSlider->getValue()));
		if (redVal!=savedRed||greenVal!=savedGreen||blueVal!=savedBlue) {
			swatch->setColor(redVal,greenVal,blueVal);
			savedRed		= redVal;
			savedGreen	= greenVal;
			savedBlue	= blueVal;
		}
		changeTimer.start();
	}
}


// We handle ok & cancel. If we have a listener, we'll pass these on.	
void colorPicker::handleCom(stdComs comID) {

	switch(comID) {
		case okCmd		:								// We see ok..
		case cancelCmd	: done = true; break;	// We see cancel.. But do nothing?
		default			: break;						// Should never get here.
	}
	if (ourListener) {
		ourListener->handleCom(comID);
	}
}


colorObj colorPicker::result(void) {

	colorObj	aColor;
	
	aColor.setColor(swatch);
	return aColor;
}


	