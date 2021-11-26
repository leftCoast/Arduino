#ifndef alertBox_h
#define alertBox_h

#include <lilOS.h>
#include <label.h>



// **************************************************************
// ************************ OKBtn stuff *************************
// **************************************************************


class OKBtn :	public iconButton {

	public:
				OKBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~OKBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};



// **************************************************************
// ********************** cancelBtn stuff ***********************
// **************************************************************


class cancelBtn :	public iconButton {

	public:
				cancelBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~cancelBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};



// **************************************************************
// ********************** alertBox  stuff ***********************
// **************************************************************



class alertBox :	public modal {

	public :
				alertBox(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~alertBox(void);
};

