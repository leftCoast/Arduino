
#include <filePath.h>
#include <SD.h>
#include <resizeBuff.h>

void upCase(char* inStr) {

	int	i;
	
	i=0;
	while(inStr[i]) {
		inStr[i] = toupper(inStr[i]);
		i++;
	}
}


// ***** pathItem
// This will have the default actions.

pathItem::pathItem(void)
	:dblLinkListObj() {
	
	ourType		= noType;	
	name[0]		= '\0';
}

// No matter what we become, we'll need to dump this..
pathItem::~pathItem(void) { }
	

// Everyone will have a path type. This is how we get around not having dynamic_cast.	
pathItemType pathItem::getType(void) { return ourType; }


// The name of this, Root, folder or file. The name you would use for a file listing.
char* pathItem::getName(void) { return name; }


// How many chars do we use for our path name? Default is just the chars for our name.
// Folders will be more. But again, this is a good default.
int pathItem::getNumPathChars(void) {  return strlen(name); }


// Ok this gets all recursive here. How many chars do we need to display the full path to
// this item? So the full path is our parent's full path plus our path name chars.
// NOTE : THIS MUST BE POINTING AT THE TAIL OF THE PATH. NOT THE HEAD. IE : THE LAST ITEM.
int pathItem::getTotalPathChars(void) { return getParent()->getTotalPathChars() + getNumPathChars(); }


// We are building a text path. The buffer is big enough, just add yourself to the end.
// NOTE: THIS ONE STARTS AT THE HEAD OF THE LIST. IE:THE FIRST ITEM.
void pathItem::addNameToPath(char* path) { strcat(path,name); }

	
// Return a pointer to our parent object.
pathItem* pathItem::getParent(void) { return (pathItem*)dllPrev; }


// Fill child list. This one will contain all the file code to take this path and read out
// the basically do a directory listing. Each item gets some sort of path item created for
// it and added into the list.
pathItem* pathItem::fillChildList(void) {

	char*			ourPath;
	int			numBytes;
	File     	dir;                                // File handle used for the current directory.
   File     	entry;                              // File handle used for the different entries of the current directory.
   pathItem*	trace;
   pathItem*	newItem;                            // Its what we create.
   pathItem*	childList;
   bool			done;                               // A boolean used to track when to stop all this nonsense.
	
	ourPath = NULL;
	childList = NULL;
	switch (ourType) {
		case rootType		:																	// For root..
		case folderType	:																	// And folders..
			numBytes = getTotalPathChars() + 1;											// Calculate the amount of RAM we need for the path.
			if(resizeBuff(numBytes,&ourPath)) {											// If we can get the RAM for our path string..
				trace = (pathItem*)getFirst();											// Grab a link to our root node..
				while(trace) {																	// While trace isn't NULL..
					trace->addNameToPath(ourPath);										// Add trace's path text to the path.
					trace = (pathItem*)trace->dllNext;									// Jump to the next path link in our list.
				}
				if (SD.exists(ourPath)) {													// If the directory exists..
					dir = SD.open(ourPath);													// Try opening the directory.
					if (dir.isDirectory()) {												// If it is a directory..
						dir.rewindDirectory();                 						// Rewind it to the first entry.
						done = false;                          						// We ain't done yet.
						do {                                   						// Start looping through the entries.
							entry = dir.openNextFile();									// Grab an entry.
							if (entry) {														// If we got an entry..
								if (strlen(entry.name()) <13) {								// If the entry name is 8.3 or shorter..
									if (entry.isDirectory()) {									// If it is a directory..
										newItem = new folderItem((char*)entry.name());	// Create the folderItem.
									} else {															// Else its a file..
										newItem = new fileItem((char*)entry.name());		// Create the new fileItem.
									}
									if (childList) { 												// If there is a list..
										newItem->linkToEnd(childList);						// Link yourself to the end of this list.
									} else {															// Else there is no list.. (yet)
										childList = newItem;										// You are the list!
									}
								}
								entry.close();                							// And we close the entry.	
							} else {                            						// Else, we didn't get an entry from above.
								done = true;                     						// No entry means, we are done here.
							}
						} while (!done);                       						// And we do this loop, over and over, while we are not done.
					} else {																		// Else, this was a datafile.
						Serial.println("Not a directory.");								// Print an error.
					}
					dir.close();                              						// Looping through entries is done, close up the original file.
				} else {                                     						// If this worked correctly, we'd know there was an error at this point.                                        
					Serial.print("Can't find : ");     									// Can't find it.
					Serial.println(ourPath);												// Print our path.
				}
			} else {
				Serial.print("Not enough RAM for path.");     						// Path is just too long!
			}
		break;
		case fileType		:
		case noType			: break;															// For fileType & noType, do nothing.
	}
	return childList;
}


