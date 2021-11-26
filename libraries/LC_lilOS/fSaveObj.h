#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileBaseViewer.h>
#include <bmpKeyboard.h>
#include <editLabel.h>

class fSaveObj;



// **************************************************************
// ********************** newFolderBtn stuff ***********************
// **************************************************************


class newFolderBtn :	public iconButton {

	public:
				newFolderBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~newFolderBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};


// **************************************************************
// ********************** deleteBtn stuff ***********************
// **************************************************************


class deleteBtn :	public iconButton {

	public:
				deleteBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~deleteBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};



// **************************************************************
// ********************* saveKbd stuff *************************
// **************************************************************


class saveKbd : public bmpKeyboard {

	public:
				saveKbd(editable* inEditObj,fSaveObj* inModal);
	virtual	~saveKbd(void);
	
	virtual	void	handleKey(keyCommands inEditCom);
	
				fSaveObj*	ourModal;
};


// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

	
class fSaveObj :	public fileBaseViewer {

	public:
				fSaveObj(panel* inPanel,bool(*funct)(char*)=NULL);
	virtual	~fSaveObj(void);
	
				void	setName(char* inName);
				void	showPath(void);
	virtual	void	reset(void);
	virtual	bool	setPath(char* inPath);
	virtual	bool	pushItem(pathItem* theNewGuy);
	virtual	void	popItem(void);
	virtual	void	setSuccess(bool trueFalse);
	virtual	char*	getSavePath(void);
	virtual	void	drawSelf(void);
	
				label*			pathStr;
				editLabel*		nameStr;
				newFolderBtn*	folderBtn;
				deleteBtn*		delBtn;
				char*				savePath;
};

#endif