#ifndef cellListener_h
#define cellListener_h

#include <idlers.h>

// This is the guy that sits in the background watching status. It brings up the panel for
// answering incoming phone calls. It also pulls out new SMS messages for you to send to
// the user.

class cellListener :  public idler {

	public:
				cellListener(void);
	virtual	~cellListener(void);

				void	begin(int answerPanelID);
	virtual	void	idle(void);
				void	doSMS(void);
				
				int	answerID;
				int	mSMSID;
 			 	int	mSMSIndex;
};

extern bool				callIncoming;
extern bool				newSMSmsg;		// HEY! Clear this once you've read the SMS message.
extern char*			SMSMsg;
extern cellListener	ourListener;

#endif