// ***** rootItem
// All paths start with one.	
	
	
rootItem::rootItem(void)
	:pathItem() {
	
	ourType = rootType;
	strcpy(name,"/");
}
	
	
rootItem::~rootItem(void) {  }
	
	
int rootItem::getTotalPathChars(void) { return 1; }


// We are building a text path. The buffer is big enough, just add yourself to the end.
void rootItem::addNameToPath(char* path) { strcpy(path,name); }



// ***** fileItem
// These are so useful.


fileItem::fileItem(char* fileName)
	:pathItem() {
	
	ourType = fileType;
	strcpy(name,fileName);
}
	

// Files don't have children. So, do nothing.	
fileItem::~fileItem(void) { }
	
	
// Again, files don't have children. So, do nothing.
pathItem* fileItem::fillChildList(void) { return NULL; }


// Same as before.. Anyone calling this is nuts anyway.
void fileItem::addNextChild(pathItem* childItem) { }



// ***** folderItem

	
folderItem::folderItem(char* folderName)
	:pathItem() {
	
	ourType = folderType;
	strcpy(name,folderName);
}


folderItem::~folderItem(void) {  }
	

// Folders in paths have trailing '/'. So we add this in.	
void folderItem::addNameToPath(char* path) {  

	strcat(path,name);
	strcat(path,"/");
}	
	
// Num bytes in name + one for the '/'.	
int folderItem::getNumPathBytes(void) { return strlen(name) + 1; }



// ***** filePath



filePath::filePath(void) {

	pathList		= NULL;
	childList	= NULL;
	path			= NULL;
}


filePath::~filePath(void) { reset(); }


// Same effect as just dumping everything.
void filePath::reset(void) {
	
	if (pathList) {
		pathList->dumpList();
		delete(pathList);
		pathList = NULL;
	}
	if (childList) {
		childList->dumpList();
		delete(childList);
		childList = NULL;
	}
	resizeBuff(0,&path);
}
 
 
// Ok, kinda' a special here. -IF- we were to add this name to our current pathList.. What
// do we end up with? A file? A folder? Nothing at all? Find out and return the answer.
pathItemType filePath::checkPathPlus(char* name) {

	File				testFile;
	pathItem*		trace;
	char*				testPath;
	int				numBytes;
	pathItemType	theType;
	
	testPath = NULL;
	theType = noType;
	if (pathList) {
		trace = (fileItem*)pathList->getLast();
		if (trace) {
			numBytes = trace->getTotalPathChars() + strlen(name) + 1;
			if (resizeBuff(numBytes,&testPath)) {
				trace = pathList;
				while(trace) {
					trace->addNameToPath(testPath);
					trace = (pathItem*)trace->dllNext;
				}
				strcat(testPath,name);
				if (SD.exists(testPath)) {
					testFile = SD.open(testPath);													// Try opening the directory.
					if (testFile) {
						if (testFile.isDirectory()) {
							theType = folderType;
						} else {
							theType = fileType;
						}
						testFile.close();
					}
				}
				resizeBuff(0,&testPath);
			}
		}
	}
	return theType;
}


