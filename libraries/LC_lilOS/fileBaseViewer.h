#ifndef fileBaseViewer_h
#define fileBaseViewer_h

#include <alertObj.h>
#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>
//#include <label.h>

class		fileBaseViewer;
class		fileDir;
class		fileListBox;



// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
   
class fileListItem :	public drawGroup {

	public:
				fileListItem(fileBaseViewer* inViewer,fileListBox* inList,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool);
	
				fileBaseViewer*	ourViewer;
				fileListBox*		ourList;
				pathItemType		ourType;
				char*					ourName;
};



// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************

class fileListBox :	public scrollingList {

	public:
				fileListBox(int x, int y, int width,int height);
	virtual	~fileListBox(void);
	
				bool	checkFile(fileBaseViewer* ourPath,pathItem* trace);
	virtual	void	fillList(fileBaseViewer* ourPath);
	virtual	void	drawSelf(void);
	
				bmpObj*	folderBmp;
				bmpObj*	docBmp;
				label*	itemLabel;
};



// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************


class fileDir :	public drawGroup {

	public:
				fileDir(fileBaseViewer* inViewer,fileListBox* inFileListBox);
	virtual	~fileDir(void);
	
				void	refresh(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool setLoose);
	
				fileBaseViewer*	ourViewer;
				fileListBox*		ourFileListBox;
				label*				dirName;
				bmpObj*				SDIcon;
				bmpObj*				folderIcon;
};
	
	
	
// **************************************************************
// ******************* fileBaseViewer stuff *********************
// **************************************************************


class fileBaseViewer :	public alertObj,
								public filePath {

	public:
				fileBaseViewer(panel* inPanel,bool(*funct)(char*));
	virtual	~fileBaseViewer(void);
	
	virtual	void	chooseFolder(char* name);
	virtual	void	chooseFile(char* name);
	//virtual	void	setSuccess(bool trueFalse);
	virtual	void	setItem(fileListItem* currentSelected);
	//virtual	void	drawSelf(void);
	//virtual	void	addObj(drawObj* newObj);
	//virtual	void	idle(void);
	virtual	void	handleCom(stdComs comID);
	
				bool				(*filterFx)(char*);
				fileListItem*	currentItem;
				fileListBox*	ourFileListBox;
				fileDir*			ourFileDir;
				panel*			ourPanel;
				//stdComBtn*		sBtn;
				//stdComBtn*		cBtn;
				///label*			ourLabel;
				timeObj*			dblClickTimer;
				//drawObj*			condemned;
};

#endif