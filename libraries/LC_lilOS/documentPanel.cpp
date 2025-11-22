#include <documentPanel.h>
#include <resizeBuff.h>
#include <strTools.h>
//#include <debug.h>

#define SAVE_BEFORE_CLOSE "Continue and loose unsaved changes?"


char*	stateStr = NULL;

char* docStateStr(docPanelStates aState) {
	
	switch(aState) {
		case fileClosed				: heapStr(&stateStr,"File closed."); 						break;
		case haveFileNoNameNoEdits	: heapStr(&stateStr,"Have file no name, no edits.");	break;
		case haveNamedFileNoEdits	: heapStr(&stateStr,"Have named file, no edits.");		break;
		case hasEditsNoName			: heapStr(&stateStr,"Has edits, no name.");				break;
		case hasEditsNamed			: heapStr(&stateStr,"Has edits, named.");					break;
		case selectOpen				: heapStr(&stateStr,"Select open.");						break;
		case saveOpen					: heapStr(&stateStr,"Save open.");							break;
		case askOpen					: heapStr(&stateStr,"Ask open.");							break;
		case newDocFileOpen			: heapStr(&stateStr,"New doc file open.");				break;
	}
	return stateStr;
}

/*
bool appleFilter(pathItem* inItem) {

	if (inItem->getName()[0]=='_') 					return false;
	if (!strcmp(inItem->getName(),"SPOTLI~1"))	return false;
	if (!strcmp(inItem->getName(),"TRASHE~1"))	return false;
	if (!strcmp(inItem->getName(),".TRASHES"))	return false;
	if (!strcmp(inItem->getName(),"FSEVEN~1"))	return false;
	if (strstr(inItem->getName(),"DS_STO"))		return false;
	if (strstr(inItem->getName(),".DS_"))			return false;
	if (strstr(inItem->getName(),"._"))				return false;
	if (strstr(inItem->getName(),".FSEVE"))		return false;
	else return true;
}
*/

bool hasExtension(char* inStr,const char* extension) {

	int 	numChars;
	int	index;
	
	numChars = strlen(inStr);
	if (numChars>(int)strlen(extension)) {
		index = numChars;
		while(inStr[index]!='.'&&index>=0) index--;
		if (inStr[index]=='.') {
			return !strcmp(&(inStr[index]),extension);
		}
	}
	return false;
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


selectFileObj::selectFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(const char*))
	: fSelectObj(inListener,funct),
	stateHolder(inLastState,inLastComID) { }


selectFileObj::~selectFileObj(void) {  }



// **************************************************************
// **********************    saveFileObj    *********************
// **************************************************************


saveFileObj::saveFileObj(listener* inListener,docPanelStates inLastState,stdComs inLastComID,bool(*funct)(const char*))
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
documentPanel::documentPanel(int panelID,menuBarChoices menuBarChoice,eventSet inEventSet)
	: panel(panelID,menuBarChoice,inEventSet) {
	
	askAlert		= NULL;							// NULL out all the dynamic stuff for starters.
	selectAlert	= NULL;							// NULL
	saveAlert	= NULL;							// NULL
	ourDoc		= NULL;							//
	defaultPath	= NULL;							//
	filter		= MacOSFilter;					// Filter function gets defaulted to Apple junk stripper.
	ourState		= haveFileNoNameNoEdits;	// Well, it'll be this once the docObj is created..
}


// Destructor. Alerts auto-destruct as do any drawObjects. We need to deal with the
// document and the different dynamic strings.
documentPanel::~documentPanel(void) {

	if (ourDoc) delete(ourDoc);	// Loose the document object.
	freeStr(&defaultPath);			// Loose the default doc folder path.
}


// setup() & loop() panel style.
void documentPanel::setup(void) {
	
	createDocObj();
	stdComBtn* ourNewBtn = newStdBtn(40,1,icon22,newFileCmd,this);
	mMenuBar->addObj(ourNewBtn);
	stdComBtn* ourOpenBtn = newStdBtn(80,1,icon22,openFileCmd,this);
	mMenuBar->addObj(ourOpenBtn);
	stdComBtn* ourSaveBtn = newStdBtn(120,1,icon22,saveFileCmd,this);
	mMenuBar->addObj(ourSaveBtn);
}


void documentPanel::closing(void) { if (ourDoc) ourDoc->closeDocFile(); }
	
// Let the children set the filter..				
void documentPanel::setFilter(bool(*funct)(const char*)) { filter = funct; }


// Let the descendants set a default folder path for saving/retrieving documents.
void documentPanel::setDefaultPath(const char* inFolder) { heapStr(&defaultPath,inFolder); }

			
// We have a file, but it's closed..
void documentPanel::handleComFileClosed(stdComs comID) {
	
	switch(comID) {																			// Checking for each command..
		case newFileCmd	:																	// Ask for a new file to be created.
			if (createNewDocFile()) {														// If we can create a new file..
				ourState = newDocFileOpen;													// Our state is now that we have a new doc. file.
			}
		break;																					// And we're all done.
		case openFileCmd	:																	// Ask to choose a file to edit.
			selectAlert = new selectFileObj(this,fileClosed,comID,filter);		// Open the select file alert.
			if (defaultPath) {																// If we have a default folder selected..
				selectAlert->setPath(defaultPath);										// Point the select Alert to that folder.
			}																						// 
			ourState = selectOpen;															// Our state is now selectAlert is open.
		break;																					// And again, we are done.
		case saveFileCmd	: break;															// Save file? Pointless, we'll do nothing of the kind!
		default: panel::handleCom(comID);												// Everything else we pass up the chain.
	}
}


