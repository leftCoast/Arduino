#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileBaseViewer.h>


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
	
				label*	pathStr;
				label*	nameStr;
};

#endif