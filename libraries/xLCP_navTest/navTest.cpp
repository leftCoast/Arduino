#include <navTest.h>
#include <strTools.h>


#define NAV_TEST_PATH	 "/system/appFiles/navtest/"
#define FILE_PATH			"/system/appFiles/navtest/wpList"


void failBeep(void) { tone(OSPtr->getTonePin(), 100,200); }


// **************************************************
// *************** setMarkBtn ***********************
// **************************************************


setMarkBtn::setMarkBtn(const char* path,navTest* inListener)
	:iconButton(SET_WP_BTN_X,SET_WP_BTN_Y,path) { ourListener = inListener; }
	
	
setMarkBtn::~setMarkBtn(void) { }


void setMarkBtn::doAction(void) { 

	if(ourListener) {
		ourListener->setMark();
		setFocusPtr(NULL);
	}
}



// **************************************************
// *************** wpListItem ***********************
// **************************************************		
	
colorObj backColor;

				
wpListItem::wpListItem(wpObj* inWpObj,wpDispList* inListMgr)
	: drawGroup(0,0,WP_ITEM_W,WP_ITEM_H) {
	
	rect labelRect;
	
	ourWP = inWpObj;
	ourListMgr = inListMgr;
	setEventSet(touchLift);
	labelRect.setRect(4,2,150,10);
	wpName = new fontLabel(&labelRect);
	if (wpName) {
		wpName->setColors(&yellow,&backColor);
		wpName->setFont(AFF_SANS_BOLD_9_OB);
		wpName->setValue(ourWP->getName());
		addObj(wpName);
	}
}

	
wpListItem::~wpListItem(void) {

	if (haveFocus()) {			// If we have focus..
		currentFocus = NULL;		// We QUIETLY point it to NULL without calling stuff. (Because? We're a ghost!)
	}
}


void wpListItem::doAction(void) { setFocusPtr(this); }


void wpListItem::setThisFocus(bool setLoose) {

	if (setLoose) {
		ourListMgr->setSelected(this);
		wpName->setColors(&white,&blue);
	} else {
		ourListMgr->setSelected(NULL);
		wpName->setColors(&yellow,&backColor);
	}
	drawObj::setThisFocus(setLoose);
}

						
void wpListItem::drawSelf(void) {
	
	if (haveFocus()) {
		screen->fillRect(this,&blue);
	} else {
		screen->fillRect(this,&black);
	}
}


				
// **************************************************
// *************** wpDispList ***********************
// **************************************************	


wpDispList::wpDispList(int inX,int inY,int inW,int inH)
	: drawList(inX,inY,inW,inH,dragEvents) {
	
	selected = NULL;
	backColor.setColor(LC_CHARCOAL);
}
	
	
wpDispList::~wpDispList(void) {  }

					
void wpDispList::drawSelf(void) {

	rect drawRect;
	
	drawRect.setRect(this);
	drawRect.insetRect(-1);
	screen->fillRect(this,&black);
	screen->drawRect(&drawRect,&red);
}
		
		
void wpDispList::setSelected(wpListItem* inSelect) { selected = inSelect; }


wpListItem* wpDispList::getSelected(void) { return selected; }	


				
// **************************************************
// ***************** navTest ************************
// **************************************************


enum testCmds {
	
	noCommand,
	addWP,
	editWP,
	readWPFile,
	saveWPFile,
	listWP,
	deleteWP,
	sortNames,
	
	printWD,    // The rest is up to you. help would be a good one. Have it list
	changeDir,  // What the other commands are, how they work and what they do.
	listDir,
	makeDir,
	deleteFile
};
		

navTest::navTest(int ourAppID)
	: panel(ourAppID) {
	
	cmdParser = NULL;		// Donno' just in case.
}


navTest::~navTest(void) {

	if (cmdParser) {
		delete(cmdParser);
		cmdParser = NULL;
	}
}


