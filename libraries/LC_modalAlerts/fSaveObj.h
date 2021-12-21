/*
#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileViewer.h>
#include <bmpKeyboard.h>
#include <editLabel.h>
#include <alertObj.h>

class fSaveObj;

enum saveModes { choosing,newFolder,deletingFile };


// **************************************************************
// ********************* saveKbd  stuff *************************
// **************************************************************


class saveKbd : public bmpKeyboard {

	public:
				saveKbd(editable* inEditObj,fSaveObj* inModal);
	virtual	~saveKbd(void);
	
	virtual	void	handleKey(keyCommands inEditCom);
	
				fSaveObj*	ourModal;
};



// **************************************************************
// ******************** saveAlert  stuff ************************
// **************************************************************


class folderAlert :	public alertObj {

	public:
				folderAlert(char* msg,saveKbd* inKbd,listener* inListener);
	virtual	~folderAlert(void);
	
	virtual	bool	acceptEvent(event* inEvent,point* locaPt);
	//virtual	void	drawSelf(void);
	
				saveKbd*		sKbd;
				editLabel*	folderPath;
};



// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

	
class fSaveObj :	public fileViewer {

	public:
				fSaveObj(panel* inPanel,bool(*funct)(char*)=NULL);
	virtual	~fSaveObj(void);
	
				void	setName(char* inName);
	virtual	char*	getSavePath(void);
				void	createFolder(void);
				void	deleteFile(void);
				void	setMode(saveModes newMode);
	virtual	void	handleCom(stdComs comID);
	virtual	void	setItem(fileListItem* currentSelected);
			
				saveModes		wereDoing;
				char*				savePath;
				saveKbd*			aKbd;
				editLabel*		nameStr;
				alertObj*		deleteAlert;
				folderAlert*	newFldrAlertPtr;
};

#endif
*/