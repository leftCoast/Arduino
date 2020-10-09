#include "SDTools.h"
#include <resizeBuff.h>

#define	BUFF_BYTES	80			// How many bytes to allocate for the name buff.


char  nameBuff[BUFF_BYTES];  // Use it as you please. Its shared between all.


// Groups like Apple will fill the SD drive with all sorts of junk we do't need
// to see. This function's job is to filter it out. As best as it can.
bool filterFile(char* fileName) {

   //if (strstr(fileName,"~1"))							return false;  // No Apple clones.
   if (fileName[0]=='.')								return false;  // No invisables.
   if (fileName[0]=='_'&&strchr(fileName,'~'))	return false;  // No weird underscores.
   if (!strcmp(fileName,"TRASHE~1"))				return false;
   if (!strcmp(fileName,"FSEVEN~1"))				return false;
   if (!strcmp(fileName,"SPOTLI~1"))				return false;
   return true;
}


// *******************************************************
//                      strItem class
// *******************************************************


strItem::strItem(void)
	: linkListObj() { mStr = NULL; }
	
	
strItem::strItem(char* inStr)
	: linkListObj() {
	
	mStr = NULL;
	setStr(inStr);
}

   
strItem::~strItem(void) { resizeBuff(0,&mStr); }


bool strItem::setStr(char* inStr) {

	int numBytes;
	
	if (inStr) {									// If they passed us a non NUL string..
		numBytes = strlen(inStr)+1;			// Calculate the RAM needed.
      if (resizeBuff(numBytes,&mStr)) {	// If we can get RAM to store the string in..
      	strcpy(mStr,inStr);             	// Stuff in the string.
      	return true;
      }
   } else {											// Else, we got a NULL string..
       resizeBuff(0,&mStr);					// Recycle the current string, set to NULL.
	}
	return false;
}  

            
char* strItem::getStr(void) { return mStr; }


int strItem::getStrlen(void) { return strlen(mStr); }


// *******************************************************
//                      pathStr class
// *******************************************************


pathStr::pathStr(void)
	: linkList() {  }

pathStr::~pathStr(void) {  }
   
   
int pathStr::getPathBytes(void) {
	
	int 		numBytes;
	strItem*	trace;
	
	numBytes = 1;											// We need one for initial "/".
	trace = (strItem*)getFirst();						// Starting at beginning.
	while(trace) {											// While we have a valid pointer..
		numBytes = numBytes + trace->getStrlen();	// Add the numb chars for each item.
		numBytes++;											// Add one for the slash.
		trace = (strItem*)trace->getNext();			// Jump to the next item.
	}
	numBytes++;												// And one for '\0';
	return numBytes;										
}


void pathStr::getPath(char* path) {

	strItem*	trace;
	
	strcpy(path,"/");
	trace = (strItem*)getFirst();
	while(trace) {
		strcat(path,trace->getStr());
		if (trace->getNext()) {
			strcat(path,"/");
		}
		trace = (strItem*)trace->getNext();
	}
}


char* pathStr::getPath(void) {

	getPath(&nameBuff[0]);
	return nameBuff;
}


void pathStr::setPath(char* inStr) {

	int 		numItems;
	strItem*	trace;
	char*		aStr;
	char*		anItem;
	int		numBytes;
	
	aStr = NULL;													// ALWAYS init pointers to be used with resizeBuff to NULL.
	if (!inStr) {													// If we were passed a NULL..
		dumpList();													// Basically we just dump everything.
	} else if (!strcmp(inStr,"/")) {							// If we were just passed a /..
		dumpList();													// Again, dup it all.
	} else if (!strcmp(inStr,"..")) {						// If were passed the string ".." Means up one.
		numItems = getCount();									// Count the number of items in our list.
		if (numItems>1) {											// If we have more than one..
			trace = (strItem*)getByIndex(numItems-2);		// We point at the second from the last.
			if (trace) {											// Sanity, this is not NULL right?
				trace->deleteTail();								// Delete the last item.
			}
		} else {														// Else, we have only one or none..
			dumpList();												// In either case, dumping it all will suffice.
		}
	} else {															// Else, we have been a full or relative path to parse.
		if (inStr[0]=='/') {										// Starts with a slash, full path.
			dumpList();												// Full path, start fresh.
		}																// Now the method of full or relative is the same.
		numBytes = strlen(inStr)+1;							// How big is this incoming path?
		if (resizeBuff(numBytes,&aStr)) {					// If we can get RAM to store the path in..
      	strcpy(aStr,inStr);             					// Stuff in the buffer.
			anItem = strtok(aStr,"/");							// First slice HI-YA!
  			while (anItem != NULL) {							// While we are getting tokens..
  				trace = new strItem(anItem);					// Use the token to generate a list item.
  				addToEnd(trace);									// Add the new list itme to our list.
  				anItem = strtok (NULL,"/");					// See if we get another.
  			}
  			resizeBuff(0,&aStr);									// And we dump the sliced up buffer.
  		}
  	}		
}



