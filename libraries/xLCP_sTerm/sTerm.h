#ifndef sTerm_h
#define sTerm_h

#include <lilOS.h> 
#include <bmpObj.h>
#include <textView.h>
#include "bmpKeyboard.h"
#include "editLabel.h"

class sTerm;

// *****************************************************
//                      sTermKeyboard
// *****************************************************


class sTermKeyboard : public bmpKeyboard {

	public:
				sTermKeyboard(sTerm* inPanel,editLabel* inEditLabel,textView* inTextView,bool modal);
	virtual	~sTermKeyboard(void);

	virtual	void	keyClicked(keyboardKey* aKey);
	virtual	void	handleMessage(const char* buff);
	virtual	void	handleKey(keyCommands inEditCom);

				sTerm*	mPanel;
				textView*   	mTextView;
				editLabel*  	mEditLabel;         
};


// *****************************************************
//                      sTermText
// *****************************************************


class sTermText :	public textView {

	public:
          	sTermText(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet=noEvents);
	virtual	~sTermText(void);
  
  	virtual	void	appendText(const char* text);
	virtual	void	appendText(char aChar);
};


// *****************************************************
//                      sTerm
// *****************************************************


class sTerm :  public panel {

	public:
				sTerm(int ourAppID);
	virtual	~sTerm(void);
          
	virtual	void	setup(void);
				void	setStream(Stream* inStream=&Serial,Stream* outStream=&Serial);
	virtual	void	drawSelf(void);
	//virtual	void	close(void);
	virtual	void	loop(void);
	virtual	void	closing(void);
  
          	Stream*			ourInStream;
          	Stream*			ourOutStream;
          	sTermText*		ourScreen;
          	//sTermUpdater*	mUpdater;
};

#endif
