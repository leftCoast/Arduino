#ifndef fileSelect_h
#define fileSelect_h

#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>
#include <label.h>

class		fileBaseViewer;
class		fileDir;
class		fileListBox;


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
				fileListItem(fileBaseViewer* inViewer,fileListBox* inLIxt,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				fileBaseViewer*	ourViewer;
				fileListBox*		ourList;
				pathItemType		ourType;
				//label*				fileName;
				char*					ourName;
};



// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************

class fileListBox :	public scrollingList {

	public:
				fileListBox(int x, int y, int width,int height);
	virtual	~fileListBox(void);
	
				void	fillList(fileBaseViewer* ourPath);
	virtual	void	drawSelf(void);
};



// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************


class fileDir :	public drawGroup {

	public:
				fileDir(filePath* inPath,fileListBox* inFileListBox);
	virtual	~fileDir(void);
	
				void	refresh(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				filePath*		ourPath;
				fileListBox*	ourFileListBox;
				label*			dirName;
				bmpObj*			ourIcon;
};
	
	
	
// **************************************************************
// ******************* fileBaseViewer stuff *********************
// **************************************************************


class fileBaseViewer :	public modal,
								public filePath {

	public:
				fileBaseViewer(panel* inPanel);
	virtual	~fileBaseViewer(void);
				
	virtual	void	chooseFolder(char* name);
	virtual	void	chooseFile(char* name);
	virtual	void	setFilterCallback(bool(*funct)(char*));	// Use a callback to filter what you see.
	virtual	void	drawSelf(void);
	
				bool				(*filterFx)(char*);
				fileListBox*	ourFileListBox;
				fileDir*			ourFileDir;
				panel*			ourPanel;
};


	
// **************************************************************
// ********************** fOpenObj stuff ************************
// **************************************************************


class fOpenObj :	public fileBaseViewer {

	public:
				fOpenObj(panel* inPanel);
	virtual	~fOpenObj(void);
				
	virtual	void	chooseFile(char* name);
};



// **************************************************************
// ********************** fSaveObj stuff ************************
// **************************************************************


class fSaveObj :	public fileBaseViewer {

	public:
				fSaveObj(panel* inPanel);
	virtual	~fSaveObj(void);
	
};

#endif