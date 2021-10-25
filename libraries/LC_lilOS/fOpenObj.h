#ifndef fOpenPanel_h
#define fOpenPanel_h

#include <lilOS.h>
#include <filePath.h>
#include <scrollingList.h>
#include <label.h>


class fileList;

// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
   
class fileListItem :	public drawGroup {

	public:
				fileListItem(fileList* inList,pathItemType inType,char* inName);
	virtual	~fileListItem(void);
	
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				fileList*		ourList;
				pathItemType	ourType;
				label*			fileName;
};



// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************

class fileList :	public scrollingList {

	public:
				fileList(int x, int y, int width,int height);
	virtual	~fileList(void);
	
				void	fillList(filePath* ourPath);
	virtual	void	drawSelf(void);
	
};



// **************************************************************
// ********************* fileListDir  stuff *********************
// **************************************************************


class fileListDir :	public drawGroup {

	public:
				fileListDir(filePath* inPath);
	virtual	~fileListDir(void);
	
				void	refresh(void);
	virtual	void 	draw(void);
	virtual	void	drawSelf(void);
	
				filePath*	ourPath;
				label*		dirName;
				bmpObj*		ourIcon;
};
	

	
// **************************************************************
// ********************** fOpenObj stuff ************************
// **************************************************************


class fOpenObj :	public drawGroup,
						public filePath {

	public:
				fOpenObj(void);
	virtual	~fOpenObj(void);
				
				void	setFilterCallback(bool(*funct)(char*));	// Use a callback to filter what you see.
				void	setResultCallback(void(*funct)(bool));		// Get a call back with the result.
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				bool				(*filterFx)(char*);
				void				(*resultFx)(bool);
				fileList*		fileListBox;
				fileListDir*	ourFileListDir;
				maskRect*		ourModalMask;
};

#endif