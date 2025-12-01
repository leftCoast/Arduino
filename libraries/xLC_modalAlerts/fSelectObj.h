#ifndef fSelectObj_h
#define fSelectObj_h

#include <fileViewer.h>
#include <resizeBuff.h>
#include <strTools.h>

enum selectType { filesOnly, foldersOnly, filesAndFolders };


// **************************************************************
// ********************* selectFileDir **************************
// **************************************************************


class selectFileDir :	public fileDir {

	public:
				selectFileDir(int inX, int inY, int inWidth,int inHeight,selectType inSelectType,fileViewer* inViewer,fileListBox* inListBox);
	virtual	~selectFileDir(void);

	virtual	void	chooseFile(const char* name);
	virtual	void	setItem(pathItemType inType,const char* name);
	virtual	char* endChoice(void);
	
				selectType		ourSelectType;
				tempStr			currentName;
				pathItemType	currentType;
};



// **************************************************************
// *********************** fSelectObj ***************************
// **************************************************************


class fSelectObj :	public fileViewer {

	public:
				fSelectObj(listener* inListener,bool(*funct)(const char*)=NULL,selectType inSelectType=filesOnly);
	virtual	~fSelectObj(void);
	
	virtual	void	setInitialPath(char* inPath);
	virtual	void  handleCom(stdComs comID);
	
	selectType	ourSelectType;
};

#endif