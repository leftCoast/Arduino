#ifndef fSelectObj_h
#define fSelectObj_h

#include <fileViewer.h>

enum selectType { filesOnly, foldersOnly, filesAndFolders };


// **************************************************************
// ********************* selectFileDir **************************
// **************************************************************


class selectFileDir :	public fileDir {

	public:
				selectFileDir(int inX, int inY, int inWidth,int inHeight,selectType inSelectType,fileViewer* inViewer,fileListBox* inListBox);
	virtual	~selectFileDir(void);

	virtual	void	chooseFile(char* name);
	virtual	void	setItem(pathItemType inType,char* name);
	virtual	char* endChoice(void);
	
				selectType		ourSelectType;
				char*				currentNamePtr;
				pathItemType	currentType;
};



// **************************************************************
// *********************** fSelectObj ***************************
// **************************************************************


class fSelectObj :	public fileViewer {

	public:
				fSelectObj(listener* inListener,bool(*funct)(char*)=NULL,selectType inSelectType=filesOnly);
	virtual	~fSelectObj(void);
	
	virtual	void	setInitialPath(char* inPath);
	virtual	void  handleCom(stdComs comID);
	
	selectType	ourSelectType;
};

#endif