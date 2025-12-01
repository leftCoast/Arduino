#ifndef modalKbd_h
#define modalKbd_h

#include <bmpKeyboard.h>
#include <stdComs.h>
#include <modalMgr.h>
#include <editLabel.h>


// **************************************************************
// *********************** modalKbd stuff ***********************
// **************************************************************


class modalKbd :	public bmpKeyboard {

	public :
				modalKbd(void);
	virtual	~modalKbd(void);
	
	virtual	void			setListener(listener* inListener);
	virtual	listener*	getListener(void);
	virtual	void			handleKey(keyCommands inEditCom);
	virtual	void			drawSelf(void);
	
	private :
				listener*	ourListener;
};



// **************************************************************
// *********************** kbdUser  stuff ***********************
// **************************************************************

	
class kbdUser {

	public :
				kbdUser(listener* inListener);
	virtual	~kbdUser(void);
	
				void			setEditField(editable* inEditable);
				void			checkKbdEvent(event* inEvent,point* globalPt);
				
				listener*	lastListener;
				editable*	lastEditObj;
};

#endif