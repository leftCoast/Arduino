#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>
#include <iconButton.h>
#include  <iconEditScr.h>
#include <fOpenObj.h>
#include <fSaveObj.h>


class iconEdit;


class newFileBtn : public iconButton22 {

	public :
					newFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp);
	virtual		~newFileBtn(void);					

	virtual		void    doAction(void);

					iconEdit*	ourApp;
};


class openFileBtn : public iconButton22 {

	public :
					openFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp);
	virtual		~openFileBtn(void);					

	virtual		void    doAction(void);

					iconEdit*	ourApp;
};


class saveFileBtn : public iconButton22 {

	public :
					saveFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp);
	virtual		~saveFileBtn(void);					

	virtual		void    doAction(void);

					iconEdit*	ourApp;
};


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
	
				iconEditScr*	theEditScr;
				fOpenObj*		openDBox;
				fSaveObj*		saveDBox;
};

#endif
