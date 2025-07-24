#include <navDisp.h>
#include <GPSReader.h>

navDisp::navDisp (void) { }


navDisp::~navDisp(void) {  }


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
	
	
}

void navDisp::showPos(globalPos* fix) {

	char	outStr[40];
	char	numStr[20];
	
	strcpy(outStr,"GMT ");
	sprintf(numStr,"%d",fixData.hours);
	strcat(outStr,numStr);
	strcat(outStr,":");
	sprintf(numStr,"%d",fixData.min);
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


void navDisp::idle(void) { }

	