bool navTest::setupScreen(void) {
	
	setMarkBtn*	markBtn;
	stdComBtn*	addBtn;
	stdComBtn*	deleteBtn;
	stdComBtn*	editBtn;
	
	screen->fillScreen(&black);
	screen->setCursor(25,25);     // Move cursor to the top left somewhere.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(1);       // Small enough to fit.
	screen->drawText("Type ? into the serial monitor.");
	setFilePath(SET_WP_BTN_N);
	markBtn = new setMarkBtn(mFilePath,this);
	if (markBtn) {
		addObj(markBtn);
		addBtn = newStdBtn(ADD_WP_BTN_X,ADD_WP_BTN_Y,icon32,newItemCmd,this);
		if (addBtn) {
			addObj(addBtn);
			deleteBtn = newStdBtn(DELETE_WP_BTN_X,DELETE_WP_BTN_Y,icon32,deleteItemCmd,this);
			if (deleteBtn) {
				addObj(deleteBtn);
				editBtn = newStdBtn(EDIT_WP_BTN_X,EDIT_WP_BTN_Y,icon32,editCmd,this);
				if (editBtn) {
					addObj(editBtn);
					ourWPDisplay = new wpDispList( WP_LIST_X, WP_LIST_Y,WP_LIST_W,WP_LIST_H);
					if (ourWPDisplay) {
						addObj(ourWPDisplay);
						return true;
					}
				}
			}
		}
	}
	return false;
}


bool navTest::setupParser(void) {

	cmdParser = new lilParser(65);
	if (!cmdParser) {
		Serial.println("Can't allocate our parser.");
		return false;close();
	}
	cmdParser->addCmd(addWP,"addwp");
	cmdParser->addCmd(editWP,"editwp");
	cmdParser->addCmd(listWP,"listwp");
	cmdParser->addCmd(deleteWP,"deletewp");
	cmdParser->addCmd(sortNames,"sortwp");
	cmdParser->addCmd(readWPFile,"readwp");
	cmdParser->addCmd(saveWPFile,"savewp");
	
	cmdParser->addCmd(printWD, "pwd");		// Add pwd command  [ If they type "pwd" they mean printWD ]
	cmdParser->addCmd(changeDir, "cd");		// Add cd command
	cmdParser->addCmd(listDir, "ls");		// Add ls command
	cmdParser->addCmd(makeDir, "mkdr");		// Add mkdr command
	cmdParser->addCmd(makeDir, "mkdir");	// We'll take it either way.
	cmdParser->addCmd(deleteFile, "rm");	// Add rm command.
	return true;
}


bool navTest::setupFakeFix(void) {

	fakeFix = new globalPos();
	if (fakeFix) {
		fakeFix->setPos(48.490077,-122.661747);
		return true;
	}
	return false;
}


bool navTest::setupWPList(void) {
	
	wayPoint	aWP;
	
	ourWPList = new wpList(fakeFix);
	if (ourWPList) {
		ourWPList->readList(FILE_PATH);
		return true;
	}
	return false;
}


bool navTest::fillScreenList(void) {

	wpObj* 		trace;
	wpListItem*	newListItem;
	
	trace = (wpObj*) ourWPList->getFirst();
	while(trace) {
		newListItem = new wpListItem(trace,ourWPDisplay);
		if (newListItem) {
			ourWPDisplay->addObj(newListItem);
			trace = (wpObj*)trace->getNext();
		} else {
			trace = NULL;
			return false;
		}
	}
	return true;
}


bool navTest::setupLowLevel(void) {

	wd.setPath("/");	// Initial path setting.
	return true;
}


void navTest::setup(void) {

	if (setupScreen()) {											// If we can setup the screen..
		if (setupParser()) {										// If we can setup the parser..
			if (setupFakeFix()) {
				if (setupWPList()) {	
					if (fillScreenList()) {
						if (setupLowLevel()) {							// If we can setup the low level stuff..
							Serial.println("Setup looks good!");	// Let 'me know
							Serial.print("Fake fix is : ");
							Serial.print(fakeFix->showLatStr());
							Serial.print("   ");
							Serial.println(fakeFix->showLonStr());
							showCurs();                         	// Default curser setting.
							return;											// Then we're good to go!
						}
					}
				}
			}														//
		}																//
	}																	//
	close();															// If we get here? We failed. Shut it down!
}


