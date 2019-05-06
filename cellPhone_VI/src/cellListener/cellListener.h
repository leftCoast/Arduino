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
				void	decodeSMS(void);
				
				int	answerID;
				int	mSMSID;
 			 	int	mSMSIndex;
 			 	bool	mFirstRing;
 			 	int	mStatNum;
};

extern bool				callIncoming;
extern bool				newSMSmsg;				// This is for the current text msg screen to see & clear.
extern char				SMSPN[];
extern char				SMSMsg[];
extern char				SMSRaw[];
extern cellListener	ourListener;

#endif
