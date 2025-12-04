#ifndef starTrek_h
#define starTrek_h

#include <idlers.h>
#include <lilOS.h> 
#include <bmpObj.h>
#include <textView.h>
#include <lilParser.h>
#include "textBuff.h"
#include "bmpKeyboard.h"
#include "editLabel.h"
#include "arduinoTrek.h"

class starTrekPanel;

// *****************************************************
//                      starTrekKeyboard
// *****************************************************


class starTrekKeyboard : public bmpKeyboard {

	public:
				starTrekKeyboard(starTrekPanel* inPanel,editLabel* inEditLabel,textView* inTextView,bool modal);
	virtual	~starTrekKeyboard(void);

	virtual	void	keyClicked(keyboardKey* aKey);
	virtual	void	handleMessage(char* buff);
	virtual	void	handleKey(keyCommands inEditCom);

				starTrekPanel*	mPanel;
				textView*   	mTextView;
				editLabel*  	mEditLabel;         
};


// *****************************************************
//                      starTrekUpdater
// *****************************************************

class starTrekUpdater : public idler {

  public:
				enum CSI { noCSI, clearScr };
  
				starTrekUpdater(starTrekPanel* inPanel);
  virtual   ~starTrekUpdater(void);

				void	parseChar(char aChar);
  virtual	void  idle(void);

            starTrekPanel* mPanel;
            lilParser   	mParser;
            bool				mParsing;
};


// *****************************************************
//                      starTrekText
// *****************************************************


class starTrekText :	public textView {

	public:
          	starTrekText(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet=noEvents);
	virtual	~starTrekText(void);
  
  	virtual	void	appendText(char* text);
	virtual	void	appendText(char aChar);
};



// *****************************************************
//                      starTrekPanel
// *****************************************************


class starTrekPanel :  public panel {

  public:
          starTrekPanel(int ourAppID);
  virtual ~starTrekPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  loop(void);
  virtual void  closing(void);
  
          starTrekText*    ourScreen;
          starTrekUpdater*	mUpdater;
};

#endif
