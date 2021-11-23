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
				void	showPath(void);
	virtual	void	reset(void);
	virtual	bool	setPath(char* inPath);
	virtual	bool	pushItem(pathItem* theNewGuy);
	virtual	void	popItem(void);
	virtual	void	setSuccess(bool trueFalse);
	virtual	char*	getSavePath(void);
	
				label*		pathStr;
				editLabel*	nameStr;
				char*			savePath;
};

#endif