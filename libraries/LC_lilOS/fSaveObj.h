#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileBaseViewer.h>
#include <bmpKeyboard.h>
#include <editLabel.h>

class fSaveObj;

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
	virtual	void	setSuccess(bool trueFalse);
	virtual	char*	getSavePath(void);
	virtual	void	drawSelf(void);
	virtual	void	handleCom(stdComs comID);
	virtual	void	setItem(fileListItem* currentSelected);
			
				char*			savePath;
				label*		pathStr;
				editLabel*	nameStr;
};

#endif