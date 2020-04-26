#ifndef datafield_h
#define datafield_h

#include <drawObj.h>
#include <scrKeyboard.h>
#include <editLabel.h>

// The idea of a datafield is an editing field that can A) Have a background drawObject
// and B) Optionally live on a panel/screen with multiple other datafields sharing a
// single keyboard. And of course, doing all the correct editing things.
//
// Sharing the keyboard with other datafields uses the focus functionality given to us by
// drawObj. Thie idea is that when we gain or loose focus, we can do the "right thing"
// managing our given editing field, turning it on/off.



class datafield :	public drawGroup {

	public:
					datafield(int x,int y,int w,int h); 
	virtual		~datafield(void);
	
	virtual	void	begin(keyboard* inKeyboard,editLabel* inEditLabel,drawObj* background=NULL);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool setLoose);
	virtual	void	idle(void);
	
				keyboard*	mKeyboard;
				editLabel*	mEditField;
				eventSet		mEditEvents;
};

#endif