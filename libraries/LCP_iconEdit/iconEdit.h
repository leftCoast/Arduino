#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>
#include <stdComs.h>
#include  <iconEditScr.h>
#include <fOpenObj.h>
#include <fSaveObj.h>


class iconEdit	: public panel {

	public:
					iconEdit(lilOS* ourOS,int ourAppID);
	virtual		~iconEdit(void);

				void	beginFileNew(void);
				void	beginFileOpen(void);
				void	beginFileSave(void);
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	drawSelf(void);
	virtual	void	closing(void);
	virtual	void	handleCom(stdComs comID);
	
				iconEditScr*	theEditScr;
				fOpenObj*		openDBox;
				fSaveObj*		saveDBox;
};

#endif