// This is used for setting the initial path for browsing. Paths must start with '/'
// because they all start at root. They Must also fit in 8.3 file types, because SD is
// brain dead and we're stuck with it for now. If the path is NOT found on the SD card,
// this fails and gives back a false. This does NOT fill out the childList. Only the pathList
bool filePath::setPath(char* inPath) {
	
	rootItem*	theRoot;
	folderItem*	aFolder;
	fileItem*	aFile;
	char			itemName[13];
	int			nIndex;
	int			pIndex;
	bool			fail;
	
	fail		= false;														// We've not failed. Yet..
	reset();																	// Clear out and reset everything.
	if (inPath) {															// If we have been given a non-NULL path..
		theRoot = new rootItem();										// We start the list with a root item.
		if (theRoot) {														// If we got the rootItem..
			pathList = theRoot;											// We start the list with the root item.
			if (inPath[0]=='/') {
				pIndex = 1;													// Set pIndex for reading out chars.
			} else {															// Else, there's no leading '/'?
				fail = true;												// Seriously, no leading '/'. We can't work with these people!
			}
			nIndex = 0;														// Set nIndex for reading in a name.
			while(inPath[pIndex]!='\0'&&!fail) {					// While we have chars to read and we have not failed..
				if (inPath[pIndex]=='/') {								// If we have run across a '/' and not failed..
					switch (checkPathPlus(itemName)) {
						case folderType :
							aFolder = new folderItem(itemName);		// Have a go at creating a folderItem.	
							if (aFolder) {									// If we got our folderItem..
								aFolder->linkToEnd(pathList);			// Link it to the end of our list.
								nIndex = 0;									// And reset the itemName index.
							} else {
								fail = true;								// We can't get the RAM, we fail!
							}
						break;
						case fileType	:									// Preceding a '/' must not be a file type.
						case noType		:  fail = true; break;		// And noType means the path did not exist.
					}
				} else {
					if (nIndex<12) {									
						itemName[nIndex]=inPath[pIndex];				// Pop the char into the name string.
						nIndex++;											// Bump up the name string index.
						itemName[nIndex]='\0';							// Pop in a 0 char just in case this is the last one.
						pIndex++;											// Bump up the path string index.
					} else {													// Else we are being asked to put in too long of a name.
						fail = true;										// Not doing this today, call foul!
					}
				}
			}
			if (!fail) {													// If we did not fail..
				if (nIndex>0) {											// If we have a non empty name string..
					switch (checkPathPlus(itemName)) {				// See what we find..
						case folderType :									// If its a folder..
							aFolder = new folderItem(itemName);		// Have a go at creating a folderItem.	
							if (aFolder) {									// If we got our folderItem..
								aFolder->linkToEnd(pathList);			// Link it to the end of our list.
							} else {
								fail = true;								// We can't get the RAM, we fail!
							}
						break;
						case fileType	:
							aFile = new fileItem(itemName);			// Have a go at creating a folderItem.	
							if (aFile) {									// If we got our folderItem..
								aFile->linkToEnd(pathList);			// Link it to the end of our list.
							} else {
								fail = true;								// We can't get the RAM, we fail!
							}
						break;
						case noType		:  fail = true; break;		// And noType means the path did not exist.
					}
				}
			}		
		}
	}
	if (fail) {																// If we were NOT successful..
		reset();																// Clear out and reset everything.
	}
	refreshChildList();													// You need this for a success, won't hurt on fail.
	return !fail;															// In any case we return our result.
}


char* filePath::getPath(void) {
	
	int			numBytes;
	pathItem*	trace;
	
	if (pathList) {
		trace = (fileItem*)pathList->getLast();
		if (trace) {
			numBytes = trace->getTotalPathChars() + 1;
			if (resizeBuff(numBytes,&path)) {
				trace = pathList;
				while(trace) {
					trace->addNameToPath(path);
					trace = (pathItem*)trace->dllNext;
				}
				return path;
			}
		}
	}
	path[0] = '\0';
	return path;
}


// Lets see what's at the end of the list we currently have. This will pass back a pointer
// to it for your to have a peek at it. Maybe ask what kind of thing it is, possibly
// what's it name?
pathItem* filePath::getCurrItem(void) {

	if (pathList) {
		return (pathItem*)pathList->getLast();
	}
	return NULL;
}


// If we have a current item. Pass back a pointer to it's name. (Don't mess with it!)
char* filePath::getCurrItemName(void) {

	pathItem*	currItem;
	
	currItem = getCurrItem();
	if (currItem) {
		return currItem->getName();
	} 
	return NULL;
}


// We need to refresh the current child list. Dump what we have and fill it with what we
// got.
void filePath::refreshChildList(void) {

	pathItem* currentItem;
	
	if (childList) {
		childList->dumpList();
		delete(childList);
		childList = NULL;
	}
	currentItem = getCurrItem();
	if (currentItem) {
		childList = currentItem->fillChildList();
	}
}


// Lest see how many kids we have..
int filePath::numChildItems(void) {

	if (childList) {								// If the pointer is Non NULL..
		return childList->countTail() + 1;	// Gotta' add one for the first one.
	}
	return 0;										// Otherwise we pass back a big zero.
}


