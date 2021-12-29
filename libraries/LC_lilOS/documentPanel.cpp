#include <documentPanel.h>
#include <resizeBuff.h>

#include <debug.h>

#define SAVE_BEFORE_CLOSE "Continue and loose unsaved changes?"


bool appleFilter(char* fileName) {

	if (fileName[0]=='_') 					return false;
	if (!strcmp(fileName,"SPOTLI~1"))	return false;
	if (!strcmp(fileName,"TRASHE~1"))	return false;
	if (strstr(fileName,"DS_STO"))		return false;
	if (strstr(fileName,".DS_"))			return false;
	else return true;
}


// **************************************************************
// **********************    stateHolder    *********************
// **************************************************************

// State holder for alerts. This holds the state we were in and the command we were
// dealing with when our alert was created. And we can hand them back when asked.
stateHolder::stateHolder(docPanelStates inLastState,stdComs inLastComID) {

	lastState = inLastState;
	lastComID = inLastComID;
}

stateHolder::~stateHolder(void) {  }		
			
docPanelStates	stateHolder::getLastState(void) { return lastState; }

stdComs	stateHolder::getLastComID(void) { return lastComID; }

				
			
// **************************************************************
// *********************    selectFileObj    ********************
// **************************************************************


selectFileObj::selectFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(char*))
	: fSelectObj(inListener,funct),
	stateHolder(inLastState,inLastComID) { }


selectFileObj::~selectFileObj(void) {  }



// **************************************************************
// **********************    saveFileObj    *********************
// **************************************************************


saveFileObj::saveFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(char*))
	: fSaveObj(inListener,funct),
	stateHolder(inLastState,inLastComID) { }
	
	
saveFileObj::~saveFileObj(void) {  }
	

				
// **************************************************************
// ***********************		askOkObj		***********************
// **************************************************************


askOkObj::askOkObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID)
	: alertObj(SAVE_BEFORE_CLOSE,inListener,warnAlert,true,true),
	stateHolder(inLastState,inLastComID) { }
	
	
askOkObj::~askOkObj(void) {  }



// **************************************************************
// ********************		documentPanel		********************
// **************************************************************


// Our handy dandy constructor.
documentPanel::documentPanel(int panelID,char* inDocFolder,menuBarChoices menuBarChoice,eventSet inEventSet)
	: panel(panelID,menuBarChoice,inEventSet) {
	
	askAlert		= NULL;												// NULL out all the dynamic stuff for starters.
	selectAlert	= NULL;												// NULL
	saveAlert	= NULL;												// NULL
	docFolder	= NULL;												//	You get the picture..
	ourDoc		= NULL;												//
	filter		= appleFilter;										// Filter function gets defaulted to Apple junk stripper.
	if (resizeBuff(strlen(inDocFolder)+1,&docFolder)) {	// If we can allocate the RAM for the folder path..
		strcpy(docFolder,inDocFolder);							// Copy the folder path.
	}
	ourState			= noFile;										// Actually no file object. But this
}																			// really means is no file IN the object.


// Destructor. Alerts auto-destruct as do any drawObjects. We need to deal with the
// document and the different dynamic strings.
documentPanel::~documentPanel(void) {

	if (ourDoc) delete(ourDoc);	// Loose the document object.
	resizeBuff(0,&docFolder);		// And this needs to go.
}


// setup() & loop() panel style.
void documentPanel::setup(void) {
ST	
	createDocObj();
	stdComBtn* ourNewBtn = newStdBtn(40,1,icon22,newFileCmd,this);
	mMenuBar->addObj(ourNewBtn);
	stdComBtn* ourOpenBtn = newStdBtn(80,1,icon22,openFileCmd,this);
	mMenuBar->addObj(ourOpenBtn);
	stdComBtn* ourSaveBtn = newStdBtn(120,1,icon22,saveFileCmd,this);
	mMenuBar->addObj(ourSaveBtn);
	handleCom(newFileCmd);
}

void documentPanel::closing(void) {ST if (ourDoc) ourDoc->closeDocFile(); }


