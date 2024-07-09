#ifndef stdComs_h
#define stdComs_h

#include <iconButton.h>

// Not all standard icons are used for commands. This library links those that are
// commands to the command set that they represent. And supplies a handy function for
// creating new ready to use ones.

enum stdComs	{	cutCmd, copyCmd, closeCmd, pasteCmd, cancelCmd, okCmd, newFileCmd,
						newFolderCmd, newItemCmd, openFileCmd, saveFileCmd, deleteItemCmd,
						searchCmd, sortCmd, editCmd };

enum stdLabels	{	warnLbl, choiceLbl, noteLbl, folderLbl, folderRetLbl, docLbl,
						SDCardLbl };
										
enum iconSize	{ icon16, icon22, icon32 };

						
// **************************************************************
// ***********************		listener		***********************
// **************************************************************


class listener {

	public:
				listener(void);
	virtual	~listener(void);
	
	virtual	void	handleCom(stdComs comID);
};



// **************************************************************
// ***********************		stdCom		***********************
// **************************************************************


class stdComBtn : public iconButton {

	public:
				stdComBtn(int xLoc,int yLoc,const char* path,stdComs iconType,listener* inListener,int pixles);
	virtual	~stdComBtn(void);
	
	virtual	void	doAction(void);
				void	active(bool trueFalse);
				
				listener*	ourListener;
				stdComs		ourComID;
};

bmpObj*		newStdLbl(int x,int y,iconSize inSize,stdLabels iconType);
stdComBtn*	newStdBtn(int x,int y,iconSize inSize,stdComs iconType,listener* inListener);

#endif