#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileBaseViewer.h>
#include <bmpKeyboard.h>
#include <editLabel.h>
#include <alertObj.h>

class fSaveObj;

enum saveModes { choosing,newFolder,deletingFile };

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
	
	//static	void	createAlert(void);
				void	setName(char* inName);
	virtual	void	setSuccess(bool trueFalse);
	virtual	char*	getSavePath(void);
	virtual	void	drawSelf(void);
	static	void	newFolderAlert(void);
				void	deleteFile(void);
	static	void	deleteFileAlert(void);
				void	setMode(saveModes newMode);
	virtual	void	handleCom(stdComs comID);
	virtual	void	setItem(fileListItem* currentSelected);
			
				saveModes	wereDoing;
				char*			savePath;
				saveKbd*		aKbd;
				editLabel*	nameStr;
				alertObj*	ourAlert;
};

#endif