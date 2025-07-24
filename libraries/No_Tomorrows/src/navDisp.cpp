#include <navDisp.h>


navDisp::navDisp (void) { }


navDisp::~navDisp(void) {  }


void navDisp::setup(void) {

	
	screen->fillScreen(&black);
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
	
	strcpy(outStr,"Latitude  : ");
	strcat(outStr,fix->showLatStr());
	latLabel->setValue(outStr);
	
	strcpy(outStr,"Longitude : ");
	strcat(outStr,fix->showLonStr());
	lonLabel->setValue(outStr);
}


void navDisp::idle(void) { }

	

