#ifndef navTest_h
#define navTest_h

#include <lilOS.h>
#include <lilParser.h>

class navTest	:  public panel {

	public:
				navTest(int ourAppID);
				~navTest(void);
		
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	closing(void);
	
				void doAddWP(void);
				void doEditWP(void);
				void doListWP(void);
				void doDeteWP(void);
				void printHelp(void);
				
				lilParser*	cmdParser;
};

#endif