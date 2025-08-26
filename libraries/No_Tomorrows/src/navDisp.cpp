#include <strTools.h>
#include <navDisp.h>
#include <GPSReader.h>
#include <navigation.h>

//#include <fonts/FreeSansOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique12pt7b.h>

#define LED_RECT					300,5,10,10

#define SPEEDBOX_RECT			30,30,250,64
#define DEPTHBOX_RECT			30,105,250,64
#define BEARINGBOX_RECT			30,180,250,64
#define DISTANCEBOX_RECT		30,255,250,64
#define BOROMETERBOX_RECT		30,330,250,64

#define LAT_X						55
#define LAT_Y						420
#define LAT_W						480
#define LAT_H						36
#define LON_X						LAT_X
#define LON_Y						LAT_Y+30
#define LON_W						LAT_W
#define LON_H						LAT_H


#define AFF_SANS_BOLD_24_OB	&FreeSansBoldOblique24pt7b,45,-12
#define AFF_SANS_BOLD_12_OB	&FreeSansBoldOblique12pt7b,24,-6


navDisp::navDisp (void) { updateTimer = new timeObj(2000); }


navDisp::~navDisp(void) { if (updateTimer) delete(updateTimer); }


void navDisp::setup(void) {

	rect	LEDRect;
	
	screen->fillScreen(&black);
	
	LEDRect.setRect(LED_RECT);
	fixLED = new LED(&LEDRect,&green,&red);
	viewList.addObj(fixLED);
	
	timeLabel = new label(100,0,200,32);
	timeLabel->setColors(&yellow,&black);
	timeLabel->setTextSize(2);
	viewList.addObj(timeLabel);
	
	latLabel = new label(LAT_X,LAT_Y,LAT_W,LAT_H);
	latLabel->setColors(&yellow,&black);
	latLabel->setTextSize(2);
	viewList.addObj(latLabel);
	
	lonLabel = new label(LON_X,LON_Y,LON_W,LON_H);
	lonLabel->setColors(&yellow,&black);
	lonLabel->setTextSize(2);
	viewList.addObj(lonLabel);
	
	knotGauge = new valueBox(SPEEDBOX_RECT,"Kn",1);
	if (knotGauge) {
		knotGauge->setup();
		viewList.addObj(knotGauge);
	}
	
	depthGauge = new valueBox(DEPTHBOX_RECT,"Fm",1);
	if (depthGauge) {
		depthGauge->setup();
		viewList.addObj(depthGauge);
	}
	
	bearingGauge = new valueBox(BEARINGBOX_RECT,"Deg m",0);
	if (bearingGauge) {
		bearingGauge->setup((float*)&(ourNavApp.bearingVal));
		viewList.addObj(bearingGauge);
	}
	
	distanceGauge = new valueBox(DISTANCEBOX_RECT,"Nmi",1);
	if (distanceGauge) {
		distanceGauge->setup((float*)&(ourNavApp.distanceVal));
		viewList.addObj(distanceGauge);
	}
	
	barometerGauge = new valueBox(BOROMETERBOX_RECT,"InHg",2);
	if (barometerGauge) {
		barometerGauge->setup(&(ourNavApp.airPSI));
		viewList.addObj(barometerGauge);
	}
}


void navDisp::showPos(globalPos* fix) {

	char		outStr[40];
	double	value;
	char		qStr[3];
	
	if (ourGPS->valid) {
		
		fixLED->setState(true);
		
		sprintf(outStr,"GMT %02d:%02d",ourGPS->hours,ourGPS->min);
		timeLabel->setValue(outStr);
		
		strcpy(qStr," N");
		if (fix->getLatQuad()==south) {
			strcpy(qStr," S");
		}
		value = fix->getLatAsDbl();
		if (value<0) value = -value;
		sprintf (outStr,"Lat : %*f%s",10,value,qStr);
		latLabel->setValue(outStr);
		
		strcpy(qStr," W");
		if (fix->getLonQuad()==east) {
			strcpy(qStr," E");
		}
		value = fix->getLonAsDbl();
		if (value<0) value = -value;
		sprintf (outStr,"Lon : %*f%s",10,value,qStr);
		lonLabel->setValue(outStr);
	} else {
		fixLED->setState(false);
		sprintf(outStr,"GMT --:--");
		timeLabel->setValue(outStr);
		sprintf (outStr,"Lat : ---.--");
		latLabel->setValue(outStr);
		sprintf (outStr,"Lon : ---.--");
		lonLabel->setValue(outStr);
	}
}



