
#include "cellListener.h"	// Well, duh.
#include <cellManager.h>	// Currently where the status register is living.
#include "litlOS.h"			// Our job is to open panels. litlOS is the guy for this.
 
bool callIncoming;          


cellListener::cellListener(void)
  :idler() {
    
  callIncoming = false;
  answerID = -1;
}


cellListener::~cellListener(void) {  }


void cellListener::begin(int answerPanelID) {

	hookup();
	answerID = answerPanelID;
}


void cellListener::idle(void) {
	
	if (!callIncoming) {
		if (statusReg.callStat == CS_ringingIn) {
			callIncoming = true;
			nextPanel = answerID;
		}
	}  else {
		if(statusReg.callStat != CS_ringingIn) {
			callIncoming = false;
		}
	}
}


cellListener  ourListener;
