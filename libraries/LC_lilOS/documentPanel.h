#ifndef documentPanel_h
#define documentPanel_h

#include <docFileObj.h>
#include <lilOs.h>
#include <fSelectObj.h>
#include <fSaveObj.h>

// Base framework for a document editing panel. This ties in all the state and dilog boxes
// for doing something like this. New file, open file, save file, do you really want to
// loose unsaved changes.. All that jazz. Seeing its for a document file you can use it as
// a base class for for anything derived from the document class.
//
// For more in-depth documentation on how it works, see the .cpp file.

extern bool appleFilter(const char* fileName);


enum docPanelStates {	noFile, haveFileNoNameNoEdits, haveNamedFileNoEdits, 
								hasEditsNoName, hasEditsNamed, selectOpen, saveOpen, askOpen };


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
				selectFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(char*)=NULL);
	virtual	~selectFileObj(void);
};



// **************************************************************
// **********************    saveFileObj    *********************
// **************************************************************


class saveFileObj :	public fSaveObj,
							public stateHolder {

	public :
				saveFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(char*)=NULL);
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


class documentPanel :	public panel {
	public :
				documentPanel(int panelID,char* inDocFolder,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents);
	virtual	~documentPanel(void);
	
	virtual	void	createDocObj(void)=0;	// This is where YOU plug in the KIND of document object you want.
	virtual	void	setup(void);
	virtual	void	closing(void);
	virtual	void	handleComNoFile(stdComs comID);
	virtual	void	handleComHaveFileNoNameNoEdits(stdComs comID);
	virtual	void	handleComHaveNamedFileNoEdits(stdComs comID);
	virtual	void	handleComHasEditsNoName(stdComs comID);
	virtual	void	handleComHasEditsNamed(stdComs comID);
	virtual	void	handleComSelectOpen(stdComs comID);
	virtual	void	handleComSaveOpen(stdComs comID);
	virtual	void	handleComAskOpen(stdComs comID);
	virtual	void	handleCom(stdComs comID);
	
				char*				docFolder;
				docPanelStates	ourState;
				askOkObj*		askAlert;
				selectFileObj*	selectAlert;
				saveFileObj*	saveAlert;
				docFileObj*		ourDoc;
				bool				(*filter)(char*);
};


#endif


	
				
										