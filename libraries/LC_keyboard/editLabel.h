#ifndef editLabel_h
#define editLabel_h

#include <label.h>
#include <editable.h>
#include <idlers.h>
#include <timeObj.h>
#include <keystroke.h>

#define	CURSOR_BLINK	500

class editLabel : public label,
						public editable,
                  public idler,
                  public timeObj {

	public:
				editLabel();
  				editLabel(const char* inText);
  				editLabel(const char* inText, int inSize);
  				editLabel(int inLocX, int inLocY, int inWidth,int inHeight);
  				editLabel(int inLocX, int inLocY, int inWidth,int inHeight,const char* inText);
  				editLabel(int inLocX, int inLocY, int inWidth,int inHeight,const char* inText,int textSize);
  				editLabel(rect* inRect,const char* inText,int textSize=1);
  				editLabel(label* aLabel);
	virtual	~editLabel(void);

				void	init(void);
	virtual	void	setIndex(int newIndex);
	virtual	int	getIndex(void);
	
	virtual	void	beginEditing(void);
	virtual	void	handleInputKey(void);
	virtual	void	handleBackspaceKey(void);
	virtual	void	handleArrowFWDKey(void);
	virtual	void	handleArrowBackKey(void);
	virtual	void	endEditing(void);
	
  	virtual	void  doAction(event* inEvent,point* locaPt);
	virtual	void	drawSelf(void);
	virtual	void	setInitalPointers(void);
	virtual	void	showText(void);
	virtual	void	idle(void);
				
				char*	editBuff;
				char*	backupBuff;
				char* tempBuff;
				int	index;
				int	firstChar;
				int	cursor;
				bool	cursorOnOff;
				
};

#endif
