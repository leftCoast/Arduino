#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>
#include <iconButton.h>
#include  <iconEditScr.h>


class saveBtn : public iconButton22 {

	public :
					saveBtn(int xLoc, int yLoc,char* path,iconEditScr* editScr);
	virtual		~saveBtn(void);					

virtual void    doAction(void);

					iconEditScr*	theEditScr;
};


class iconEdit	: public panel {

	public:
					iconEdit(lilOS* ourOS,int ourAppID);
	virtual		~iconEdit(void);
	
	virtual	void  		setup(void);
	virtual	void  		loop(void);
	virtual	void  		drawSelf(void);
	virtual	void  		closing(void);

				iconEditScr*	theEditScr;
};

#endif