// We want to SEE the child with this name. This does NOT unhook the child. It just gives
// you a pointer to it. You can look, but if you want to do something with it, you better
// unhook() it. first.
pathItem*  filePath::getChildItemByName(char* name) {

	pathItem*	trace;
	int			numBytes;
	char*			tempName;
	char			fileName[13];
	bool			success;
	
	success = false;											// Not a success yet.
	tempName = NULL;											// Start tempName at NULL.
	numBytes = strlen(name)+1;								// How many bytes will we need to copy the passed in name?
	if (resizeBuff(numBytes,&tempName)) {				// If we can allocate a buffer for the passed in name..
		strcpy(tempName,name);								// Make a local copy of it..
		upCase(tempName);										// Capitalize it.
		trace = childList;									// Point trace out the child list.
		while(trace && !success) {							// While trace is non_NULL, and we ain't found the named node..
			strcpy(fileName,trace->name);					// Make local copy of the node's name.
			upCase(fileName);									// Capitalize it.
			if (!strcmp(fileName,tempName)) {			// If the names match..
				success = true;								// Then we are a success!
			} else {												// Else, no match..
				trace = (pathItem*)trace->dllNext;		// So, jump to the next node to check.
			}
		}
		resizeBuff(0,&tempName);							// Done with our copy of the file name. recycle it.
	}
	if (success) {												// In the end if we had success.
		return trace;											// Trace is pointing to the node, so return trace.
	}
	return NULL;												// If we got here we didn't find it. Pass back NULL.
}
			
	
// If one has a list of child items to choose from. This grabs the one with this passed
// in name, pulls it from the list and adds it to the end of the path list.	
bool   filePath::pushChildItemByName(char* name) {

	pathItem*	theChild;
	bool			success;
	
	success = false;									// Not a success yet.
	theChild = getChildItemByName(name);		// Try grabbing a child by this name.
	if (theChild) {									// If we were able to find such child node..
		theChild->unhook();							// Unhook this child from the pathList.
		success = pushItem(theChild);				// Try putting the child at the end of the pathList. Save off the result.
	}
	return success;									// report or success. (Or failure)
}


// We want to SEE the child #index. (Start at 0 like an array). This does NOT unhook the
// child. It just gives you a pointer to it. You can look, but if you want to do something
// with it you better unhook() it. first.
pathItem* filePath::getChildItemByIndex(int index) {

	pathItem*	theChild;
	
	if (childList) {															// If we have a non-NULL child list..
		if (index==0) {														// If index = 0, special case..
			return childList;													// Its the child list itself.
		} else {																	// Else, we want index of tail, not us..
			theChild = (pathItem*)childList->getTailObj(index-1);	// We grab for the node at # index. (see definition of getTailObj())
			return theChild;													// Get one or not, we pass back the result.
		}
	}
	return NULL;																// If we got here, we failed. So pass back NULL.
}


// If one has a list of child items to choose from. This grabs the one at index, pulls it
// from the list and adds it to the end of the path list.
bool filePath::pushChildItemByIndex(int index) {

	pathItem* theNewGuy;
	bool			success;
	
	success = false;									// Not a success yet.
	theNewGuy = getChildItemByIndex(index);	// We make a grab for the node at (index) of this list..
	if (theNewGuy) {									// If we got a node..
		theNewGuy->unhook();							// We unhook the new guy from the child list.
		success = pushItem(theNewGuy);			// And link him into the pathList. Save if successful or not.
	}
	return success;									// Report our success to the world.
}


// Have a fileItem? This will add it to the end of your path list. (If possible)	
bool filePath::pushItem(pathItem* theNewGuy) {

	rootItem*	aRoot;
	bool			success;
	
	success = false;											// Not a success yet.
	if (theNewGuy) {											// If we got a non-NULL new guy..
		if (pathList) {										// If we have a non-NULL pathList..
			theNewGuy->linkToEnd(pathList);				// Tell the new guy to link hisself to the end of the list.
			success = true;									// Shit howdy! Looks ike we done did it!
		} else {													// Else, we had an empty (NULL) pathList..
			if (theNewGuy->getType()==rootType) {		// If this new guy is a rootType..
				pathList = theNewGuy;						// Point the pathList at this new guy.
				success = true;								// And there we go. A success!
			} else {												// Else, NOT a root type?..
				aRoot = new rootItem();						// We'll fix it. Try to create a root item.
				if (aRoot) {									// If we got the root item..
					pathList = aRoot;							// Point the pathList at this new root.
					theNewGuy->linkToEnd(pathList);		// Then tell the new item to link itself to the end of the list.
					success = true;							// And again, we pulled this mess out of the fire.
				} 			
			}
		}
	}
	refreshChildList();											// This will at least clean out the old list.
	return success;
}


// Pop off the last item from your path list. Now, Do we really want to pop off the root
// item? Because now it will. Seems that the list should at lest contain the root. Oh
// well, we'll run it like this and see how it goes.
void filePath::popItem(void) {
	 
	pathItem*	theLastGuy;
	
	if (pathList) {														// If we have a pathList to work with..
		theLastGuy = (pathItem*)pathList->getLast();				// We grab the last item on the list.
		if (theLastGuy) {													// If we got an item.. (I can't see how we couldn't, really..)
			if (pathList==theLastGuy) {								// If the pathList is actually pointing to the last guy.. IE the root.
				pathList = NULL;											// Set pathList to NULL. Because its going to be empty.
			}
			delete (theLastGuy);											// Delete the last node.
		}
	}
	refreshChildList();														// This will at least clean out the old list.
}
			

	