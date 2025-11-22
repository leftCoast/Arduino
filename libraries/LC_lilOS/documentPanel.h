#ifndef documentPanel_h
#define documentPanel_h


#include <docFileObj.h>
#include <lilOS.h>
#include <fSelectObj.h>
#include <fSaveObj.h>

// Base framework for a document editing panel. This ties in all the state and dialog
// boxes for doing something like this. New file, open file, save file, do you really want
// to loose unsaved changes? All that jazz. Seeing its for a document file, you can use it
// as a base class for for anything derived from the document class.
//
// For more in-depth documentation on how it works, see the .cpp file.


// Some tools for filtering out file lists.

bool hasExtension(char* inStr,const char* extension);


enum docPanelStates {	fileClosed, haveFileNoNameNoEdits, haveNamedFileNoEdits,
								hasEditsNoName, hasEditsNamed, selectOpen, saveOpen, askOpen,
								newDocFileOpen };

char* docStateStr(docPanelStates aState);


// **************************************************************
// **********************    stateHolder    *********************
// **************************************************************


class stateHolder {

	public:
				stateHolder(docPanelStates inLastState,stdComs inLastComID);
	virtual	~stateHolder(void);			
		
	
				docPanelStates	getLastState(void);
				stdComs			getLastComID(void);
	
				docPanelStates lastState;
				stdComs 			lastComID;
};

	
		
// ************************************************************** 
// *********************    selectFileObj    ********************
// **************************************************************


class selectFileObj :	public fSelectObj,
								public stateHolder {

	public :
				selectFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(const char*)=NULL);
	virtual	~selectFileObj(void);
};



// **************************************************************
// **********************    saveFileObj    *********************
// **************************************************************


class saveFileObj :	public fSaveObj,
							public stateHolder {

	public :
				saveFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(const char*)=NULL);
	virtual	~saveFileObj(void);
};



// **************************************************************
// ***********************		askOkObj		***********************
// **************************************************************

								
class askOkObj :	public alertObj,
						public stateHolder {

	public :
				askOkObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID);
	virtual	~askOkObj(void);
};


		
// **************************************************************
// ********************		documentPanel		********************
// **************************************************************

// Note on createDocObj(void). This is one that the class that inherits this, creates the
// document object of the class that it will edit. Use this to create the fileObj of your
// design and be able to edit it.
//
//
// Note on createNewDocFile(void). This is also one that the class that inherits this
// fills out. The result of this function is to send back an okCmd or cancelCmd through
// the command/listener channel. Depending on whether or not a new doc was created and
// loaded or not. 
//
// Why is this written this way?
// 
// The documentPanel class has no idea what kind of document any child class will be
// dealing with. So it has no idea what is involved in creating a new one. There is a very
// good chance that this may involve asking the user for information using an alert
// Object. So, this gives the child object the option to dovetail it's own alert object
// into the UI.
//
// NOTE: Sadly you have to put -something- in your newly created file or the SD stuff
// doesn't work and everything falls apart. No empty file allowed, I guess.


class documentPanel :	public panel {

	public :
				documentPanel(int panelID,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents);
	virtual	~documentPanel(void);
	
	virtual	void	createDocObj(void)=0;	
	virtual	bool	createNewDocFile(void)=0;
	virtual	void	setup(void);
	virtual	void	closing(void);
	virtual	void	setFilter(bool(*funct)(const char*));
	virtual	void	setDefaultPath(const char* inFolder);
	virtual	void	handleComFileClosed(stdComs comID);
	virtual	void	handleComHaveFileNoNameNoEdits(stdComs comID);
	virtual	void	handleComHaveNamedFileNoEdits(stdComs comID);
	virtual	void	handleComHasEditsNoName(stdComs comID);
	virtual	void	handleComHasEditsNamed(stdComs comID);
	virtual	void	handleComSelectOpen(stdComs comID);
	virtual	void	handleComSaveOpen(stdComs comID);
	virtual	void	handleComAskOpen(stdComs comID);
	virtual	void	handleNewDocFileOpen(stdComs);
	virtual	void	handleCom(stdComs comID);
	
				docPanelStates	ourState;
				docPanelStates	savedState;	// Used for creating new doc files.
				askOkObj*		askAlert;
				selectFileObj*	selectAlert;
				saveFileObj*	saveAlert;
				docFileObj*		ourDoc;
				char*				defaultPath;
				bool				(*filter)(const char*);
};


#endif


	
				
										