void navTest::loop(void) {

	char 	aChar;
	int	comVal;
	bool  curs;
	
	if (Serial.available()) {									// If the user has typed a char..
      curs = true;                                 	// Show the cursor after..
      aChar = Serial.read();									// Read and save it.
      Serial.print(aChar);										// Echo it back to the serial monitor.
      aChar = tolower(aChar);									// Make it all lower case. Makes sorting it out easier.
      comVal = cmdParser->addChar(aChar);					// Parse this char and see what we get back.
      switch(comVal) {											// If we find a command, we call a hanlder for it.
      	case noCommand		: curs = false;	break;	// You know, still parsing.
      	case addWP			: doAddWP();		break;
      	case editWP  		: doEditWP();		break;
      	case listWP			: doListWP();		break;
         case deleteWP		: doDeteWP();		break;
      	case sortNames		: doSortNames();	break;
      	case readWPFile	: doReadWPFile();	break;
      	case saveWPFile	: doSaveWPFile();	break;
      	
      	case printWD      : Serial.println(wd.getPath());  break;   // Easy peasy! Just print wd out.
			case listDir      : listDirectory();               break;   // Print out a listing of the working directory.
			case makeDir      : makeDirectory();               break;   // See if we can create a directory in the working directory.
			case changeDir    : changeDirectory();             break;   // Try changing directorys.
			case deleteFile   : deleteItem();                  break;   // Delete a directory or file.
      	default				: printHelp();	break;	// Tell 'em what it's all about.
      }
      if (curs) showCurs();
   }
}


void navTest::setMark(void) {
	
	wpListItem*	trace;
	wpObj*		wpNode;
	
	trace = ourWPDisplay->getSelected();
	if (trace) {
		wpNode = trace->ourWP;
		OSPtr->beep();
		Serial.print("Setting : ");
		Serial.println(wpNode->getName());
	} else {
		failBeep();
		Serial.println("Nothing selected.");
	}
}
			
			
// Handle the commands from the standard buttons.
void navTest::handleCom(stdComs comID) {
	
	panel::handleCom(comID);
	switch(comID) {
		case cancelCmd			:
			OSPtr->beep();
		break;
		case okCmd				:
			OSPtr->beep();
		break;
		case newItemCmd		:
			OSPtr->beep();
		break;
		case saveFileCmd		:
			OSPtr->beep();
		break;
		case deleteItemCmd	:
			OSPtr->beep();
		break;
		case sortCmd			:
			OSPtr->beep();
		break;
		case editCmd			:
			OSPtr->beep();
		break;
		default					:
		break;
	}
}


void navTest::closing(void) {
	
	Serial.println("bye.");
}


void navTest::showCurs(void) {

  Serial.print(wd.getPath());
  Serial.print(" > ");
}

// **************************************************
// 						COMMAND HANDLERS
// **************************************************


void navTest::doAddWP(void) {

	int		numTextParams;
	char*		localBuff;
	double	lat;
	double	lon;
	wayPoint	newWP;
	
	numTextParams = cmdParser->numParams() - 2;				// Name + 2 numbers.
	if (numTextParams>0) {											// Make sure we got enough.
		localBuff = NULL;												// Setup to grab a copy.
		heapStr(&localBuff,cmdParser->getParamBuff());		// Gives us at more than enough room.
		if (localBuff) {												// Got the RAM?
			strcpy(localBuff,cmdParser->getNextParam());		// Copy in first param.
			for (int i=1;i<numTextParams;i++) {					// Now for every other text param..
				strcat(localBuff," ");								// Add a space
				strcat(localBuff,cmdParser->getNextParam());	// Add the text token.
			}																//
			newWP.setName(localBuff);								// Stuff in the assembled name.
			freeStr(&localBuff);										// recycle our copy.
			lat = atof(cmdParser->getNextParam());				// Grab and decode lat.
			lon = atof(cmdParser->getNextParam());				// Grab and decode lon.
			newWP.setPos(lat,lon);									// Stuff in the values.
			ourWPList->addWP(&newWP);								// And add our point.
		}
	}
}


void navTest::doEditWP(void) {
	Serial.println("Edit a way point");
}


