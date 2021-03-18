#include <docFileObj.h>
#include <resizeBuff.h>
#include <timeObj.h>


#define TEMP_NAME_CHARS	8	// The max num chars for the temp file name 9999.tmp
#define PATH_MIN_CHARS	5	// Min chars is X.tmp

char*	tempDir = NULL;

// This will be called automatically when needed and it will use the default folder name.
// If you want a different location you can call this in setup() with a different name and
// all your document files will use that.
bool createTempDir(char* dirPath) {

	File	theDir;
	int	numChars;
	bool	addSlash;
	bool	success;
	
	if (tempDir) return true;							// It there already is a temp directory set up, bail!
	addSlash = false;
	success = false;
	// WAIT!! Patching bug here. If you pass name/ and it fails, it crashes! No '/' no crash.
	numChars = strlen(dirPath);                  // Lets see how long the path is.
	if (dirPath[numChars-1]=='/') {					// If it has has a traileing '/'..            
		dirPath[numChars-1]='\0';						// Clip off the trailing '/'.
	} 
	theDir=SD.open(dirPath,FILE_READ);				// Lets try to open the path they gave us.
	if (theDir) {											// If we were able to open the path they gave us..
		if (theDir.isDirectory()) {               // If its a directory..
			numChars = strlen(dirPath);            // Lets see how long the path is now.
			if (resizeBuff(numChars+2,&tempDir)) {	// If we can allocate the tempDir path string..
				strcpy(tempDir,dirPath);				// Copy in the path we got.
				strcat(tempDir,'/');						// Plus the '/' we pulled off of it.
				success = true;							// Note our success.
			}
		}
		theDir.close();									// Close the directory we opened.
	} else {													// Else there was no directory of that name..
		if (SD.mkdir(dirPath)) {							// If we can create the directory.
			numChars = strlen(dirPath);            // Lets see how long the path is.
			if (dirPath[numChars-1]!='/') {			// If we DON'T have a traileing '/'.. 
				numChars++;									// Add one for the '/'.
				addSlash = true;							// Note that we'll need to add a '/'.
			}
			if (resizeBuff(numChars+1,&tempDir)) {	// If we can allocate the tempDir path string..
				strcpy(tempDir,dirPath);				// Copy in the path we got.
				if (addSlash) strcat(tempDir,'/');	// Plus the '/' if it was missing.
				success = true;							// Note our success.
			}
		}
	}
	return success;
}



// ************************************************************
// ********************  docFileObj class  ********************
// ************************************************************

		
docFileObj::docFileObj(void) {

	docFilePath		= NULL;		// Dynamic things start at NULL.
	tempFilePath	= NULL;		// Dynamic things start at NULL.
	haveInfo			= false;		// And we have no info at all as yet.
}


docFileObj::~docFileObj(void) {

	resizeBuff(0,&docFilePath);		// Recycle the docFilePath.
	if (tempFilePath) {					// If we have a temp file..
		SD.remove(tempFilePath);		// Erase the temp file.
		resizeBuff(0,&tempFilePath);	// Recycle the tempFilePath.
	}
}


// Create a new blank document. This removes all reference to any file we are working
// with, deletes any temp file we had. Then, sets up a new temp file and calls
// initNewTempFile() to initialize it.
bool docFileObj::newDocFile(void){
	
	bool	success;
	
	success = false;							// As always, not a success yet.
	resizeBuff(0,&docFilePath);			// New file? At this point we have no document file.
	haveInfo = false;							// 'Cause from now we don't.
	if (createTempFile()) {					// If we have or can create a temp file..	
		if (initNewTempFile()) {			// If we can init this new working file.. (New docs gets this call.)
			haveInfo = true;					// If the initTempFile() was a success, then we have info.
			success = true;					// And this was a complete success!
		}
	}
	return success;							// return our success or failure.
}


// Called by user to open a new document. If there is already a document that this object
// is associated with, clear everything out and start over fresh.
bool docFileObj::openDocFile(char* filePath) {

	int	numChars;
	bool	success;
	
	success = false;												// Not a success yet..
	if (filePath) {												// If they gave us a non NULL path pointer..
		numChars = strlen(filePath);							// Count the number of chars in the file path.
		if (numChars>PATH_MIN_CHARS) {						// If the file path is > than the path minium.
			haveInfo = false;										// No longer setup.
			if (tempFilePath) {									// If we currently have a tempFile..
				SD.remove(tempFilePath);						// Erase the temp file.
				resizeBuff(0,&tempFilePath);					// Recycle the tempFilePath.	
			}
			if (resizeBuff(numChars+1,&docFilePath)) {	// If we get the RAM to store the path..
				strcpy(docFilePath,filePath);					// Save off the path.
				haveInfo = checkDoc();							// See if we can open and understand the document file.
				success = haveInfo;								// If we have info, we'll call that a success.
			}
		}
	}
	return success;												// Tell the user.
}