// We have an auto generated new file that's un-edited.
void documentPanel::handleComHaveFileNoNameNoEdits(stdComs comID) {
	
	switch(comID) {																						// Checking for each command..
		case newFileCmd	: break;																		// New file? Pointless, we do nothing and exit.
		case openFileCmd	:																				// Ask to open new file.
			selectAlert = new selectFileObj(this,haveFileNoNameNoEdits,comID,filter);	// Open the select file alert.
			if (defaultPath) {																			// If we have a default folder selected..
				selectAlert->setPath(defaultPath);													// Point the select Alert to that folder.
			}																									// 
			ourState = selectOpen;																		// Our state is now selectAlert is open.
		break;																								// Exit.
		case saveFileCmd	:																				// Ask to save this file.
			saveAlert = new saveFileObj(this,haveFileNoNameNoEdits,comID,filter);		// Open the save file alert.
			if (defaultPath) {																			// If we have a default folder selected..
				saveAlert->setPath(defaultPath);														// Point the select Alert to that folder.
			}																									//
			ourState = saveOpen;																			// Our state is now saveAlert is open.
		break;																								// And we are done.
		default: panel::handleCom(comID);															// Everything else we pass up the chain.
	}
}


void documentPanel::handleComHaveNamedFileNoEdits(stdComs comID) {
	
	switch(comID) {																						// Checking for each command..
		case newFileCmd	:																				// Ask for a new file to be created.
			if (createNewDocFile()) {																	// If we can create a new file..
				ourState = newDocFileOpen;																// Our state is now that we are waiting for it to be created.	
			}																									//
		break;																								// Sign off.
		case openFileCmd	:																				// They want to open a different file..
			selectAlert = new selectFileObj(this,haveNamedFileNoEdits,comID,filter);	// Open the select file alert.
			if (defaultPath) {																			// If we have a default folder selected..
				selectAlert->setPath(defaultPath);													// Point the select Alert to that folder.
			}																									//
			ourState = selectOpen;																		// Our state is now selectAlert is open.
		break;																								// Exit.
		case saveFileCmd	:																				// Ask to save this file.
			saveAlert = new saveFileObj(this,haveNamedFileNoEdits,comID,filter);			// Open the save file alert.
			saveAlert->setPath(ourDoc->getFolder());												// Grab the folder path.
			saveAlert->setName(ourDoc->getName());													// Grab the file name.
			ourState = saveOpen;																			// Our state is now saveAlert is open.
		break;																								// And we are done.
		default : panel::handleCom(comID);															// Everything else we pass up the chain.
	}
}


void documentPanel::handleComHasEditsNoName(stdComs comID) {

	switch(comID) {																		// Checking for each command..
		case newFileCmd	:																// Ask for a new file to be created..
		case openFileCmd	:																// They want to open a different file.
		case closeCmd		:																// They want to close the panel.
			askAlert = new askOkObj(this,hasEditsNoName,comID);				// Ask if ok to loose changes.
			ourState = askOpen;															// Our state is now waiting for an yes/no answer.
		break;																				// Done.
		case saveFileCmd	:																// Ask to save this file.
			saveAlert = new saveFileObj(this,hasEditsNoName,comID,filter);	// Open the save file alert.
			if (defaultPath) {															// If we have a default folder selected..
				saveAlert->setPath(defaultPath);										// Point the select Alert to that folder.
			}
			ourState = saveOpen;															// Our state is now saveAlert is open.
		break;																				// Done.
		default: panel::handleCom(comID);											// Everything else we pass up the chain.														
	}
}


void documentPanel::handleComHasEditsNamed(stdComs comID) {
	
	switch(comID) {																		// Checking for each command..
		case newFileCmd	:																// Ask for a new file to be created..
		case openFileCmd	:																// They want to open a different file.
		case closeCmd		:																// They want to close the panel.
			askAlert = new askOkObj(this,hasEditsNamed,comID);					// Ask if ok to loose changes.
			ourState = askOpen;															// Our state is now waiting for an yes/no answer.
		break;																				// Done.
		case saveFileCmd	:																// Ask to save this file.
			saveAlert = new saveFileObj(this,hasEditsNamed,comID,filter);	// Open the save file alert.
			saveAlert->setPath(ourDoc->getFolder());								// Grab the folder path.
			saveAlert->setName(ourDoc->getName());									// Grab the file name.
			ourState = saveOpen;															// Our state is now saveAlert is open.
		break;																				// Done.
		default: panel::handleCom(comID);											// Everything else we pass up the chain.
	}
}


