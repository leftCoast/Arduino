#include <strTools.h>
#include <navDisp.h>
#include <GPSReader.h>
#include <navigation.h>
/*
//#include <fonts/FreeSansOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique12pt7b.h>
#include <fonts/FreeSansBoldOblique9pt7b.h>
#include <fonts/FreeMono12pt7b.h>
#include <fonts/FreeMono9pt7b.h>
#include <fonts/FreeSansOblique9pt7b.h>
*/
#define LED_RECT					310,5,10,10
#define FIX_RECT					275,0,30,20
				
#define SPEED_RECT				10,30,250,64
#define DEPTH_RECT				10,105,250,64
#define BEARING_RECT				10,180,250,64
#define DISTANCE_RECT			10,255,250,64
#define BOROMETER_RECT			10,330,250,64

#define LATT_X						10
#define LATT_Y						420
#define LATT_W						70
#define LATT_H						36
#define LONT_X						LATT_X
#define LONT_Y						LATT_Y+30
#define LONT_W						LATT_W
#define LONT_H						LATT_H

#define LAT_X						90
#define LAT_Y						420
#define LAT_W						175
#define LAT_H						36
#define LON_X						LAT_X
#define LON_Y						LAT_Y+30
#define LON_W						LAT_W
#define LON_H						LAT_H

/*
#define AFF_SANS_BOLD_24_OB	&FreeSansBoldOblique24pt7b,45,-12
#define AFF_SANS_BOLD_12_OB	&FreeSansBoldOblique12pt7b,24,-6
#define AFF_SANS_BOLD_9_OB		&FreeSansBoldOblique9pt7b,18,-5
#define AFF_SANS_9_OB			&FreeSansOblique9pt7b,18,-5
#define AFF_MONO_12				&FreeMono12pt7b,20,-6
#define AFF_MONO_9				&FreeMono9pt7b,17,-3
*/

// What flavor data is the GPS databox wanting to display?
#define BEARING	0
#define DIST		1
#define COG			2
#define SOG			3

// What flavor of data is the NMEA databox wanting to display?
#define	RPM_VAL		0
#define	FUEL		1
#define	SPEED		2
#define	DEPTH		3
#define	BARO		4

runningAvg	baroSmoother(10);


navDisp::navDisp (void) { savedStamp = NULL; }


navDisp::~navDisp(void) { freeStr(&savedStamp); }


void navDisp::setup(void) {

	rect			LEDRect;
	fontLabel*	latText;
	fontLabel*	lonText;
	fontLabel*	fixText;
	
	screen->fillScreen(&black);
	
	fixText = new fontLabel(FIX_RECT);
	fixText->setColors(&yellow,&black);
	fixText->setFont(AFF_SANS_9_OB);
	fixText->setValue("Fix");
	viewList.addObj(fixText);
	
	LEDRect.setRect(LED_RECT);
	fixLED = new LED(&LEDRect,&green,&red);
	viewList.addObj(fixLED);
	
	timeLabel = new erasableText(10,0,250,32);
	timeLabel->setColors(&yellow,&black);
	timeLabel->setFont(AFF_MONO_12);
	viewList.addObj(timeLabel);
	
	latText = new fontLabel(LATT_X,LATT_Y,LATT_W,LATT_H);
	latText->setColors(&yellow,&black);
	latText->setFont(AFF_MONO_12);
	latText->setValue("Lat :");
	viewList.addObj(latText);
	
	latLabel = new erasableText(LAT_X,LAT_Y,LAT_W,LAT_H);
	latLabel->setColors(&yellow,&black);
	latLabel->setFont(AFF_MONO_12);
	viewList.addObj(latLabel);
	
	lonText = new fontLabel(LONT_X,LONT_Y,LONT_W,LONT_H);
	lonText->setColors(&yellow,&black);
	lonText->setFont(AFF_MONO_12);
	lonText->setValue("Lon :");
	viewList.addObj(lonText);
	
	lonLabel = new erasableText(LON_X,LON_Y,LON_W,LON_H);
	lonLabel->setColors(&yellow,&black);
	lonLabel->setFont(AFF_MONO_12);
	viewList.addObj(lonLabel);
	
	knotGauge = new NMEABox(SPEED_RECT,"Kn",1);
	if (knotGauge) {
		knotGauge->setup(SPEED);
		viewList.addObj(knotGauge);
	}
	
	depthGauge = new NMEABox(DEPTH_RECT,"Fm",1);
	if (depthGauge) {
		depthGauge->setup(DEPTH);
		viewList.addObj(depthGauge);
	}
	
	bearingGauge = new GPSBox(BEARING_RECT,"Deg m",0);
	if (bearingGauge) {
		bearingGauge->setup(BEARING);
		viewList.addObj(bearingGauge);
	}
	
	distanceGauge = new GPSBox(DISTANCE_RECT,"N mi",1);
	if (distanceGauge) {
		distanceGauge->setup(DIST);
		viewList.addObj(distanceGauge);
	}
	barometerGauge = new NMEABox(BOROMETER_RECT,"InHg",2);
	if (barometerGauge) {
		barometerGauge->setup(BARO);
		viewList.addObj(barometerGauge);
	}
}


