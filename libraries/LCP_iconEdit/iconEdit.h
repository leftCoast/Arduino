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
	
	enum iEditStates { editing, newDoc, getColor };
	 				
				iconEdit(lilOS* ourOS,int ourAppID);
	virtual	~iconEdit(void);
	
	virtual	void	createDocObj(void);
	virtual	void	createNewDocFile(void);
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void	loop(void);
				char* getColorPickerPath(void);
	virtual	void	openColorPicker(void);
	virtual	void	editingMode(stdComs comID);
	virtual	void	newDocOpen(stdComs comID);
	virtual	void	colorOpen(stdComs comID);
	virtual	void	handleCom(stdComs comID);
	
				iEditStates		ourState;
				bool				haveComToPassOn;
				stdComs			comID;
				colorPicker*	colorAlert;
				colorBtnObj*	colorBtn;
				char*				strBuff;
};


#endif