// *******************************************************
//                      dirItem class
// *******************************************************


dirItem::dirItem(File inItem)
   : strItem() {
   
   setStr(inItem.name());						// Grab the name.
   mIsDir = inItem.isDirectory();         // Save off if its a directory or not.
   if (mIsDir) {                          // If it is a diectory..
      mNumBytes = 0;                      // It has zero bytes.
   } else {                               // Else, its not a directory..
      mNumBytes = inItem.size();          // Save off the file size.
   }
}

  
dirItem::~dirItem(void) { }
	
   
char* dirItem::getName(void) { return getStr(); }


unsigned long dirItem::getSize(void) { return mNumBytes; }


bool dirItem::getIsDir(void) { return mIsDir; }



// *******************************************************
//                      dirList class
// *******************************************************


dirList::dirList(void)
   : linkList() { 
   
   mLastIndex = -1;
   mLastItem = NULL; 
}
   
   
dirList::~dirList(void) {  }
   
  
void dirList::readDir(char* path) {

   File     dir;                                // File handle used for the current directory.
   File     entry;                              // File handle used for the different entries of the current directory.
   dirItem* newItem;                            // Its what we create.
   bool     done;                               // A boolean used to track when to stop all this nonsense.

   dumpList();                                  // Clear the list for a new set.
   mLastIndex = -1;                             // Flag for no search yet.
   mLastItem = NULL;                            // Flag nothing found yet.
   dir = SD.open(path);                         // Try opening the directory.
   if (dir) {                                   // If we were able to open the working directory..
      dir.rewindDirectory();                    // Rewind it to the first entry.
      done = false;                             // We ain't done yet.
      do {                                      // Start looping through the entries.
         entry = dir.openNextFile();            // Grab an entry.
         if (entry) {                           // If we got an entry..
            strcpy(nameBuff,entry.name());      // Grab the name.
            if (filterFile(nameBuff)) {         // filter out Apple nonsense.
               newItem = new dirItem(entry);    // Create the list item.
               addToTop(newItem);               // Pop it in the list.
               entry.close();                   // And we close the entry.
            }
         } else {                               // Else, we didn't get an entry from above.
            done = true;                        // No entry means, we are done here.
         }
      } while (!done);                          // And we do this loop, over and over, while we are not done.
      dir.close();                              // Looping through entries is done, close up the original file.
   } else {                                     // If this worked correctly, we'd know there was an error at this point.                                        
      Serial.println("Fail to open file.");     // Sadly, instead of returning a NULL, it just crashes.
   }
}


dirItem* dirList::findItem(int index) {

   dirItem* anItem;
   
   if (index == mLastIndex) {
      anItem = mLastItem; 
   } else {
      anItem = (dirItem*)getByIndex(index);
      mLastIndex = index;
      mLastItem = anItem;
   }
   return anItem;
}


char* dirList::getItemName(int index) {

   dirItem* anItem;
   
   anItem = (dirItem*)findItem(index);
   if (anItem) {
      return anItem->getName();
   }
   return NULL;
}


unsigned long dirList::getItemSize(int index) {

   dirItem* anItem;
   
   anItem = (dirItem*)findItem(index);
   if (anItem) {
      return anItem->getSize();
   }
   return NULL;
}


bool dirList::getItemIsDir(int index) {

   dirItem* anItem;
   
   anItem = (dirItem*)findItem(index);
   if (anItem) {
      return anItem->getIsDir();
   }
   return false;
}


