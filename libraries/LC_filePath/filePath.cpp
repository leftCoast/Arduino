
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


pathItem::pathItem(pathItem* aGrandItem)
	:dblLinkListObj() {
	
	ourType		= aGrandItem->ourType;	
	strcpy(name,aGrandItem->name);
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

/*
// We are building a text path. The buffer is big enough, just add yourself to the end.
// NOTE: THIS ONE STARTS AT THE HEAD OF THE LIST. IE:THE FIRST ITEM.
void pathItem::addNameToPath(char* path) {

	Serial.print("ITEM adding name to path. ");
	Serial.println((int)ourType);
	Serial.print("Starting with [");
	Serial.print(path);
	Serial.println("]");
	strcat(path,name);
	Serial.print("Ending with [");
	Serial.print(path);
	Serial.println("]");
}
*/
	
// Return a pointer to our parent object.
pathItem* pathItem::getParent(void) { return (pathItem*)dllPrev; }


// Return a pointer to our next neighbor.
pathItem* pathItem::getNext(void) { return (pathItem*)dllNext; }



// ***** rootItem
// All paths start with one.	
	
	
rootItem::rootItem(void)
	:pathItem() {
	
	ourType = rootType;
	strcpy(name,"/");
}
	
	
rootItem::~rootItem(void) {  }
	
	
int rootItem::getNumPathChars(void) { return 1; }


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



void fileItem::addNameToPath(char* path) { strcat(path,name); }
	
	
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
int folderItem::getNumPathChars(void) { return strlen(name) + 1; }



// ***** filePath



filePath::filePath(void) {

	pathList		= NULL;
	childList	= NULL;
	path			= NULL;
}


filePath::~filePath(void) { reset(); }


// Same effect as just dumping everything.
void filePath::reset(void) {
	
	if (childList) {					// If we have a non NULL child list..
		childList->dumpList();		// We dump all the nodes attached to this list node.
		delete(childList);			// Then we delete the childList pointer node.
		childList = NULL;				// And NULL it out;
	}
	if (pathList) {					// If we have a non NULL path list..
		pathList->dumpList();		// We dump all the nodes attached to this list node.
		delete(pathList);				// Then we delete the list pointer node.
		pathList = NULL;				// And NULL out the list pointer.
	}
	resizeBuff(0,&path);				// And our temporary path buffer is recycled.
}
 
 
int filePath::numPathBytes(void) {

 	pathItem*	trace;
 	int			numBytes;
 	
 	numBytes = 0;														// Zero out the counter.
 	trace = pathList;													// Move trace to the head of our path list.
 	while(trace) {														// While trace is not NULL, do..
 		numBytes = numBytes + trace->getNumPathChars();		// Add this node's num chars to the counter.
		trace = (pathItem*)trace->dllNext;						// Jump to the next node.
	}
	numBytes++;															// Add one for the '\0'.
	return numBytes;													// Return the number of chars..
 }
 
 
// Ok, kinda' a special here. -IF- we were to add this name to our current pathList.. What
// do we end up with? A file? A folder? Nothing at all? Find out and return the answer.
pathItemType filePath::checkPathPlus(char* name) {

	File				testFile;
	pathItem*		trace;
	char*				testPath;
	int				numBytes;
	pathItemType	theType;
	
	testPath = NULL;															// NULL out the string we'll be allocating.
	theType = noType;															// Set default type as non-type.
	numBytes = numPathBytes() + strlen(name);							// Calc. the total number of bytes needed.
	if (resizeBuff(numBytes,&testPath)) {								// If we can grab the memory for this path..
		trace = pathList;														// Move trace to the head of our path list.
		while(trace) {															// While trace is not NULL do..
			trace->addNameToPath(testPath);								// We add this node's name to the path.
			trace = trace->getNext();
		}
		strcat(testPath,name);												// Now we add this new name to this path string..
		testFile = SD.open(testPath,FILE_READ);						// Try opening the constructed path.
		if (testFile) {														// If this constructed path exists..
			if (testFile.isDirectory()) {									// If the returned file is a directory..
				theType = folderType;										// We set our type to folderType.
			} else {																// Else, not a folder..
				theType = fileType;											// We set our type to fileType.
			}
			testFile.close();													// Close the file/folder.
		}
		resizeBuff(0,&testPath);											// And recycle the path string.
	}
	return theType;															// And return our results.
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
	
	fail		= false;												// We've not failed. Yet..
	reset();															// Clear out and reset everything.
	if (inPath) {													// If we have been given a non-NULL path..
		if (inPath[0]=='/') {									// Of said path string starts with '/'..
			theRoot = new rootItem();							// First thing is to grab a new root node.
			if (theRoot) {											// If we got the root node..
				pathList = theRoot;								// Start the list with it.
				pIndex = 1;											// Set pIndex for reading out chars.
			} else {													// Else, no root node? Ran out of RAM!
				return false;										// Return false, we failed.
			}
		} else {														// Else, there's no leading '/'?
			return false;											// Seriously, no leading '/'? We can't work with these people!
		}
	} else {															// Else, they handed us a NULL string?!?
		return false;												// Where are all these Bozos coming from?
	}
	nIndex = 0;														// Set nIndex for reading in a name.
	while(inPath[pIndex]!='\0'&&!fail) {					// While we have chars to read and we have not failed..
		if (inPath[pIndex]=='/') {								// If we have run across a '/'..
			switch (checkPathPlus(itemName)) {				// itemName has the folder name, see if it exists and is of type..
				case folderType :									// Folder! All is well!
					aFolder = new folderItem(itemName);		// Have a go at creating a folderItem.	
					if (aFolder) {									// If we got our folderItem..
						aFolder->linkToEnd(pathList);			// Link it to the end of our list.
						nIndex = 0;									// And reset the itemName index.
					} else {
						fail = true;								// We can't get the RAM, we fail!
					}
				break;
				case fileType	:									// Preceding a '/' must not be a file type.
				case noType		:  fail = true;	break;	// And noType means the path did not exist.
			}
			pIndex++;
		} else {														// Else, it wasn't a '/' so it "must" be a name char..
			if (nIndex<12) {										// If we've not run out of name chars..								
				itemName[nIndex]=inPath[pIndex];				// Pop the char into the name string.
				nIndex++;											// Bump up the name string index.
				itemName[nIndex]='\0';							// Pop in a 0 char just in case this is the last one.
				pIndex++;											// Bump up the path string index.
			} else {													// Else, we are being asked to put in too long of a name.
				fail = true;										// Not doing this today, call foul!
			}
		}
	}																	// At this point we've gone through all the folders. We MAY have a file at the end. We'll check.
	if (!fail) {													// If we did not fail..
		if (nIndex>0) {											// If we have a non empty name string..
			switch (checkPathPlus(itemName)) {				// See what we find..
				case folderType :									// If its a folder? (Really we should fail, but for now, we'll fix it.)
					aFolder = new folderItem(itemName);		// Have a go at creating a folderItem.	
					if (aFolder) {									// If we got our folderItem..
						aFolder->linkToEnd(pathList);			// Link it to the end of our list.
					} else {											// Else, we were unable to allocate a folder item.
						fail = true;								// We can't get the RAM, we fail!
					}
				break;												// Last folder is done, bolt!
				case fileType	:
					aFile = new fileItem(itemName);			// Have a go at creating a fileItem.	
					if (aFile) {									// If we got our fileItem..
						aFile->linkToEnd(pathList);			// Link it to the end of our list.
					} else {											// Else, we were unable to allocate a file item.
						fail = true;								// We can't get the RAM, we fail!
					}
				break;												// Last file is complete.
				case noType		:  fail = true;	break;	// And noType means the path did not exist. Its a fail.
			}
		}
	}		
	if (fail) {														// If we were NOT successful..
		reset();														// Clear out and reset everything.
	}
	refreshChildList();											// You need this for a success. Won't hurt on fail.
	return !fail;													// In any case we return our result.
}


// We'll allocate this, but we'll also reuse the memory. So be careful. It won't last.
char* filePath::getPath(void) {
	
	pathItem*	trace;
	
	resizeBuff(0,&path);										// Clear the last path string.
	if (pathList) {											// If we have a path list..
		if (resizeBuff(numPathBytes(),&path)) {		// Grab the RAM needed to store the path string.
			trace = pathList;									// Gab the root of the path.
			while(trace) {										// While we're pointing to a path node..
				trace->addNameToPath(path);				// Add this nodes name to the string.
				trace = (pathItem*)trace->dllNext;		// And jump to the next node.
			}
		}
	}
	return path;													// Return the result. 
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

	char*			ourPath;
	File			dir;
	File			entry;
	pathItem*	newItem;
	bool			done;
	
	if (childList) {																		// If we currently have a child list..
		childList->dumpList();															// Dump the tail of the list.
		delete(childList);																// Dump what the list is pointing to.
		childList = NULL;																	// Null out the list variable.
	}
	ourPath = getPath();																	// Yeah, I know this is just a pointer to our path member.
	if (ourPath) {																			// If we got our path string..
		dir = SD.open(ourPath);															// Try opening the directory.
		if (dir) {																			// If our directory exists..
			if (dir.isDirectory()) {													// If it is a directory..
				dir.rewindDirectory();                 							// Rewind it to the first entry.
				done = false;                          							// We ain't done yet.
				do {                                   							// Start looping through the entries.
					entry = dir.openNextFile();										// Grab an entry.
					if (entry) {															// If we got an entry..
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
						entry.close();                								// And we close the entry.	
					} else {                            							// Else, we didn't get an entry from above.
						done = true;                     							// No entry means, we are done here.
					}
				} while (!done);                       							// And we do this loop, over and over, while we are not done.
			} else {																			// Else, this was a datafile.
				Serial.println("Not a directory.");									// Print an error.
			}
			dir.close();                              							// Looping through entries is done, close up the original file.
		} else {                                     							// If this worked correctly, we'd know there was an error at this point.                                        
			Serial.print("Can't open : [");     									// Can't find it.
			Serial.print(ourPath);														// Print our path.
			Serial.println("]");															// 
		}
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
// you a pointer to it. You can look, but don't mess with it!
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
// in name, copies it, then adds the copy to the end of the path list.	
bool   filePath::pushChildItemByName(char* name) {

	pathItem*	theChild;
	pathItem*	newItem;
	bool			success;
	
	success = false;									// Not a success yet.
	theChild = getChildItemByName(name);		// Try grabbing a child by this name.
	if (theChild) {									// If we were able to find such child node..
		switch(theChild->getType()) {
			case rootType		: newItem = new rootItem(); break;
			case fileType		: newItem = new fileItem(theChild->name); break;
			case folderType	: newItem = new folderItem(theChild->name); break;
			case noType			: newItem = NULL; break;
		}
		if (newItem) {									// If we were able to allocate the new item..
			success = pushItem(newItem);			// Try putting the new item at the end of the pathList. Save off the result.
			if (!success) {
				delete(newItem);
			}
		}
	}
	return success;									// report or success. (Or failure)
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
			}
		}
	}
	refreshChildList();										// This will at least clean out the old list.
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
	refreshChildList();													// This will at least clean out the old list.
}
			

	