void navTest::doListWP(void) {

	wpObj*	trace;
	char		strBuff[150];
	double	bearing;
	double	distance;
	
	Serial.println("*** Waypoint list ***"); 
	trace = (wpObj*)ourWPList->getFirst();
	while(trace) {
		bearing = fakeFix->trueBearingTo(trace);
		distance = fakeFix->distanceTo(trace);
		sprintf(strBuff,"%-18s\t%11.6f\t%11.6f\t",trace->getName(),trace->getLatAsDbl(),trace->getLonAsDbl());
		Serial.print(strBuff);
		Serial.print(bearing);
		Serial.print("\t");
		Serial.println(distance);
		trace = (wpObj*)trace->getNext();
	}
	Serial.println();
}


void navTest::doDeteWP(void) {
	/*
	char*		strBuff;
	wpObj*	theWPObj;
	
	strBuff = NULL;
	if (cmdParser->numParams()>0) {							// If we have any params.
		heapStr(&strBuff,cmdParser->getParamBuff());		// Grab local copy.
		theWPObj = ourWPList->findWPByName(strBuff);
		*/
}



void  navTest::doSortNames(void) {

	char* sortTxt;
	
	ourWPList->setSortOrder(byName);						// Default sort order is by name.
	if (cmdParser->numParams()) {							// If we have any params.
		sortTxt = NULL;										// Start local string at NULL.
		heapStr(&sortTxt,cmdParser->getNextParam());	// Grab local copy of the first param.
		if (!strcmp(sortTxt,"distance")) {				// Did they type distance?
			ourWPList->setSortOrder(byDistance);		// Sort by distance.
		} else if (!strcmp(sortTxt,"bearing")) {		// Did they say bearing?
			ourWPList->setSortOrder(byBearing);			// Sort by bearing.
		}															// 
		freeStr(&sortTxt);										// Recycle our string.
	}																//
	ourWPList->sort(true);									// Do the sort.
	doListWP();													// Show the result.
}
	

void  navTest::doReadWPFile() {

	ourWPList->readList(FILE_PATH);
	Serial.println("File read.");
}


void  navTest::doSaveWPFile() {

	ourWPList->saveList(FILE_PATH);
	Serial.println("File saved, I think.");
}


// **************************************************
// 					LOW LEVEL FILE STUFF
// **************************************************


bool navTest::checkFile(pathItem*	item) {

  if (item->name[0]=='.') return false;
  return true;
}


pathPrefix navTest::decodePrefix(const char* param) {

  delay(3);                                                 // Dash said to put this in to fix wicked bug. I did, it worked. No clue!
  if (!strcmp(param,"/")) return setRoot;                   // Just "/" alone means root. setRoot.
  else if (param[0]=='/') return fullPath;                  // Starts with '/' + more..   fullPath.
  else if (!strcmp(param,"..")) return upOne;               // Just ".." means go up one. upOne.
  else if (param[0]=='.' && param[1]=='.') return upNPath;  // Starts with ".." + more..  upNPath.
  else return relPath;                                      // Default, relitive path.    relPath.
}


// [ls] Lists all the files in the working direcotory.
void navTest::listDirectory(void) {

	pathItem*		trace;

	if (cmdParser->numParams()) {												// If they typed in something past the command.
		Serial.println("Sorry, ls takes no params. Ignoring..");		// Let 'em know..
	}																					//
	if (wd.getPathType()==fileType) {										// If this is pointing to a file..? 
		Serial.println("Sorry, this is a file, not a directory.");	// Just wrong, we can't list a file.
		return;                                                     // Just walk away..
	} else {																			// Else, not a file. Good!
		wd.refreshChildList();													// Make sure we have an up to date child list..
		if (wd.numChildItems()==0) {											// Wait, no children to list?
			Serial.println("This directory is empty.");					// Tell the user.
			return;																	// And walk away.
		} else {																		// Else, we DO have kids to list.
			trace = wd.getChildList();											// Grab a pointer to the first child.
			while(trace) {															// While we have a non-NULL pointer..
				if (checkFile(trace)) {											// Pass this child through the crucible of the user's filter function.
					Serial.print(trace->getName());							// If passed, print it's name.
					if (trace->getType()!=fileType) {						// If it's a directory..
						Serial.print("/");										// Add the "I'm a direcotry" slash to it.
					}																	// 
					Serial.println();												// Line feed..
				}																		//
				trace = (pathItem*)trace->dllNext;							// Jump to the next item on the list.
			}																			//
		}																				//
	}																					//
}


