#include <fOpenObj.h>


// **************************************************************
// ********************* dynamicPath  stuff *********************
// **************************************************************


// ***** pathItem
// This will have the default actions.

pathItem::pathItem(pathItemType itemType)
	:dblLinkListObj() {
	
	ourType		= noType;	// No type, but then, we never get created.
	name[0]		= '\0';		// Empty string. Again we never get created.
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
void pathItem::fillChildList(fileItem* childList) {

	char*			ourPath;
	char			root[2] = "/";
	int			numBytes;
	File     	dir;                                // File handle used for the current directory.
   File     	entry;                              // File handle used for the different entries of the current directory.
   pathItem*	newItem;                            // Its what we create.
   bool			done;                               // A boolean used to track when to stop all this nonsense.
	
	ourPath = NULL;
	switch (ourType) {
		case rootType		:
			ourPath = root;
		break;
		case folderType	:
			numBytes = getTotalPathChars() + 1;
			if(!resizeBuff(nuumBytes,&ourPath)) {
				return;
			} else {
				if (SD.exists(ourPath)) {											// If the directory exists..
					dir = SD.open(ourPath);											// Try opening the directory.
					if (dir.isDirectory()) {										// If it is a directory..
						dir.rewindDirectory();                 				// Rewind it to the first entry.
						done = false;                          				// We ain't done yet.
						do {                                   				// Start looping through the entries.
							entry = dir.openNextFile();							// Grab an entry.
							if (entry) {												// If we got an entry..
								if (entry.isDirectory()) {							// If it is a directory..
									newItem = new folderItem(entry.name());	// Create the folderItem.
								} else {													// Else its a file..
									newItem = new fileItem(entry.name());		// Create the new fileItem.
								}
								entry.close();                					// And we close the entry.	
								if (childList) { 										// If there is a list..
									newItem->linkToEnd(childList);				// Link yourself to the end of this list.
								} else {													// Else there is no list.. (yet)
									childList = newItem;								// You are the list!
								}
							} else {                            // Else, we didn't get an entry from above.
								done = true;                     // No entry means, we are done here.
							}
						} while (!done);                       // And we do this loop, over and over, while we are not done.
					} else {												// Else, this was a datafile.
						Serial.println("Not a directory.");		// Print an error.
					}
					dir.close();                              // Looping through entries is done, close up the original file.
				} else {                                     // If this worked correctly, we'd know there was an error at this point.                                        
					Serial.print("Can't find : ");     			// Can't find it.
					Serial.printnln(path);							//
				}
			}
		break;
		case fileType		:
		case noType			:return;

}


// ***** rootItem
// All paths start with one.	
	
	
rootItem::rootItem(void)
	:pathItem(rootType) { strcpy(name,"/"); }
	
rootItem::~rootItem(void) {  }
	
	
int rootItem::getTotalPathChars(void) { return 1; }


// We are building a text path. The buffer is big enough, just add yourself to the end.
void rootItem::addNameToPath(char* path) { strcpy(path,name); }



// ***** fileItem
// These are so useful.


fileItem::fileItem(char* fileName)
	:pathItem(fileType) { strcpy(name,fileName); }
	

// Files don't have children. So, do nothing.	
fileItem::~fileItem(void) { }
	
	
// Again, files don't have children. So, do nothing.
void pathItem::fillChildList(void) {  }


// Same as before.. Anyone calling this is nuts anyway.
void pathItem::addNextChild(pathItem* childItem) { }


// ***** folderItem


class folderItem : public	pathItem {
	
folderItem::folderItem(char* fileName)
	:pathItem(folderType) { strcpy(name,fileName); }


folderItem::~folderItem(void) {  }
	

// Folders in paths have trailing '/'. So we add this in.	
void folderItem::addNameToPath(char* path) {  

	strcat(tempName,name);
	strcat(tempName,"/");
}	
	
// Num bytes in name + one for the '/'.	
int folderItem::getNumPathBytes(void) { return strlen(name) + 1; }



// ***** dynamicPath



dynamicPath::dynamicPath(char* inPath) {

	pathList		= NULL;
	childList	= NULL;
	path			= NULL;
}


dynamicPath::~dynamicPath(void) { reset(); }


// Same effect as just dumping everything.
void dynamicPath::reset(void) {
	
	if (pathList) {
		pathList->dumpList();
		pathList = NULL;
	}
	if (childList) {
		childList->dumpList();
		childList = NULL;
	}
	resizeBuff(0,&path);
}
 
 	
bool dynamicPath::pathToList(char* inPath) {
	
	rootItem*	theRoot;
	folderItem*	aFolder;
	fileItem*	aFile;
	char*			pathPtr;
	char			itemName[13];
	bool			success;
	bool			fail;
	
	
	succcess = false;														// We have not succeeded yet..
	fail		= false;														// Nor have we failed.
	reset();																	// Clear out and reset everything.
	if (inPath) {															// If we have been given a non-NULL path..
		if (inPath[0]=='/') {											// If the path we got stars with '/'.. (It better)
			theRoot = new rootItem();									// We start the list with a root item.
			if (theRoot) {													// If we got the rootItem..
				pathList = theRoot;										// We start the list with the root item.
				if (!strcmp(inPath,"/")) {								// If the path is just "/"..
					succcess = true;										// It was just the root. So we are done!
				} else {														// Else, there's more to check and do..
					nIndex = 0;												// Set nIndex for reading in a name.
					pIndex = 1;												// Set pIndex for reading out chars.
					while(pathPtr[pIndex]!='\0'&&!fail) {			// While we have chars to read and we have not failed..
						if (pathPtr[pIndex]=='/') {					// If the path pointer is pointing at a '/'..
							if (nIndex>0) {								// If we have a non empty name string..
								aFolder = new folderItem(itemName);	// Have a go at creating a folderItem.	
								if (aFolder) {								// If we got our folderItem..
									aFolder->linkToEnd(theRoot);		// Link it to the end of our list.
									nIndex = 0;								// And reset the itemName index.
								} else {										// Else, we didn't get a folderItem..
									fail = true;							// We fail! On no!!
								}
							} else {											// Else, we have a 0 length name?
								fail = true;								// Again, we fail. 
							}
						} else {												// Else, we're not looking at a '/'..
							itemName[nIndex]=inPath[pIndex];			// Pop the char into the name string.
							nIndex++;										// Bump up the name string index.
							itemName[nIndex]='\0';						// Pop in a 0 char just in case this is the last one.
							pIndex++;										// Bump up the path string index.
						}
					}
					if (!fail) {
						if (nIndex>0) {									// If we did not fail, and we have a non empty name string..				
							aFile = new fileItem(itemName);			// Must be a file. Have a shot at creating a fileItem.
							if (aFile) {									// If we got our fileItem..
								aFile->linkToEnd(theRoot);				// Link it to the end of our list.
								success = true;							// Wow we made it! We are a success!
							}
						} else {
							success = true;								// Else, No file name? That's good too!
						}
					}
				}
			}		
		}
	}
	if (!success) {														// If we were NOT successful..
		reset();																// Clear out and reset everything.
	}
	return success;														// In any case we return our result.
}


char* dynamicPath::listToPath(void) {
	
	int			numBytes;
	fileItem*	trace;
	
	if (pathList) {
		trace = pathList->getLast();
		if (trace) {
			numBytes = trace->getTotalPathChars() + 1;
			if (resizeBuff(numBytes,&path)) {
				trace = pathList;
				while(trace) {
					trace->addNameToPath(path);
					trace = trace->dllNext;
				}
				return path;
			}
		}
	}
	path[0] = '\0';
	return path;
}


// If one has a list of child items to choose from. This grabs the one at index, pulls it
// from the list and adds it to the end of the path list.
bool dynamicPath::nextItemByIndex(int index) {

	fileItem* theNewGuy;
	bool			success;
	
	success = false;
	if (childList) {
		theNewGuy = childList->getTailObj(index);
		if (theNewGuy) {
			theNewGuy->unhook();
			success = addEndItem(theNewGuy);
		}
	}
	return success
}


// Have a fileItem? This will add it to the end of your path list. (If possible)	
bool dynamicPath::pushItem(fileItem* theNewGuy) {

	rootItem*	aRoot;
	bool			success;
	
	success = false;
	if (theNewGuy) {
		if (pathList) {
			theNewGuy->linkToEnd(pathList);
			success = true;
		} else {
			if (theNewGuy->getType()==rootType) {
				pathList = theNewGuy;
				success = true;
			} else {
				aRoot = new rootItem();
				if (aRoot) {
					pathList = aRoot;
					theNewGuy->linkToEnd(pathList);
					success = true;
				} 			
			}
		}
	}
	if (success && theNewGuy->getType()!=fileType) {
		if (childList) {
			childList->dumpList();
			childList = NULL;
		}
		theNewGuy->fillChildList(childList);
	}
	return success;
}


bool dynamicPath::popItem(void) {

	fileItem*	theLastGuy;
	
	if (pathList) {
		theLastGuy = theLastGuypathList->getLast();
		if (theLastGuy) {
			if (pathList==theLastGuy) {
				delete (theLastGuy);
				pathList = NULL;
			} else {
				delete (theLastGuy);
				theLastGuy = theLastGuypathList->getLast();
				if (childList) {
					childList->dumpList();
					childList = NULL;
				}
				theLastGuy->fillChildList(childList);
			}
		}
	}
}
			

	
// **************************************************************
// ********************** fOpenObj stuff **********************
// **************************************************************


fOpenPane::fOpenObj(lilOS* ourOS,char* inPath)
	:panel(ourOS,fOpenApp,noMenuBar) {
	
	filterFx	= NULL;
	resultFx	= NULL;
	ourPath	= NULL;
	tempPath	= NULL;
	if (inPath && strlen(inPath)) {	// If we have non NULL and non empty path..
		numBytes = strlen(inPath) + 1;
		if (resizeBuff(numBytes,&tempPath)) {
			strcpy(tempPath,inPath);
		}
	}
}
	
	
fOpenPane::~fOpenPane(void) {  }

	
void fOpenPane::setup(void) {

	ourPath = new dynamicPath();
	ourPath->pathToList(tempPath);
	Serial.println(tempPath);
	resizeBuff(0,&tempPath);
	Serial.println(ourPath->listToPath());
}	
		
// Use a callback to filter what you see.
void fOpenPane::setFilterCallback(bool(*funct)(char*)) { filter = funct; }


// Get a call back with the result.
void fOpenPane::setResultCallback(void(*funct)(bool))  { result = funct; }