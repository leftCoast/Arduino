#ifndef alertObj_h
#define alertObj_h

#include <textView.h>
#include <modalMgr.h>
#include <stdComs.h>

// Your base modal Alert object. This is something that is modal and handles ok &  cancel
// commands. It will also pass these commands to its listener, if it has one. How would I
// use this? I'd inherit it and have the dialog box do the action its talking about. If at
// all possible.


enum alertType { noteAlert, choiceAlert, warnAlert, noIconAlert };

class alertObj :	public modal,
						public listener {
	
	public :
				alertObj(const char* msg,listener* inListener,alertType inType,bool useOk=true,bool useCancel=false);
	virtual	~alertObj(void);
	
				void	setMsg(const char* inMessage);
	virtual	bool	acceptEvent(event* inEvent,point* locaPt);
	virtual	void	drawSelf(void);
	virtual	void	handleCom(stdComs comID);
	
				listener*	ourListener;
				textView*	theMsg;
				stdComBtn*	okBtn;
				stdComBtn*	cancelBtn;
};

#endif
	
	