/*
// *******************************************************
//                         sdObj class
// *******************************************************


sdObj::sdObj(int inCS) {

	mCS	= inCS;
	mInit = false;
	mWd	= NULL;
}


sdObj::~sdObj(void) { resizeBuff(0,&mWd); }


bool sdObj::begin(void) {

	mInit = SD.begin(mCS);
	setWD("/");
	return mInit;
}

  
char* sdObj::getWD(void) { return mWd; }
 
 
// Create a new directory in the working directory.
void sdObj:makeDirectory(char* inName) {
   
   	int   numChars;
   	
   	numChars = WdToBuff();								// Load temp buffer with the path. Get num chars.
      strcat(nameBuff,inName);                   	// Add in the user's parameter.
      if (!SD.mkdir(nameBuff)) {                   // If we can not create the folder.
         Serial.println("Can't create folder.");   // We'll send back an error.
      }
   }
}


// This one's a bit of a pain. Too many special cases and bug patches. Change the working
// directory. Pass in ".." to go up a level.
void sdObj:changeWD(char* dirName) {

   File  theDir;                                      // We may need to check a directory.
   int   numChars;                                    // A place to save the lengh of a string.
   int   index;                                       // A handy index to use, messing with strings.
   	
   	numChars = WdToBuff();									// Load temp buffer with the path. Get num chars.
		if (!strcmp(dirName,"/")) {								// If its just '/' IE, root..
         strcpy(nameBuff,"/");								// Give it to them and be done with it.
		} else if (!strcmp(inStr,"..")) {					// If we need to go up a directory from wd..
         if (numChars>2) {                            // If more than 2 chars, Shortest possible path is 3 chars..
            index = numChars-2;                       // Back up past the '\0' and the trailing slash.
            while(nameBuff[index]!='/') index--;		// Run backwards 'till we see the next slash.
            nameBuff[index+1] = '\0';						// Truncate the string at this point. Leaving the slash.
         }
      } else {                                        // Else, they typed in some sort of path..
         if (dirName[0]!='/') {									// If the parameter does NOT start with '/'. They're going relative!
            strcat(nameBuff,dirName);							// add the relative path.
         } else {                                     // Else, it DID start with '/'. Absolute path!
            strcpy(nameBuff,dirName);							// Just use what they typed in.
         }
         
         // WAIT!! Patching bug here. If you pass name/ and it fails, it crashes! No '/' no crash.
         numChars = strlen(nameBuff);                  // Lets see how long the path is now.
         if (nameBuff[numChars-1]=='/') {              // If we have a traileing '/'..            
            nameBuff[numChars-1]='\0';                 // Clip off the trailing, and yes correct, '/'.
         }
         
         if (theDir=SD.open(nameBuff,FILE_READ)) {		// If we can open the path they gave us..
            if (theDir.isDirectory()) {               // If its a directory..
               numChars = strlen(nameBuff);				// Lets see how long the path is now.
               if (nameBuff[numChars-1]!='/') {			// If the last char is NOT a '/'..
                  strcat(nameBuff,"/");					// Add one..
               }
               buffToWd();                    			// Success! copy the nameBuff to the working dir.
            } else {
               Serial.println("No, thats a file");    // They tried to pull a fast one on us.
            }
            theDir.close();                           // Done with the directory. Close it.
         } else {
            Serial.println("Can't find it.");         // Something went wrong.
         }
      }
   }   
}


bool sdObj:deleteItem(char* path) {

	File		theItem;												// We may need to check a directory.
	dirList	ourList;
	
	if (path) {
		if (strlen(path)>0) {
			strcpy(nameBuff,path);
			 															// Patching bug here. If you pass name/ and it fails, it crashes! No '/' no crash.
         numChars = strlen(nameBuff);						// Lets see how long the path is now.
         if (nameBuff[numChars-1]=='/') {					// If we have a traileing '/'..            
            nameBuff[numChars-1]='\0';						// Clip off the trailing, and yes correct, '/'.
         }
			if (theItem=SD.open(nameBuff,FILE_READ)) {	// If we can open the path they gave us..
				if (theItem.isDirectory()) {					// If its a directory..
					theItem.close();								// Free up RAM, we'll need it!
					ourList.readDir(path);						// Grab a list of item names.
					while(!ourList.isEmpty()) {
						strcpy(nameBuff,path);
						strcat(nameBuff,ourList.getItemName(0));
						

}



int WdToBuff(void) {

	strcpy(nameBuff,mWd);
	return strlen(nameBuff);
}


void buffToWd(void) { sdetWD(nameBuff); }


bool sdObj::setWD(char* newWd) {

	int	numBytes;
	
	numBytes = strlen(newWd)+1;
	if (resizeBuff(numBytes,&mWd)) {		// If we can get RAM to store the working directory in..
      strcpy(mWd,newWd);        			// Stuff in the path.
   }
}           

 */          
