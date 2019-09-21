#ifndef IOandKeys_h
#define IOandKeys_h

#include <scrKeyboard.h>
#include <keystroke.h>
#include <editField.h>
#include <textView.h>


// This is for running something like a terminal or a chat window. It links to a
// "send message" edit field and you hit return to send the message in that field "out".


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
