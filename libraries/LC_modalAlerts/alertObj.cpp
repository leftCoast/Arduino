#include <alertObj.h>
#include <textView.h>
#include <lilOS.h>

//#include <debug.h>

#define	ALERT_X		20
#define	ALERT_Y		40
#define	ALERT_W		240 - 2 * ALERT_X
#define	ALERT_H		80

#define	MSG_X			45
#define	MSG_Y			15
#define	MSG_W			ALERT_W - MSG_X - 10
#define	MSG_H			20

#define	OK_X			ALERT_W - 40
#define	OK_Y			ALERT_H - 40

#define	CANCEL_X		OK_X - 40
#define	CANCEL_Y		OK_Y

#define	LABEL_X		8
#define	LABEL_Y		ALERT_H/2 - 16


alertObj::alertObj(const char* msg,listener* inListener,alertType inType,bool useOk,bool useCancel)
	: modal(ALERT_X,ALERT_Y,ALERT_W,ALERT_H) { 
	
	bmpObj*	ourLabel;
	
	ourLabel = NULL;
	ourListener = inListener;
	theMsg = new textView(MSG_X,MSG_Y,MSG_W,MSG_H);
	if (theMsg) {
		theMsg->setTextColors(&black,&white);
		theMsg->setText(msg);
		addObj(theMsg);
	}
	if (useOk) {
		okBtn = newStdBtn(OK_X,OK_Y,icon32,okCmd,this);
		if (okBtn) {
			addObj(okBtn);
		}
	} else {
		okBtn = NULL;
	}
	if (useCancel) {
		cancelBtn = newStdBtn(CANCEL_X,CANCEL_Y,icon32,cancelCmd,this);
		if (cancelBtn) {
			addObj(cancelBtn);
		} 
	} else {
		cancelBtn = NULL;
	}
	switch(inType) {
		case noteAlert		: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,noteLbl);		break;
		case choiceAlert	: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,choiceLbl);	break;
		case warnAlert		: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,warnLbl);		break;
		case noIconAlert	: ourLabel = NULL; break;
	}
	if (ourLabel) {
		addObj(ourLabel);
	}
}

	
alertObj::~alertObj(void) { ourPanel->aTouchAbove(); }


// User wants the message changed. What - ever!
void alertObj::setMsg(const char* inMessage) { theMsg->setText(inMessage); }


// Is this event for us? Well yes, they ALL are. Except if we have a keyboard..	
bool alertObj::acceptEvent(event* inEvent,point* locaPt) {

	if (drawGroup::acceptEvent(inEvent,locaPt)) {			// If its actually ours..
		return true;	 												// We return true.
	}
	return true;														// In all other cases, the buck stops here.
}

	
void alertObj::drawSelf(void) {
	
	x++;										// Quick move it over one click in x.
	y++;										// One click in y.
	screen->drawRect(this,&black);	// One pixal rectangle makes our drop shadow.
	x--;										// Reset x.
	y--;										// Reset y.
	screen->fillRect(this,&white);	// Fill our rectangle white.
	screen->drawRect(this,&black);	// And draw black outline.
}


// We handle ok & cancel. If we have a listener, we'll pass these on.	
void alertObj::handleCom(stdComs comID) {

	switch(comID) {
		case okCmd		:  							//setSuccess(true); break;
		case cancelCmd	: done = true; break;	// setSuccess(false); break;
		default			: break;						// Should never happen, let it slide.		
	}														//
	if (ourListener) {								// Got a listener?
		ourListener->handleCom(comID);			// Pass it on.
	}
}


