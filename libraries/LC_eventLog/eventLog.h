#ifndef eventLog_h
#define eventLog_h


#include "timeObj.h"
#include	"idlers.h"


class eventLog	:	public idler {

	public:
				eventLog(void);
	virtual	~eventLog(void);
	
	virtual	bool	begin(char* path,int series=0);
	virtual	void	setLogging(bool offOn);
	virtual	bool	getLogging(void);
	virtual	void	addEvent(char* eventTxt);
	virtual	void	deleteFile(void);
	virtual	void	idle(void);
	
	private:
				char*		mPath;
				int		mSeries;
				bool		mReady;
				bool		mLogging;
};

#endif
				
				
	
				