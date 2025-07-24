#include <navDisp.h>


navDisp::navDisp (void) { }


navDisp::~navDisp(void) {  }


void navDisp::setup(void) {

	
	screen->fillScreen(&black);
	latLabel = new label(60,40,200,16);
	latLabel->setColors(&white,&black);
	viewList.addObj(latLabel);
	lonLabel = new label(60,60,200,16);
	lonLabel->setColors(&white,&black);
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

	