// *************     LED      *************


LED::LED(rect* inRect,colorObj* inOnColor,colorObj* inOffColor)
	: colorRect(inRect,inOnColor) {
	
	setColors(inOnColor,inOffColor);
	setState(false);
}
	
	
LED::~LED(void) {  }
	
	
void LED::setColors(colorObj* inOnColor,colorObj* inOffColor) {

	onColor.setColor(inOnColor);
	offColor.setColor(inOffColor);
	needRefresh = true;
}


void LED::setState(bool onOff) {

	if (onOff) {
		setColor(&onColor);
	} else {
		setColor(&offColor);
	}
	ourState = onOff;
}


void LED::drawSelf(void) {

	int	dia;
	
	dia = (width+height)/2;
	screen->fillCircle(x,y,dia,(colorObj*)this);
}



// ************* erasibleText *************


erasibleText::erasibleText(void)
	: fontLabel() {  }
	
erasibleText::erasibleText(rect* inRect)
	: fontLabel(inRect) {  }
	
	
erasibleText::erasibleText(int inX, int inY, int inW,int inH)
	: fontLabel(inX,inY,inW,inH) {  }	
	
	
erasibleText::~erasibleText(void) {  }
	
	
void erasibleText::drawSelf(void) {

	rect	aRect(this);
	
	aRect.width = aRect.width+8;
	screen->fillRect(&aRect,&backColor);
	//screen->drawRect(&aRect,&green);			// GREEN
	fontLabel::drawSelf();
}
	
	
	
// ************* valueBox *************	


valueBox::valueBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec)
	: drawGroup(inX,inY,inWidth,inHeight) {
	
	label	= NULL;
	prec	= inPrec;
	heapStr(&label,inLabel);
	dataSource = NULL;
	savedIntVal = 0;
	updateTimer = new timeObj(1000);
	isNanNow = true;
}
	
	
valueBox::~valueBox(void) {

	freeStr(&label);
	if (updateTimer) delete(updateTimer);
}


void valueBox::setup(float* inSource) {

	dataSource = inSource;
	valueLabel = new erasibleText();
	if (valueLabel) {
		valueLabel->setFont(AFF_SANS_BOLD_24_OB);
		valueLabel->x = 5;
		valueLabel->y = 10;
		valueLabel->width = 120;
		valueLabel->setColors(&yellow,&black);
		valueLabel->setPrecision(prec);
		valueLabel->setJustify(TEXT_RIGHT);
		valueLabel->setValue("--");
		addObj(valueLabel);
	}
	unitsLabel = new fontLabel();
	if (unitsLabel) {
		unitsLabel->setFont(AFF_SANS_BOLD_12_OB);
		unitsLabel->x = 150;
		unitsLabel->y = 24;
		unitsLabel->width = 80;
		unitsLabel->setColors(&yellow,&black);
		unitsLabel->setValue(label);
		addObj(unitsLabel);
	}
	hookup();
}


void valueBox::drawSelf(void) { /*screen->drawRect(this,&blue);*/ }


void valueBox::setValue(float value) {
	
	
	if (isnan(value)&&!isNanNow) {
		valueLabel->setValue("--");
		isNanNow = true;
	} else if (!isnan(value)) {
		valueLabel->setValue(value);
		isNanNow = false;
	}
}


void valueBox::idle(void) {

	int	newIntVal;
	float	factor;
	
	drawGroup::idle();										// Let our parent have a go.
	if (updateTimer->ding()) {								// If our timer goes off..								
		if (dataSource) {										// If we have a data source..
			if (isnan(*dataSource)) {						// Quick sanity check.
				setValue(NAN);									// Is a nan? Send it.
				return;											// And exit.
			}														//
			factor = pow(10,prec);							// Calculate the multiplication factor.
			newIntVal = round(*dataSource * factor);	// Setup a integer version of the value.
			if (newIntVal!=savedIntVal) {					// If it's different than the saved integer of what we have now..
				setValue(newIntVal/factor);				// Set the new value to the screen.
				savedIntVal = newIntVal;					// Save off our new integer value for later.
			}														//
		}															//
		updateTimer->start();								// Reset the timer.
	}
}




	

