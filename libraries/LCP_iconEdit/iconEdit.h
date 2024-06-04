#ifndef iconEdit_h
#define iconEdit_h

#include <lilOS.h>
#include	<iconEditScr.h>
#include <documentPanel.h>
#include <colorPicker.h>
#include <colorRect.h>


#define ICON_FLDR	"/EIcons/"

class iconEdit;

// **************************************************************
// **********************    colorBtnObj    *********************
// **************************************************************


class colorBtnObj :	public iconButton {

	public:
				colorBtnObj(iconEdit* inEditor);
	virtual	~colorBtnObj(void);
	
	virtual	void	doAction(void);
	
				iconEdit* ourEditor;
};



// **************************************************************
// ************************    iconEdit    **********************
// **************************************************************
	
	
class iconEdit	:	public documentPanel {

	public:
	
	enum iEditStates { editing, getColor };
	 				
				iconEdit(int ourAppID);
	virtual	~iconEdit(void);
	
	virtual	void	createDocObj(void);
	virtual	bool	createNewDocFile(void);
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void	loop(void);
	virtual	char* getLocalFilePath(const char* fileName);	// This will be VERY handy later..
				void	openColorPicker(void);
				void	editingMode(stdComs comID);
				void	newDocOpen(stdComs comID);
				void	colorOpen(stdComs comID);
				void	handleCom(stdComs comID);
	
				iEditStates		ourState;
				stdComs			comID;
				colorPicker*	colorAlert;
				colorBtnObj*	colorBtn;
				slider*			brushSlider;
				char*				strBuff;
				mapper			percentToBrush;
				mapper			brushToPercent;
};


#endif