void documentPanel::handleComSelectOpen(stdComs comID) {
	
	tempStr pathResult;
	
	switch(comID) {																	// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:															// Ok ws cliked..
			pathResult.setStr(selectAlert->getPathResult());				// Copy whatever they give us.
			if (ourDoc->changeDocFile(pathResult.getStr())) {				// If we can change to this new file..
				if (ourDoc->openDocFile(FILE_WRITE)) {							// If we can open the damn file.
					ourState = haveNamedFileNoEdits;								// We have a named file with no edits yet.
					break;																// All done, wash hands and scram!
				}																			//
			}																				//
			ourState = fileClosed;													// Ok, we basically now, have no file. Things are messed up!																				//
		break;																			// All done.
		case cancelCmd		:															// Cancel was clicked..
			ourState = selectAlert->getLastState();							// Return to previous state.
		break;																			// And we're done.
		default: panel::handleCom(comID);										// Everything else we pass up the chain.
	}
}


void documentPanel::handleComSaveOpen(stdComs comID) {

	switch(comID) {																	// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:															// Ok ws cliked..
			ourState = saveAlert->getLastState();								// Just in case we fail here..
			if (ourDoc->saveDocFile(saveAlert->getPathResult())) {		// If we can do the save or saveAs thing..
				ourState = haveNamedFileNoEdits;									// We are now looking at a named file with no edits.
			}																				//
		break;																			// And we're done here.
		case cancelCmd		:															// Cancel was clicked..
			ourState = saveAlert->getLastState();								// Return to previous state.
		break;																			// And we're done.
		default: panel::handleCom(comID);										// Everything else we pass up the chain.
	}
}


void documentPanel::handleComAskOpen(stdComs comID) {

	switch(comID) {																					// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:																			// Ok ws cliked..
			if (askAlert->getLastComID()==newFileCmd) {										// They were asking for a new file to be created..
				if (ourDoc->createNewDocFile()) {												// If we can create this new file..
					ourState = haveFileNoNameNoEdits;											// Our state is now, have file, no name, no edits.
				} else {																					// Else we couldn't open a new file..
					ourState = fileClosed;															// Right now, our state is fileClosed.
				}																							//
			} else if (askAlert->getLastComID()==openFileCmd) {							// They wanted to open a different file..
				selectAlert = new selectFileObj(this,askAlert->getLastState(),openFileCmd,filter);	// Open the select file alert.
				if (defaultPath) {																	// If we have a default folder selected..
					selectAlert->setPath(defaultPath);											// Point the select Alert to that folder.
				}																							//
				ourState = selectOpen;																// Now our state is select open.
			} else if (askAlert->getLastComID()==closeCmd) {								// They wanted the panel closed..
				ourDoc->closeDocFile();																// Close our file, loose the changes.
				panel::handleCom(closeCmd);														// This one, we pass up the chain.
			}																								//
		break;																							// Done, Whew!
		case cancelCmd		:																			// Cancel was clicked..
			ourState = askAlert->getLastState();												// Return to previous state.
		break;																							//
		default: panel::handleCom(comID);														// Everything else we pass up the chain.
	}
}


void documentPanel::handleNewDocFileOpen(stdComs comID) {

	switch(comID) {																// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:														// Ok was cliked..
			ourState = haveFileNoNameNoEdits;								// We have an un-named file with no edits. (Because it's a new file)																			//
		break;																		// And we're done here.
		case cancelCmd		:														// Cancel was clicked..
			ourState = savedState;												// Return to previous state.
		break;																		// And we're done.
		default: panel::handleCom(comID);									// Everything else we pass up the chain.
	}
}


// Handle the commands from the buttons and dialog boxes..
void documentPanel::handleCom(stdComs comID) {
	
	if (ourDoc) {																								// If we have a fileObj..
		if (ourDoc->fileEdited()) {																// If it has changes..
			switch(ourState) {																				// For these unedited states..
				case fileClosed				: 																// fileClosed and..
				case haveFileNoNameNoEdits	: ourState = hasEditsNoName; break;					// no name no edits -> hasEditsNoName.
				case haveNamedFileNoEdits	: ourState = hasEditsNamed; break;					// Has Name no edits -> hasEditsNamed.
				default							: break;														// Shut up compiler!
			}																										//
		}
		switch(ourState) {																					// For each state we can be in..
			case fileClosed				: handleComFileClosed(comID);					break;	// We have a handeler
			case haveFileNoNameNoEdits	: handleComHaveFileNoNameNoEdits(comID);	break;	// to deal with it.
			case haveNamedFileNoEdits	: handleComHaveNamedFileNoEdits(comID);	break;	//
			case hasEditsNoName			: handleComHasEditsNoName(comID);			break;	//
			case hasEditsNamed			: handleComHasEditsNamed(comID);				break;	//
			case selectOpen				: handleComSelectOpen(comID);					break;	//
			case saveOpen					: handleComSaveOpen(comID);					break;	//
			case askOpen					: handleComAskOpen(comID);						break;	//
			case newDocFileOpen			: handleNewDocFileOpen(comID);				break;	//
		}																											//
	} else {																										// Else, things are set up wrong!
		panel::handleCom(comID);																			// So, just let the ancestor deal with it.
	}
}