void navDisp::showPos(globalPos* fix) {

	char		outStr[40];
	double	value;
	char		qStr[3];
	DateTime	timeStamp(ourGPS->year,ourGPS->month,ourGPS->day,ourGPS->hours,ourGPS->min,ourGPS->sec);
	TimeSpan	deltaTime(0,ourNavApp.hoursOffUTC,0,0);
	
	if (ourGPS->valid) {
		fixLED->setState(true);
		timeStamp = timeStamp + deltaTime;
		sprintf(outStr,"%02d/%02d/%4d  %02d:%02d",
		timeStamp.month(),
		timeStamp.day(),
		timeStamp.year(),
		timeStamp.hour(),
		timeStamp.minute());
		if (strcmp(savedStamp,outStr)) {
			timeLabel->setValue(outStr);
			heapStr(&savedStamp,outStr);
		}
		
		strcpy(qStr," N");
		if (fix->getLatQuad()==south) {
			strcpy(qStr," S");
		}
		value = fix->getLatAsDbl();
		if (value<0) value = -value;
		sprintf (outStr,"%*f%s",10,value,qStr);
		latLabel->setValue(outStr);
		
		strcpy(qStr," W");
		if (fix->getLonQuad()==east) {
			strcpy(qStr," E");
		}
		value = fix->getLonAsDbl();
		if (value<0) value = -value;
		sprintf (outStr,"%*f%s",10,value,qStr);
		lonLabel->setValue(outStr);
	} else {
		fixLED->setState(false);
		sprintf(outStr,"--/--/----  --:--");
		timeLabel->setValue(outStr);
		sprintf (outStr,"---.------");
		latLabel->setValue(outStr);
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



// ************* erasableText *************


erasableText::erasableText(void)
	: fontLabel() { }
	
erasableText::erasableText(rect* inRect)
	: fontLabel(inRect) { }
	
	
erasableText::erasableText(int inX, int inY, int inW,int inH)
	: fontLabel(inX,inY,inW,inH) { }	
	
	
erasableText::~erasableText(void) { }

	
void erasableText::drawSelf(void) {
	
	rect	aRect(this);
	
	aRect.width = aRect.width+8;
	screen->fillRect(&aRect,&backColor);
	//screen->drawRect(&aRect,&green);			// GREEN
	fontLabel::drawSelf();
}



// *************  NMEABox  *************


NMEABox::NMEABox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec)
	: valueBox(inX,inY,inWidth,inHeight,inLabel,inPrec) {  }

	
NMEABox::~NMEABox(void) {  }


float NMEABox::checkData(void) {
  
	float	value;
	
	switch(dataChoice) {
		case RPM_VAL	: value = ourNavApp.engHdler->RPM;			break;
		case FUEL		: value = ourNavApp.fuelGauge->level;		break;
		case SPEED		: 
			value = ourNavApp.knotMeter->knots;
			if (value>99||value<0) {
				value = NAN;
			}
		break;
		case DEPTH		: 
			value = ourNavApp.depthSounder->feet/6.0;
			if (value>99||value<0) {
				value = NAN;
			}
		break;
		case BARO		: 
			value = baroSmoother.addData(ourNavApp.barometer->inHg);
			if (value>33||value<20) {
				value = NAN;
			}
		break;
		default		: value = NAN;
	}
	return value;
}


// *************  GPSBox  *************


GPSBox::GPSBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec)
	: valueBox(inX,inY,inWidth,inHeight,inLabel,inPrec) {  }

	
