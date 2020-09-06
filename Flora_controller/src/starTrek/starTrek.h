#ifndef starTrek_h
#define starTrek_h

#include <panel.h>
#include <idlers.h>
#include <litlOS.h> 
#include <bmpObj.h>
#include <textView.h>
#include "../../tools.h"
#include "textBuff.h"
#include "arduinoTrek.h"

class starTrekPanel;

// *****************************************************
//                      starTrekKeyboard
// *****************************************************


class starTrekKeyboard : public ourKeyboard {

  public:

                starTrekKeyboard(editLabel* inEditLabel,textView* inTextView,bool modal);
  virtual       ~starTrekKeyboard(void);

  virtual void  handleMessage(char* buff);
  virtual void  handleKey(keyCommands inEditCom);

          textView*   mTextView;
          editLabel*  mEditLabel;         
};


// *****************************************************
//                      starTrekUpdater
// *****************************************************

class starTrekUpdater : public idler {

  public:
            starTrekUpdater(starTrekPanel* inPanel);
  virtual   ~starTrekUpdater(void);

  virtual  void  idle(void);

            starTrekPanel* mPanel;
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
          starTrekPanel(void);
  virtual ~starTrekPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
  virtual void  loop(void);
  virtual void  closing(void);
  
          starTrekText*    ourScreen;
          starTrekUpdater*	mUpdater;
};

#endif
