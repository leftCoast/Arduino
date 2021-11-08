#ifndef fileSelect_h
#define fileSelect_h

#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>
#include <label.h>


class		fileListBox;
timeObj	dblClickTimer(500);


// **************************************************************
// ************************ OKBtn stuff *************************
// **************************************************************

class OKBtn :	public iconButton {

	public:
				OKBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~OKBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};


// **************************************************************
// ********************** cancelBtn stuff ***********************
// **************************************************************


class cancelBtn :	public iconButton {

	public:
				cancelBtn(int xLoc,int yLoc,char* path,modal* inModal);
	virtual	~cancelBtn(void);
	
	virtual	void	doAction(void);
	
				modal*	ourModal;
};

// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
   
class fileListItem :	public drawGroup {

	public:
				fileListItem(fileListBox* inList,fileListDir* inListDir,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				fileListBox*	ourList;
				fileListDir*	ourListDir;
				pathItemType	ourType;
				label*			fileName;
};



// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************

class fileListBox :	public scrollingList {

	public:
				fileListBox(int x, int y, int width,int height,fileListDir* inListDir);
	virtual	~fileListBox(void);
	
				void	fillList(filePath* ourPath);
	virtual	void	drawSelf(void);
	
				fileListDir*	ourFileListDir;
	
};



// **************************************************************
// ********************* fileListDir  stuff *********************
// **************************************************************


class fileListDir :	public drawGroup {

	public:
				fileListDir(fOpenObj* inPath);
	virtual	~fileListDir(void);
	
				void	refresh(void);
	virtual	void	drawSelf(void);
	
				filePath*	ourPath;
				label*		dirName;
				bmpObj*		ourIcon;
};
	

	
// **************************************************************
// ********************** fOpenObj stuff ************************
// **************************************************************


class fOpenObj :	public modal,
						public filePath {

	public:
				fOpenObj(panel* inPanel);
	virtual	~fOpenObj(void);
				
				void	chooseFolder(char* name);
				void	chooseFile(char* name);
				void	setFilterCallback(bool(*funct)(char*));	// Use a callback to filter what you see.
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				bool				(*filterFx)(char*);
				fileListBox*	ourFileListBox;
				fileListDir*	ourFileListDir;
				maskRect*		ourModalMask;
				panel*			ourPanel;
};



// **************************************************************
// ********************** fSaveObj stuff ************************
// **************************************************************


class fSaveObj :	public modal,
						public filePath {

	public:
				fSaveObj(panel* inPanel);
	virtual	~fSaveObj(void);
	/*			
	virtual	void	message(int inMsg);
				void	setFilterCallback(bool(*funct)(char*));	// Use a callback to filter what you see.
				void	setResultCallback(void(*funct)(bool));		// Get a call back with the result.
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				bool				(*filterFx)(char*);
				void				(*resultFx)(bool);
				fileListBox*	ourFileListBox;
				fileListDir*	ourFileListDir;
				maskRect*		ourModalMask;
				panel*			ourPanel;
	*/
};

#endif