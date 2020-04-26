#ifndef editable_h
#define editable_h

#include "keystroke.h"

class editable {

	public :
						editable(void);
	virtual			~editable(void);
	
	
	virtual	void	setExitOnEnter(bool trueFalse);
	virtual	void	handleKeystroke(keystroke* inKey);
	virtual	void	beginEditing(void);
	virtual	void	handleInputKey(void);
	virtual	void	handleBackspaceKey(void);
	virtual	void	handleArrowFWDKey(void);
	virtual	void	handleArrowBackKey(void);
	virtual	void	handleEnterKey(void);
	virtual	void	handleCancelKey(void);
	virtual	void	handleOkKey(void);
	virtual	void	endEditing(void);
	
				char	getCurrentChar(void);
				bool	getEditing(void);
				bool	getSuccess(void);
				bool	getExitOnEnter(void);
	
				char	mCurrentChar;
				bool	mEditing;
				bool	mSuccess;
				bool	mExitOnEnter;
};


#endif