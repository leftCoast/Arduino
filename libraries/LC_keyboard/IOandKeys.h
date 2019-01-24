#ifndef IOandKeys_h
#define IOandKeys_h

#include <scrKeyboard.h>
#include <keystroke.h>
#include <editField.h>
#include <textView.h>


class IOandKeys : public keyboard {

	public:
				IOandKeys(editField* inEditField,textView* inTextField);
				~IOandKeys(void);

	virtual	void	handleKey(keyCommands inEditCom);
				int	haveBuff(void);
				bool	getBuff(char* buff,int maxBytes);
				bool	resizeOutBuff(byte numBytes);
  
          textView* mTextField;
          bool		mHaveBuff;
          char*	mOutBuff;   // The out going message.
          byte		mNumBytes;  // How many bites is it now?
};

#endif
