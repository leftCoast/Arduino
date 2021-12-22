#ifndef fileViewer_h
#define fileViewer_h

#include <alertObj.h>
#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>

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
				fileListBox(int x, int y, int width,int height,bool(*funct)(char*));
	virtual	~fileListBox(void);
				
				void	setFileDir(fileDir* inFileDir);
				bool	checkFile(pathItem* trace);
	virtual	void	fillList(void);
	virtual	void	drawSelf(void);
	
				bool		(*filterFx)(char*);
				fileDir*	ourFileDir;
				bmpObj*	folderBmp;
				bmpObj*	docBmp;
				label*	itemLabel;
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
	virtual	void	setThisFocus(bool setLoose);
	virtual	void	setItem(pathItemType inType,char* name);
	virtual	void	chooseFolder(char* name);
	virtual	void	chooseFile(char* name);
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
				fileViewer(listener* inListener,bool(*funct)(char*));
	virtual	~fileViewer(void);
	
	virtual	void	handleCom(stdComs comID);
	virtual	char*	getPathResult(void);
	
				listener*		ourListener;
				fileDir*			ourFileDir;
				fileListBox*	ourListBox;
				fileListItem*	currentItem;
								
};

#endif