// We have no file to edit..
void documentPanel::handleComNoFile(stdComs comID) {
ST	
	switch(comID) {																	// Checking for each command..
		case newFileCmd	:															// Ask for a new file to be created.
			ourDoc->setNewFileParams();											// Setup for creating a new file.
			if (ourDoc->newDocFile(docFolder)) {								// If we can create this new file..
				ourState = haveFileNoNameNoEdits;								// Our state is now, have file, no name, no edits.
			}																				//
		break;																			// And we're all done.
		case openFileCmd	:															// Ask to choose a file to edit.
			selectAlert = new selectFileObj(this,noFile,comID,filter);	// Open the select file alert.
			ourState = selectOpen;													// Our state is now selectAlert is open.
		break;																			// And again, we are done.
		case saveFileCmd	: break;													// Save file? Pointless, we'll do nothing of the kind!
		default: panel::handleCom(comID);										// Everything else we pass up the chain.
	}
}



void documentPanel::handleComHaveFileNoNameNoEdits(stdComs comID) {
ST	
	switch(comID) {																						// Checking for each command..
		case newFileCmd	: break;																		// New file? Pointless, we do nothing and exit.
		case openFileCmd	:																				// Ask to open new file.
			selectAlert = new selectFileObj(this,haveFileNoNameNoEdits,comID,filter);	// Open the select file alert.
			ourState = selectOpen;																		// Our state is now selectAlert is open.
		break;																								// Exit.
		case saveFileCmd	:																				// Ask to save this file.
			saveAlert = new saveFileObj(this,haveFileNoNameNoEdits,comID,filter);		// Open the save file alert.
			ourState = saveOpen;																			// Our state is now saveAlert is open.
		break;																								// And we are done.
		default: panel::handleCom(comID);															// Everything else we pass up the chain.
	}
}


void documentPanel::handleComHaveNamedFileNoEdits(stdComs comID) {
ST	
	switch(comID) {																						// Checking for each command..
		case newFileCmd	:																				// Ask for a new file to be created.
			ourDoc->setNewFileParams();																// Setup for creating a new file.
			if (ourDoc->newDocFile(docFolder)) {													// If we can create this new file..
				ourState = haveFileNoNameNoEdits;													// Our state is now, have file, no name, no edits.
			}																									//
		break;																								// Sign off.
		case openFileCmd	:																				// They want to open a different file..
			selectAlert = new selectFileObj(this,haveNamedFileNoEdits,comID,filter);	// Open the select file alert.
			ourState = selectOpen;																		// Our state is now selectAlert is open.
		break;																								// Exit.
		case saveFileCmd	: break;																		// Save nothing? Again, pointless. Exit.
		default: panel::handleCom(comID);															// Everything else we pass up the chain.
	}
}


void documentPanel::handleComHasEditsNoName(stdComs comID) {
ST
	switch(comID) {																		// Checking for each command..
		case newFileCmd	:																// Ask for a new file to be created..
		case openFileCmd	:																// They want to open a different file.
		case closeCmd		:																// They want to close the panel.
			askAlert = new askOkObj(this,hasEditsNoName,comID);				// Ask if ok to loose changes.
			ourState = askOpen;															// Our state is now waiting for an yes/no answer.
		break;																				// Done.
		case saveFileCmd	:																// Ask to save this file.
			saveAlert = new saveFileObj(this,hasEditsNoName,comID,filter);	// Open the save file alert.
			ourState = saveOpen;															// Our state is now saveAlert is open.
		break;																				// Done.
		default: panel::handleCom(comID);											// Everything else we pass up the chain.														
	}
}


void documentPanel::handleComHasEditsNamed(stdComs comID) {
ST
	switch(comID) {																		// Checking for each command..
		case newFileCmd	:																// Ask for a new file to be created..
		case openFileCmd	:																// They want to open a different file.
		case closeCmd		:																// They want to close the panel.
			askAlert = new askOkObj(this,hasEditsNamed,comID);					// Ask if ok to loose changes.
			ourState = askOpen;															// Our state is now waiting for an yes/no answer.
		break;																				// Done.
		case saveFileCmd	:																// Ask to save this file.
			saveAlert = new saveFileObj(this,hasEditsNamed,comID,filter);	// Open the save file alert.
			ourState = saveOpen;															// Our state is now about saveAlert being open.
		break;																				// Done.
		default: panel::handleCom(comID);											// Everything else we pass up the chain.
	}
}