// [mkdir] Create a new directory in the working directory using typed in parameter.
void navTest::makeDirectory(void) {

  tempStr param;
  int     numBytes;
  char*   pathBuff;

  pathBuff = NULL;                                // Make SURE these start at NULL.
  if (cmdParser->numParams()==1) {                 // If they typed in something past the command.
    param.setStr(cmdParser->getParamBuff());       // We get the first parameter. Should be the new folder's name/path.
    switch (decodePrefix(param.getStr())) {       // Let's see what they are asking for..
      case setRoot  :                             // Create root..
      case upOne    :                             // Pop up a direcoty..
      case upNPath  :                             // Pop up and go elsewhere..
        Serial.println("Illegal command");        // Not going to deal with any of those for now.
      break;                                      // And we're off.
      case fullPath :                             // Full path, we can do that.
        if(!SD.mkdir(param.getStr())) {           // If we can't make the direcotry..
          Serial.println("File error.");          // Tell 'em..
        }                                         //
      break;                                      // In any case, we're off.
      case relPath  :                             // Relitive path..
        numBytes = wd.numPathBytes();             // We need our wd path..
        numBytes = numBytes + param.numChars();   // And the param .
        if (resizeBuff(numBytes,&pathBuff)) {     // If we can get the RAM..
          strcpy(pathBuff,wd.getPath());          // Stuff in our wd path.
          strcat(pathBuff,param.getStr());        // Add the param text.
          if(!SD.mkdir(pathBuff)) {               // If we can't make the direcotry..
            Serial.println("File error.");        // Tell 'em..
          }                                       //
          resizeBuff(0,&pathBuff);                // recycle the RAM.
        }                                         //
      break;                                      // In any case, we're off.
    }
  }
}


// [cd] Change the working directory. (See prefix commands above.)
void navTest::changeDirectory(void) {

  tempStr       param;
  tempStr       savedPath;
 
  savedPath.setStr(wd.getPath());                           // Save the path we have, just in case.
  if (cmdParser->numParams()==1) {                           // If they typed in a parameter (name/path).
    param.setStr(cmdParser->getParamBuff());                 // We grab the name.
    switch(decodePrefix(param.getStr())) {                  // Decide on how the param is prefixed..
      case setRoot  :                                       // Just set to root..
        //Serial.println("Setting root");
        wd.setPath("/");                                    // Easy peasy!
      break;                                                //
      case upOne    :                                       // Want to go up one deirectory..
        //Serial.println("upOne");
        if (wd.getPathType()==rootType) {                   // If we're at the root already..
          Serial.println("Sorry, already at root.");        // Send error. Can't one up more root.
        } else {                                            // Else, not root?
          wd.popItem();                                     // Go ahead and pop off the item.
        }                                                   //
      break;                                                //
      case upNPath  :                                       // Up then, relitive path..
        //Serial.println("upNPath");
        if (wd.getPathType()==rootType) {                   // If we're at the root already..
          Serial.println("Sorry, already at root.");        // Send error. Can't one up more root.
        } else {                                            // Else, not root?
          wd.popItem();                                     // Go ahead and pop off the item.
          if (!wd.addPath(&(param.getStr()[2]))) {          // If we can't set this concatinated path..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path not found.");              // Error.
          } else {                                          // Else, we got a path. Check it out.
            if (wd.getPathType()==fileType) {               // If this is pointing to a file..
              wd.setPath(savedPath.getStr());               // Put it all back..
              Serial.println("Path is not a directory.");   // Working DIRECTORY can't be a file.
            }                                               //
          }                                                 //
        }                                                   //
      break;                                                // Done!
      case fullPath :                                       // Full path..
        //Serial.println("fullPath");
        if (!wd.setPath(param.getStr())) {                  // If we can't set this path..
          wd.setPath(savedPath.getStr());                   // Put it all back..
          Serial.println("Path not found.");                // Error.
        } else {                                            // Else, we got a path, check it out.
          if (wd.getPathType()==fileType) {                 // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path is not a directory.");     // Working DIRECTORY can't be a file.
          }                                                 //
        }                                                   //
      break;                                                // Skipping off..
      case relPath  :                                       // Relitive path..
         //Serial.println("relPath");
        if (wd.addPath(param.getStr())) {                   // If we can add this relitive path bit..
          if (wd.getCurrItem()->getType()==fileType) {      // If this is pointing to a file..
            wd.setPath(savedPath.getStr());                 // Put it all back..
            Serial.println("Path is not a directory.");     // Let 'em know.
          }                                                 //
          //Serial.print("Added:");
          Serial.println(param.getStr());
        } else {                                            // Else, couldin't find the file.
          wd.setPath(savedPath.getStr());                   // Put it all back..
          Serial.println("Path not found.");                // Let 'em know.
        }                                                   //
      break;                                                // We're off..
      default : Serial.println("Should never get here!");
    }
  }
}


