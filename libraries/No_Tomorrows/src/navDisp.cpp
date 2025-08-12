#include <strTools.h>
#include <navDisp.h>
#include <GPSReader.h>
#include <navigation.h>

//#include <fonts/FreeSansOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique24pt7b.h>
#include <fonts/FreeSansBoldOblique12pt7b.h>


#define SPEEDBOX_RECT			40,100,240,64
#define DEPTHBOX_RECT			40,175,240,64
#define BEARINGBOX_RECT			40,250,240,64
#define BOROMETERBOX_RECT		40,325,240,64

#define AFF_SANS_BOLD_24_OB	&FreeSansBoldOblique24pt7b,45,-12
#define AFF_SANS_BOLD_12_OB	&FreeSansBoldOblique12pt7b,24,-6


navDisp::navDisp (void) { updateTimer = new timeObj(5000); }


navDisp::~navDisp(void) { if (updateTimer) delete(updateTimer); }


void navDisp::setup(void) {

	screen->fillScreen(&black);
	
	timeLabel = new label(125,20,200,32);
	timeLabel->setColors(&white,&black);
	timeLabel->setTextSize(1);
	viewList.addObj(timeLabel);
	
	latLabel = new label(20,40,300,32);
	latLabel->setColors(&white,&black);
	latLabel->setTextSize(1);
	viewList.addObj(latLabel);
	
	lonLabel = new label(20,80,300,32);
	lonLabel->setColors(&white,&black);
	lonLabel->setTextSize(1);
	viewList.addObj(lonLabel);
	
	knotGauge = new valueBox(SPEEDBOX_RECT,"Knots",1);
	if (knotGauge) {
		knotGauge->setup();
		viewList.addObj(knotGauge);
	}
	
	depthGauge = new valueBox(DEPTHBOX_RECT,"Fathoms",1);
	if (depthGauge) {
		depthGauge->setup();
		viewList.addObj(depthGauge);
	}
	
	bearingGauge = new valueBox(BEARINGBOX_RECT,"Deg. M",0);
	if (bearingGauge) {
		bearingGauge->setup((float*)&(ourNavApp.bearingVal));
		viewList.addObj(bearingGauge);
	}
	
	barometerGauge = new valueBox(BOROMETERBOX_RECT,"In Hg",0);
	if (barometerGauge) {
		barometerGauge->setup();
		viewList.addObj(barometerGauge);
	}
}


void navDisp::showPos(globalPos* fix) {

	char	outStr[40];
	char	numStr[20];
	
	strcpy(outStr,"GMT ");
	sprintf(numStr,"%d",ourGPS->hours);
	strcat(outStr,numStr);
	strcat(outStr,":");
	sprintf(numStr,"%d",ourGPS->min);
	strcat(outStr,numStr);
	//strcat(outStr,":");
	//sprintf(numStr,"%d",round(fixData.sec));
	//strcat(outStr,numStr);
	timeLabel->setValue(outStr);
	
	strcpy(outStr,"Latitude  : ");
	strcat(outStr,fix->showLatStr());
	latLabel->setValue(outStr);
	
	strcpy(outStr,"Longitude : ");
	strcat(outStr,fix->showLonStr());
	lonLabel->setValue(outStr);
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
	
	aRect.width = aRect.width+6;
	screen->fillRect(&aRect,&backColor);
	//screen->drawRect(&aRect,&green);
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
		valueLabel->x = 15;
		valueLabel->y = 10;
		valueLabel->width = 100;
		valueLabel->setColors(&yellow,&black);
		valueLabel->setPrecision(prec);
		valueLabel->setJustify(TEXT_RIGHT);
		valueLabel->setValue("--");
		addObj(valueLabel);
	}
	unitsLabel = new fontLabel();
	if (unitsLabel) {
		unitsLabel->setFont(AFF_SANS_BOLD_12_OB);
		unitsLabel->x = 140;
		unitsLabel->y = 24;
		unitsLabel->width = 80;
		unitsLabel->setColors(&yellow,&black);
		unitsLabel->setValue(label);
		addObj(unitsLabel);
	}
	hookup();
}


void valueBox::drawSelf(void) { }


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




	