// Take the tempFile, if any and past it over the old docFile. Passing in a file path will
// redirect to a new docFile and this will be what we will be editing from now on.
bool docFileObj::saveDocFile(char* newFilePath=NULL) {

	File	tempFile;
	File	docFile;
	int	numBytes;
	bool	success;
	
	success = false;													// Not a success yet.
	if (!tempFilePath) return false;								// No temp path? This is just nuts! Bail.
	if (newFilePath) {												// If we are saving to a new file path..
		docFile = SD.open(newFilePath,FILE_WRITE);			// Have a go at opening the new file path.
	} else if (docFilePath) {										// Else, we need to save to the original path..
		docFile = SD.open(docFilePath,FILE_WRITE);			// Have a go at opening the original file path.
	}
	if (docFile) {													// If we were able to open a file to save to..
		tempFile = SD.open(tempFilePath,FILE_READ);		// Try to open up the temp file.
		if (tempFile) {											// If we were successful in opening up the temp file..
			while(tempFile.available()) {						// While not at the end of the temp file..
				docFile.write(tempFile.read());				// Read a byte from the temp file and write it to the doc file.
			}
			tempFile.close();										// We opened the temp file, we close it!
			if (newFilePath) {									// If we are using a new file path..
				numBytes = strlen(newFilePath) + 1;			// Calculate the number of byte for the new doc file path.
				if (resizeBuff(numBytes,&docFilePath)) {	// If we can re-allocate the RAM.
					strcpy(docFilePath,newFilePath);			// Save off our new docFile path.
					success = checkDoc();						// And, if we can parse the new document file. we'll call that a success!
				}
			} else {
				success = true;									// Else we're using the original file, we're calling this a success.
			}
		}
		docFile.close();											// We opened the save file, we close it!
	}
}
				

// When the inherited opens up a document, this will be called to see if its parsable as
// the kind of file the class is looking for.
bool docFileObj::checkDoc(void) { return false; }


// This finds a unused file name in the tempFolder then checks to make sure it can be
// created/opened. Once this is accomplished, it closes the tempFile saves off the path to
// this file and returns its success or failure.
bool docFileObj::createTempFile(void) {
	
	File		tempFile;
	timeObj	timeOut(FILE_SEARCH_MS);
	char*		pathBuff;
	char		fileNumStr[TEMP_NAME_CHARS];
	int		fileNum;
	int		numBytes;
	bool		done;
	bool		success;
	
	if (tempFilePath) return true;										// If we already have a temp path set up? Bail.
	success = false;															// As always, not a success yet.
	if (createTempDir(TEMP_FOLDER)) {									// Ok we don't have a temp path. So, if we have or can create a temp directory..
		pathBuff = NULL;														// Start it at NULL so resizeBuff will work correctly.
		numBytes = strlen(tempDir)+TEMP_NAME_CHARS + 1;				//	Calculate the max RAM we'll need for the path buff.
		if (resizeBuff(numBytes,&pathBuff)) {							// If we can allocate the RAM.
			done = false;														// Not done yet.
			fileNum = 0;														// File names are a number string with ".tmp" at the end.
			do {																	// We loop around doing..
				strcpy(pathBuff,tempDir);									//	Start building the file path with the temp directory.
				itoa(fileNum++,fileNumStr,10);							// Create a number string.
				strcat(pathBuff,fileNumStr);								// Add the file num to the path.
				strcat(pathBuff,".tmp");									// Add ".tmp" to the path.
				tempFile = SD.open(pathBuff,FILE_READ);				//	Try to open this file for reading.
				if (tempFile) {												// If the file opened..
					tempFile.close();											// We just close it and move on.
				} else {															// Else, we have a possible candidate here.
					done = true;												// Either its the real deal or an error. In any case, we are done. 
					tempFile = SD.open(pathBuff,FILE_WRITE);			// Try to create the file we couldn't open.
					if (tempFile) {											// If we were able to create the file..
						tempFile.close();										// Close it.
						numBytes = strlen(pathBuff)+1;					// Grab the num chars of the path +1.
						if (resizeBuff(numBytes,&tempFilePath)) {		// If we can allocate the tempFilePath c string..
							strcpy(tempFilePath,pathBuff);				// Save off the path that worked.	
							success = true;									// And We'll call that a success!
						}
					}
				}
			} while(!done && !timeOut.ding());							// Loop while we are not done. (And have time)
			resizeBuff(0,&pathBuff);										// Recycle the build-a-path buffer.
		}
	}
	return success;															// return if we had success or not.
}


// Does nothing now, but it can be inherited to setup a newly created temp file that has
// no document file to copy from.
bool docFileObj::initNewTempFile(void) { return true; }


// Takes the docFile and copies it over to the tempFile giving the user somewhere to put
// her changes.
bool docFileObj::copyToTempFile(void) {

	File	tempFile;
	File	docFile;
	bool	success;

	success = false;												// We're not a success yet.
	if (haveInfo) {												// If we have checked and ok'ed our doc file..
		if (createTempFile()) {									// If we can/or have set up a temp file.
			tempFile = SD.open(tempFilePath,FILE_WRITE);	// Try to open the temp file for writing.
			if (tempFile) {										// If we opened the temp file..
				docFile = SD.open(docFilePath,FILE_READ);	// Try to open the doc file for reading.
				if (docFile) {										// If we opened the doc file..
					while(docFile.available()) {				// While w're not at the EOF of the image file..
						tempFile.write(docFile.read());		// Write this byte from the doc file to the temp file.
					}
					success = true;								// Ok, we'll call this a success.
					docFile.close();								// We opened it, we close it.
				}
				tempFile.close();									// Again, we opened it, we close it.
			}
		}
	}
	return success;												// Return our success or failure.
}



										
					