//[rm] Delete a file or directory..
void navTest::deleteItem(void) {

  tempStr param;
  tempStr savedPath;
 
  savedPath.setStr(wd.getPath());             // Save the path we have, just in case.
  if (cmdParser->numParams()==1) {             // If they typed in a parameter (name).
    param.setStr(cmdParser->getParamBuff());   // We grab the name.
    switch(decodePrefix(param.getStr())) {    // Decide on how the param is prefixed..
      case setRoot  :                         // Just set to root..
      case upOne    :                         // Want to go up one deirectory..
      case upNPath  :                         // Up then, relitive path..
      case fullPath :                         // Full path..
        Serial.println("Illegal command.");   // We don't do those with delete.
      break;                                  // Skipping off..
      case relPath  :                         // Relitive path..
        if (wd.addPath(param.getStr())) {     // If we can add this relitive path bit..
          if(!wd.deleteCurrentItem()) {       // If we failed to delete the item..
            Serial.println("File error.");    // Something went wrong.
          }                                   //
        } else {                              // Else, couldin't find the file.
          Serial.println("Path not found.");  // Let 'em know.
        }                                     //
        wd.setPath(savedPath.getStr());       // Put it all back..
      break;                                  // Enough!
    }
  }
}


// **************************************************
// 					HELP TEXT
// **************************************************


void navTest::printHelp(void) {

	Serial.println("This is the testbed for developing the waypoint database.");
	Serial.println();
	Serial.println("Commands :");
	Serial.println("addWP    - followed by [name][lat][lon] as decimal degrees, adds a new waypoint.");
	Serial.println("editWP   - followed by [name], edits a new waypoint. Somehow..");
	Serial.println("listWP   - prints out a list of the waypoints.");
	Serial.println("deleteWP - followed by [name], deletes a waypoint.");
	Serial.println("sortWP   - followed by name, distance or bearing. Sorts the list in that way.");
	Serial.println("saveWP   - Saves our current list of waypoints to disk.");
	Serial.println("readWP   - Replaces our current list of waypoints with those on disk.");
	Serial.println();
	Serial.println("Low level file commands..");
	Serial.println("pwd           show working directory.");
	Serial.println("ls            list items in the working directory.");
	Serial.println("cd *path*     Can be fullpath starting at '/' or");
	Serial.println("              reletive path startig with just a");
	Serial.println("              directoty. Or .. to go back to");
	Serial.println("              parent directory.");
	Serial.println("mkdir *path*  Can be fullpath starting at '/' or"); 
	Serial.println("              reletive path startig with just a");
	Serial.println("              directoty.");
	Serial.println("rm *path*     Can be fullpath starting at '/' or"); 
	Serial.println("              reletive path starting with just a");
	Serial.println("              directoty. Will delete files or");
	Serial.println("              directories. Directories do NOT need");
	Serial.println("              to be empty, this will empty them.");
	Serial.println("              NOTE: Be careful with this. Emptying");
	Serial.println("              directories can eat a lot of RAM if");
	Serial.println("              they are nested deeply.");
}


      	
      	
      	
      	
      	