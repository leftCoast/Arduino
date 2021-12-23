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
	
	virtual	void	setListener(listener* inListener);
	virtual	void	handleKey(keyCommands inEditCom);
	virtual	void	drawSelf(void);
	
				listener*	ourListener;
};



// **************************************************************
// *********************** kbdUser  stuff ***********************
// **************************************************************

	
class kbdUser {

	public :
				kbdUser(void);
	virtual	~kbdUser(void);

				void	setListener(listener* inListener);				
				void	setEditField(editLabel* inEditable);

				
				editable*	lastEditObj;
};

#endif