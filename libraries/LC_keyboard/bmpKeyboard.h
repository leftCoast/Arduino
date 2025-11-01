#ifndef bmpKeyboard_h
#define bmpKeyboard_h

#include <scrKeyboard.h>
#include <bmpObj.h>
#include <bitmap.h>
#include <bmpObj.h>


// *****************************************************
//                       bmpKeybaard
// *****************************************************


class bmpKeyboard : public keyboard {

	public:
				bmpKeyboard(editable* inEditObj,bool modal=false);
	virtual	~bmpKeyboard(void);

	virtual	void	loadKeys(void);
  				
				int			col(int col, int row);
				bitmap*		getKeyMap(void);
				colorObj*	getKeyTextColor(void);
  
  protected:
          bool    	mModal;
          bitmap		mKeyBMap;
          colorObj	mKeyTextColor;
};



// *****************************************************
//                       bmpInputKey
// *****************************************************


class bmpInputKey :  public inputKey {

	public:
				bmpInputKey(const char* inLabel,const char* inNum,const char* inSym,int inX,int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard);
	virtual	~bmpInputKey(void);

	virtual	void	drawSelf(void);
  
				bitmap*		mKeyBMap;
};



// *****************************************************
//                       bmpControlKey
// *****************************************************


class bmpControlKey : public controlKey {

  public:
          bmpControlKey(const char* inLabel,keyCommands inCom,int inX, int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard,const char* bmpPath);
  virtual ~bmpControlKey(void);

  virtual void    drawSelf(void);
          
          bmpObj*	ourBMPObj;
};


#endif
