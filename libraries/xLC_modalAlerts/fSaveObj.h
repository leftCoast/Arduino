
#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileViewer.h>
#include <editLabel.h>
#include <alertObj.h>
#include <modalKbd.h>

class fSaveObj;



// **************************************************************
// *********************** deleteAlert **************************
// **************************************************************


class deleteAlert :	public alertObj,
							public kbdUser {

	public:
				deleteAlert(listener* inListener,filePath* inPath,char* selectName);
	virtual	~deleteAlert(void);
	
	virtual	bool acceptEvent(event* inEvent,point* locaPt);
	virtual	void handleCom(stdComs comID);
				void deleteItem(void);
				
				filePath*	ourPath;
				char*			ourSelectName;
};



// **************************************************************
// *********************** folderAlert **************************
// **************************************************************


class folderAlert :	public alertObj,
							public kbdUser {

	public:
				folderAlert(listener* inListener,filePath* inPath);
	virtual	~folderAlert(void);
	
	virtual	bool acceptEvent(event* inEvent,point* locaPt);
	virtual	void handleCom(stdComs comID);
				void createFolder(void);
				
				editLabel*	nameLabel;
				filePath*	ourPath;
};



// **************************************************************
// *********************** saveFileDir **************************
// **************************************************************


class saveFileDir :	public fileDir {

	public:
				saveFileDir(int inX, int inY, int inWidth,int inHeight,fSaveObj* inViewer,fileListBox* inListBox);
	virtual	~saveFileDir(void);

	virtual	void	doAction(void);
	virtual	void	setItem(pathItemType inType,const char* name);
	virtual	char* endChoice(void);
	
				char*				finalPath;
};

// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

enum saveModes { choosing,newFolder,deletingFile };

	
class fSaveObj :	public fileViewer,
						public kbdUser {

	public:
				fSaveObj(listener* inListener,bool(*funct)(const char*)=NULL);
	virtual	~fSaveObj(void);
	
				void	setName(const char* inName);
				char*	getName(void);
				void	setSelectedName(const char* name);
				void	setMode(saveModes newMode);
	virtual	void	handleCom(stdComs comID);

			
				saveModes		wereDoing;
				char*				savePath;
				char*				selectedName;
				editLabel*		nameLabel;
				folderAlert*	newFldrAlert;
				deleteAlert*	doDeleteAlert;
				
};

#endif
