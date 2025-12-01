#ifndef fileViewer_h
#define fileViewer_h

#include <alertObj.h>
#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>
#include <label.h>


class		fileViewer;
class		fileDir;
class		fileListBox;


// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
   
class fileListItem :	public drawGroup {

	public:
				fileListItem(fileListBox* inList,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool);
	
				fileListBox*		ourList;
				pathItemType		ourType;
				char*					ourName;
};



// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************

class fileListBox :	public scrollingList {

	public:
				fileListBox(int x, int y, int width,int height,bool(*funct)(const char*));
	virtual	~fileListBox(void);
				
				void	setFileDir(fileDir* inFileDir);
				bool	checkFile(pathItem* trace);
	virtual	void	fillList(void);
	virtual	void	drawSelf(void);
	
				bool		(*filterFx)(pathItem*);
				fileDir*	ourFileDir;
				bmpObj*	folderBmp;
				bmpObj*	docBmp;
				label*	itemLabel;
				timeObj*	clickTimer;
};



// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************


class fileDir :	public drawGroup,
						public filePath {

	public:
				fileDir(int inX, int inY, int inWidth,int inHeight,fileViewer* inViewer,fileListBox* inListBox);
	virtual	~fileDir(void);
	
				void	refresh(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setItem(pathItemType inType,const char* name);
	virtual	void	chooseFolder(const char* name);
	virtual	void	chooseFile(const char* name);
	virtual	char* endChoice(void);
	
				fileViewer*		ourViewer;
				fileListBox*	ourFileListBox;
				label*			dirName;
				bmpObj*			SDIcon;
				bmpObj*			folderIcon;
};
	
	
	
// **************************************************************
// ********************* fileViewer stuff ***********************
// **************************************************************


class fileViewer :	public alertObj {

	public:
				fileViewer(listener* inListener,bool(*funct)(const char*));
	virtual	~fileViewer(void);
	
	virtual	void	setPath(char* inPath);
	virtual	void	handleCom(stdComs comID);
	virtual	char*	getPathResult(void);
	
				fileDir*			ourFileDir;
				fileListBox*	ourListBox;
				fileListItem*	currentItem;
								
};

#endif