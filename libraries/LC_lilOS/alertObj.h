#ifndef alertObj_h
#define alertObj_h

#include <lilOS.h>

enum alertType { noteAlert, choiceAlert, warnAlert };

class alertObj :	public modal {
	
	public :
				alertObj(char* msg,listener* inListener,alertType inType,bool useOk=true,bool useCancel=false);
	virtual	~alertObj(void);
	
	virtual	void	drawSelf(void);
	
				listener*	ourListener;
				stdComBtn*	okBtn;			// Save these two just in case some one needs to
				stdComBtn*	cancelBtn;		// muck with 'em.
};

#endif
	
	