
#ifndef fSaveObj_h
#define fSaveObj_h

#include <fileViewer.h>
#include <editLabel.h>
#include <alertObj.h>
#include <modalKbd.h>

class fSaveObj;

enum saveModes { choosing,newFolder,deletingFile };


// **************************************************************
// ******************** saveAlert  stuff ************************
// **************************************************************


// class folderAlert :	public alertObj {
// 
// 	public:
// 				folderAlert(char* msg,saveKbd* inKbd,listener* inListener);
// 	virtual	~folderAlert(void);
// 	
// 	virtual	bool	acceptEvent(event* inEvent,point* locaPt);
// 	
// 				saveKbd*		sKbd;
// 				editLabel*	folderPath;
// };



// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

	
class fSaveObj :	public fileViewer,
						public kbdUser {

	public:
				fSaveObj(listener* inListener,bool(*funct)(char*)=NULL);
	virtual	~fSaveObj(void);
	
				void	setName(char* inName);
	//virtual	char*	getSavePath(void);
				//void	createFolder(void);
				//void	deleteFile(void);
				void	setMode(saveModes newMode);
	virtual	void	handleCom(stdComs comID);
	//virtual	void	setItem(fileListItem* currentSelected);
			
				saveModes		wereDoing;
				char*				savePath;
				editLabel*		nameStr;
				//alertObj*		deleteAlert;
				//folderAlert*	newFldrAlertPtr;
};

#endif
