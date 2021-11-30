#ifndef alertObj_h
#define alertObj_h

#include <lilOS.h>

enum alertType { noteAlert, choiceAlert, WarnAlert };

class alertObj :	public modal {
	
	public :
				alertObj(char* msg,listener* inListener,alertType inType, bool useCancel);
	virtual	~alertObj(void);
	
	virtual	void	drawSelf(void);
	
				listener*	ourListener;
};

#endif
	
	