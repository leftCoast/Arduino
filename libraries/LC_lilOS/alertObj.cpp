#include <alertObj.h>
#include <label.h>


#define	ALERT_X		20
#define	ALERT_Y		40
#define	ALERT_W		240 - 2 * ALERT_X
#define	ALERT_H		80

#define	MSG_X			10
#define	MSG_Y			20
#define	MSG_W			ALERT_W - 2 * MSG_X
#define	MSG_H			20

#define	OK_X			ALERT_W - 40
#define	OK_Y			ALERT_H - 40

#define	CANCEL_X		OK_X - 40
#define	CANCEL_Y		OK_Y

#define	LABEL_X		8
#define	LABEL_Y		ALERT_H/2 - 16


alertObj::alertObj(char* msg,listener* inListener,alertType inType, bool useCancel)
	: modal(ALERT_X,ALERT_Y,ALERT_W,ALERT_H) { 
	
	bmpObj*	ourLabel;
	
	ourListener = inListener;
	label* theMsg = new label(MSG_X,MSG_Y,MSG_W,MSG_H,msg);
	if (theMsg) {
		theMsg->setJustify(TEXT_CENTER);
		addObj(theMsg);
	}
	stdComBtn* okBtn = newStdBtn(OK_X,OK_Y,icon32,okCmd,ourListener);
	if (okBtn) {
		addObj(okBtn);
	}
	stdComBtn* cancelBtn = newStdBtn(CANCEL_X,CANCEL_Y,icon32,cancelCmd,ourListener);
	if (cancelBtn) {
		addObj(cancelBtn);
	}
	switch(inType) {
		case noteAlert		: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,noteLbl);		break;
		case choiceAlert	: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,choiceLbl);	break;
		case WarnAlert		: ourLabel = newStdLbl(LABEL_X,LABEL_Y,icon32,warnLbl);		break;
	}
	if (ourLabel) {
		addObj(ourLabel);
	}
}

	
alertObj::~alertObj(void) {  }

	
void alertObj::drawSelf(void) {
	
	x++;										// Quick move it over one click in x.
	y++;										// One click in y.
	screen->drawRect(this,&black);	// One pixal rectangle makes our drop shadow.
	x--;										// Reset x.
	y--;										// Reset y.
	screen->fillRect(this,&white);	// Fill our rectangle white.
	screen->drawRect(this,&black);	// And draw  black outline for us.
}
	
