#ifndef scrKeyboard_h
#define scrKeyboard_h

#include <label.h>
#include <keystroke.h>
#include <editable.h>
#include <idlers.h>




// Keys need to know the state of the keyboard. When they get time
// they'll check the keyboard to see what state things are in and
// redraw themselves accordingly. Also output their data accordingly.


enum  keyStates { chars, shifted, numbers, symbols };

class keyboard;

struct keyColors {
	colorObj		inputKeyText;
	colorObj		inputKeyBase;
	colorObj		inputKeyHText;
	colorObj		inputKeyHBase;
	
	colorObj		contolKeyText;
	colorObj		contolKeyBase;
	colorObj		contolKeyHText;
	colorObj		contolKeyHBase;
	
	colorObj		deleteKeyText;
	colorObj		deleteKeyBase;
	colorObj		deleteKeyHText;
	colorObj		deleteKeyHBase;
};

extern keyColors kbPallette;


class keyboardKey : public idler {

	public:
						keyboardKey(keyboard* inKeyboard);
	virtual			~keyboardKey(void);
	
	virtual	void	beenClicked(void);
	
				keyboard*   mKeyboard;  // Master!
				keyStates   mState;     // Current state I'm showing.
				char        mChar;
				char        mNum;
				char        mSymbol;
				keyCommands mCom;
};


// inputKeys are for inputting data. Like letters & numbers.
class inputKey : public keyboardKey, public label {

  public:
          inputKey(char* inLabel,char* inNum,char* inSym,word locX, word locY,byte width,byte height,keyboard* inKeyboard);
  virtual ~inputKey(void);

  virtual void    idle();     // Use this to keep updated.
  virtual void    drawSelf(void);
  virtual void    doAction(void);
};


// controlKeys are for doing everything else. Like shift, delete and arrow keys.
class controlKey : public keyboardKey, public label {

  public:
          controlKey(char* inLabel,keyCommands inCom,word locX, word locY,byte width,byte height,keyboard* inKeyboard);
  virtual ~controlKey(void);

  virtual void    drawSelf(void);
  virtual void    handleShift(void); 
  virtual void    handleNumber(void);
  virtual void    doAction(void);
};


class keyboard : public drawGroup {

	public:
				keyboard(editable* inEditObj=NULL);
	virtual	~keyboard(void);

	virtual	void			loadKeys(void);
	virtual	void			keyClicked(keyboardKey* aKey);
	virtual	void			handleKey(char inChar);
	virtual	void			handleKey(keyCommands inEditCom);
	virtual	void			handleKey(keyStates inState);
	virtual	void			setEditField(editable* inLabel);
	virtual	keyStates	getState(void);
	virtual	void    		drawSelf(void);
				int			globalY(void);
			
          editable*		mEditObj;
          keyStates		mState;
          int				mGlobalY;
}; 


#endif
