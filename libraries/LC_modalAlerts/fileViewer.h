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
				fileListItem(fileViewer* inViewer,fileListBox* inList,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool);
	
				fileViewer*	ourViewer;
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
	
				bool	checkFile(fileViewer* ourPath,pathItem* trace);
	virtual	void	fillList(fileViewer* ourPath);
	virtual	void	drawSelf(void);
	
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
				fileDir(fileListBox* inFileListBox);
	virtual	~fileDir(void);
	
				void	refresh(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	virtual	void	setThisFocus(bool setLoose);
	
				fileListBox*		ourFileListBox;
				label*				dirName;
				bmpObj*				SDIcon;
				bmpObj*				folderIcon;
				timeObj				dblClickTimer;
};
	
	
	
// **************************************************************
// ********************* fileViewer stuff ***********************
// **************************************************************


class fileViewer :	public alertObj {

	public:
				fileViewer(listener* inListener,bool(*funct)(char*));
	virtual	~fileViewer(void);
	
	virtual	void	chooseFolder(char* name);
	virtual	void	chooseFile(char* name);
	virtual	void	setItem(fileListItem* currentSelected);
	virtual	void	handleCom(stdComs comID);
	
				bool				(*filterFx)(char*);
				//fileListItem*	currentItem;
				//fileListBox*	ourFileListBox;
				fileDir*			ourFileDir;
				

				
};

#endif