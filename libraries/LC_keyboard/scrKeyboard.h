#ifndef scrKeyboard_h
#define scrKeyboard_h

#include <label.h>
#include <keystroke.h>
#include <editField.h>
#include <idlers.h>


#define KEY_WD  23
#define KEY_HT  23

#define COL_1   1
#define COL_SP  1

#define ROW_1   217
#define ROW_SP  4

// Keys need to know the state of the keyboard. When they get time
// they'll check the keyboard to see what state things are in and
// redraw themselves accordingly. also output their data accordingly.


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
  virtual ~keyboardKey(void);

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


class keyboard {

	public:
				keyboard(editField* inEditField);
	virtual	~keyboard(void);

	virtual	void			handleKey(char inChar);
	virtual	void			handleKey(keyCommands inEditCom);
	virtual	void			handleKey(keyStates inState);
	virtual	void			setEditField(editField* inField);
	virtual	keyStates	getState(void);
			
          editField*		mEditField;
          keyStates		mState;
          
          inputKey*   qKey;
          inputKey*   wKey;
          inputKey*   eKey;
          inputKey*   rKey;
          inputKey*   tKey;
          inputKey*   yKey;
          inputKey*   uKey;
          inputKey*   iKey;
          inputKey*   oKey;
          inputKey*   pKey;
          
          inputKey*   aKey;
          inputKey*   sKey;
          inputKey*   dKey;
          inputKey*   fKey;
          inputKey*   gKey;
          inputKey*   hKey;
          inputKey*   jKey;
          inputKey*   kKey;
          inputKey*   lKey;
          
          inputKey*   zKey;
          inputKey*   xKey;
          inputKey*   cKey;
          inputKey*   vKey;
          inputKey*   bKey;
          inputKey*   nKey;
          inputKey*   mKey;
          
          inputKey*   spcKey;
          
          controlKey* shiftKey;
          controlKey* backSpKey;
          controlKey* leftArrow;
          controlKey* rightArrow;
          controlKey* symbolKey;
          controlKey* enterKey;
}; 


#endif