void documentPanel::handleComSelectOpen(stdComs comID) {
ST	
	switch(comID) {																	// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:															// Ok ws cliked..
			if (ourDoc->changeDocFile(selectAlert->getPathResult())) {	// If we can change to this new file..
				ourState = haveNamedFileNoEdits;									// We have a named file with no edits yet.
			} else {																		// Else, something went wrong..
				ourState = selectAlert->getLastState();						// Sad, we go back to where we were.
			}																				//
		break;																			// All done.
		case cancelCmd		:															// Cancel was clicked..
			ourState = selectAlert->getLastState();							// Return to previous state.
		break;																			// And we're done.
		default: panel::handleCom(comID);										// Everything else we pass up the chain.
	}
}


void documentPanel::handleComSaveOpen(stdComs comID) {
ST
	switch(comID) {																// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:														// Ok ws cliked..
			if (ourDoc->saveDocFile(selectAlert->getPathResult())) {	// We're able to save it..
				ourState = haveNamedFileNoEdits;								// We have a named file with no edits. ('Cause we saved them)
			} else {																	// Else, something went wrong with the saving..
				ourState = selectAlert->getLastState();					// We go back to where we were.
			}																			//
		break;																		// And we're done here.
		case cancelCmd		:														// Cancel was clicked..
			ourState = selectAlert->getLastState();						// Return to previous state.
		break;																		// And we're done.
		default: panel::handleCom(comID);									// Everything else we pass up the chain.
	}
}


void documentPanel::handleComAskOpen(stdComs comID) {
ST
	switch(comID) {																				// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:																		// Ok ws cliked..
			ourDoc->closeDocFile();																// Close the file, loose the changes.
			ourState = noFile;																	// Right now, our state is noFile.
			if (askAlert->getLastComID()==newFileCmd) {									// They were asking for a new file to be created..
				ourDoc->setNewFileParams();													// Setup for creating a new file.
				if (ourDoc->newDocFile(docFolder)) {										// If we can create this new file..
					ourState = haveFileNoNameNoEdits;										// Our state is now, have file, no name, no edits.
				}																						//																				//
			} else if (askAlert->getLastComID()==openFileCmd) {							// They wanted to open a different file..
				selectAlert = new selectFileObj(this,noFile,openFileCmd,filter);	// Open the select file alert.
				ourState = selectOpen;															// Now our state is select open.
			} else if (askAlert->getLastComID()==closeCmd) {							// They wanted the panel closed..
				panel::handleCom(closeCmd);													// This one, we pass up the chain.
			}																							//
		break;																						// Done, Whew!
		case cancelCmd		:																		// Cancel was clicked..
			ourState = askAlert->getLastState();											// Return to previous state.
		break;																						//
		default: panel::handleCom(comID);													// Everything else we pass up the chain.
	}
}


// Handle the commands from the buttons and dialog boxes..
void documentPanel::handleCom(stdComs comID) {
	
	if (ourDoc) {																								// If we have a fileObj..
		if (ourDoc->fileEdited()) {																// If it has changes..
			switch(ourState) {																				// For these unedited states..
				case noFile						: 																// noFile and..
				case haveFileNoNameNoEdits	: ourState = hasEditsNoName; break;					// no name no edits -> hasEditsNoName.
				case haveNamedFileNoEdits	: ourState = hasEditsNamed; break;					// Has Name no edits -> hasEditsNamed.
				default							: break;														// Shut up compiler!
			}																										//
		}																											//
		switch(ourState) {																					// For each state we can be in..
			case noFile						: handleComNoFile(comID);						break;	// We have a handeler
			case haveFileNoNameNoEdits	: handleComHaveFileNoNameNoEdits(comID);	break;	// to deal with it.
			case haveNamedFileNoEdits	: handleComHaveNamedFileNoEdits(comID);	break;	//
			case hasEditsNoName			: handleComHasEditsNoName(comID);			break;	//
			case hasEditsNamed			: handleComHasEditsNamed(comID);				break;	//
			case selectOpen				: handleComSelectOpen(comID);					break;	//
			case saveOpen					: handleComSaveOpen(comID);					break;	//
			case askOpen					: handleComAskOpen(comID);						break;	//
		}																											//
	} else {																										// Else, things are set up wrong!
		panel::handleCom(comID);																			// So, just let the ancestor deal with it.
	}
}