GPSBox::~GPSBox(void) {  }


float GPSBox::checkData(void) {
  
	float	value;
	
	value = NAN;
	if (ourGPS->valid) {
		switch(dataChoice) {
			case BEARING	: value = ourNavApp.bearingMark(true);	break;
			case DIST		: value = ourNavApp.distance();			break;
			default			: value = NAN;									break;
		}
	}
	return value;
}


		
// ************* valueBox *************	


valueBox::valueBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec)
	: drawGroup(inX,inY,inWidth,inHeight) {
	
	label	= NULL;
	heapStr(&label,inLabel);				// Save off copy of the units label.
	prec = inPrec;								// We'll need this later.
	factor = pow(10,inPrec);				// Calculate the multiplication factor.
	isNanNow = true;							// We'll start at NAN. Because  we really have no value.
	updateTimer = new timeObj(1000);		// refresh once a second as default.
	dataChoice = 0;
}
	
	
valueBox::~valueBox(void) {

	if (label)	freeStr(&label);
	if (updateTimer) delete(updateTimer);
}


void valueBox::setup(int inDataChoice) {

	dataChoice = inDataChoice;
	valueLabel = new erasableText();
	if (valueLabel) {
		valueLabel->setFont(AFF_SANS_BOLD_24_OB);
		valueLabel->x = 5;
		valueLabel->y = 10;
		valueLabel->width = 120;
		valueLabel->setColors(&yellow,&black);
		valueLabel->setPrecision(prec);
		valueLabel->setJustify(TEXT_RIGHT);
		valueLabel->setValue("-- ");
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
		freeStr(&label);
		addObj(unitsLabel);
	}
	hookup();
}


void valueBox::drawSelf(void) { /*screen->drawRect(this,&blue);*/ }


void valueBox::setValue(float value) {
	
	int	newIntVal;
	
	if (isnan(value)&&isNanNow) return;					// If we got a nan, it's already showing a nan.. Bail.
	else if (isnan(value)) {								// Else if we got a NAN and it's showing a value..
		valueLabel->setValue("-- ");						// Set label to dashes.
		isNanNow = true;										// We are NOW showing NAN.
	} else if (!isnan(value)&&isNanNow) {				// Else if we got a value and it's showing a NAN..
		savedIntVal = round(value * factor);			// Setup a integer version of the value.
		valueLabel->setValue(savedIntVal/factor);		// Set the new value to the screen.
		isNanNow = false;										// And we are no longer showing a NAN.
	} else {														// Else.. 
		newIntVal = round(value * factor);				// Setup a integer version of the value.
		if (newIntVal!=savedIntVal) {						// If it's different than the saved integer of what we have now..
			valueLabel->setValue(newIntVal/factor);	// Set the new value to the screen.
			savedIntVal = newIntVal;
		}
	}
}


void valueBox::idle(void) {

	float	value;
	
	drawGroup::idle();			// Let our parent have a go.
	if (updateTimer->ding()) {	// If our timer goes off..
		value = checkData();		// Grab fresh data value.
		setValue(value);			// Update the display.
		updateTimer->start();	// Reset the timer.
	}
}





	

