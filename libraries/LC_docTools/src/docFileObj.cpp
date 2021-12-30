#include <docFileObj.h>
#include <resizeBuff.h>
#include <timeObj.h>

#include <debug.h>


// Returns true if this folderPath can be found, or created.
bool createFolder(const char* folderPath) {

	File	theDir;
	int	numChars;
	bool	success;
	char*	dirPath;
	
	success = false;											// If we started with a success, why start?
	if (folderPath) {											// If we got a path. (Sanity)
		numChars = strlen(folderPath);					// Lets see how long the path is.
		if (folderPath[numChars-1]=='/') {				// Paths end in '/'. Make sure this has that.
			dirPath = NULL;									// Dynamic starts with NULL.
			if (resizeBuff(numChars+1,&dirPath)) {		// WAIT!! Patching bug here. If you pass 
				strcpy(dirPath,folderPath);				// "name/" and it fails, it crashes!
				success = false;								// No '/' no crash.
				dirPath[numChars]='\0';						// Clip off the trailing '/'.
				theDir=SD.open(dirPath,FILE_READ);		// Lets try to open the path they gave us.
				if (theDir) {									// If we were able to open the path they gave us..
					success = theDir.isDirectory();		// If its a directory, its a success.
					theDir.close();							// Close the directory we opened.
				} else {											// Else, there was no directory of that name..
					success = SD.mkdir(dirPath);			// If we can create the directory, its a success.
				}													//
				resizeBuff(0,&dirPath);						// Toss the temp buff.
			}														//
		}															//
	}																//
	return success;											// Return whether we were a success or not?
}																	


// Given a path, baseName and extension this hands back a string with a path to an unused
// numbered file. For example "/docs/NoName5.doc". IF it can not allocated this file it
// will return NULL. IF THIS IS A SUCCESS, YOU MUST DE-ALLOCATE THE RETURNED STRING.
char* numberedFilePath(const char* folderPath,const char* baseName,const char* extension) {

	File	tempFile;
	char* result;
	int	numBytes;
	int	maxNum;
	int	fileNum;
	char	numStr[8];
	bool	done;
	
	result = NULL;														// As always, dynamic stuff starts at NULL.
	if (strlen(baseName)<8 											// If the params make sense..
		&& strlen(extension)<=4 									//
		&& folderPath[0]=='/') {									//
		if (createFolder(folderPath)) {							// If we can find/create the folder..
			maxNum = pow(10,8 - strlen(baseName));				// How many chars we got for a value?
			maxNum--;													// Actually, you get one too many.
			numBytes = strlen(folderPath) + 8 + 4 + 1;		// Path, max name, max extension, '\0'.
			if (resizeBuff(numBytes,&result)) {					// If we can get the RAM..
				fileNum = 1;											// Starting at one.
				done = false;											// 'Cause we ain't.
				do {														// Do for each..
					itoa(fileNum++,numStr,7);						// Setup a number string.
					strcpy(result,folderPath);						// Build up the test path.
					strcat(result,baseName);						// Add the base name.
					strcat(result,numStr);							// Add the number string.
					strcat(result,extension);						// Add the extension.
					tempFile = SD.open(result,FILE_READ);		//	Try to open this file for reading.
					if (tempFile) {									// If the file opened..
						tempFile.close();								// We just close it and move on.
					} else {												// Else, we have a possible candidate here.
						done = true;									// Either its the real deal or an error. In any case, we are done. 
						tempFile = SD.open(result,FILE_WRITE);	// Try to create the file we couldn't open.
						if (tempFile) {								// If we were able to create the file..
							tempFile.close();							// Close it.
							return result;								// And We'll call that a success!
						}													//
					}														//
				} while(!done && fileNum<maxNum);				// Loop while we are not done. (And have numbers to go.)
				resizeBuff(0,&result);								// If we get here, its a failure so recycle the RAM.
			}																//
		}																	//
	}																		//
	return result;														// And this'll be returning a NULL.
}
	



// TOTAL HACK TO GET AROUND NO FILE truncate() CALL..
File TRUNCATE_FILE(char* path) {
	
	SD.remove(path);							// Delete the file. So now it has zero bytes of data.
	return SD.open(path,FILE_WRITE);		// Re-open the file. Now its been truncated. (Sigh..)
}													// AND NOW WE HAVE A TRUNCATED FILE.
							

// fcpy() : The file version of strcpy(). The dest file must be open for writing. The src
// file must be, at least, open for reading. (Writing is ok too) The dest file index is
// left pointing to the end of the file. The src file index is not changed.
void fcpy(File dest,File src) {
	
	unsigned long	filePos;
	maxBuff	cpyBuff(src.size());
	unsigned long	numBytes;
	unsigned long	remaingBytes;
	
	
	filePos = src.position();				// Lets save the file pos for miss user.
	src.seek(0);								// Point at first byte of the src file.
	
	// HACK, THERE IS NO truncate() CALL SO WE DO IT BEFORE THIS CALL.
	//dest.truncate();							// Clear out the old.
	// AND THERE YOU GO..
	
	remaingBytes = src.size();
	for (int i=0;i<cpyBuff.numPasses;i++) {
		numBytes = min(cpyBuff.numBuffBytes,remaingBytes);
		src.read(cpyBuff.theBuff,numBytes);
		dest.write((char*)(cpyBuff.theBuff),numBytes);
		remaingBytes = remaingBytes - numBytes;
	}
	src.seek(filePos);						// Put it back like we found it.						
}


// fcat() : The file version of strcat(). The dest file must be open for writing. The src
// file must be, at least, open for reading. (Writing is ok too) The dest file index is
// left pointing to the end of the file. The src file index is not changed.
void fcat(File dest,File src) {
	
	uint32_t	filePos;
	uint32_t	numBytes;
	
	numBytes = dest.size();					// How many bytes we talking here?
	dest.seek(numBytes);						// Point at end of the dest file.
	filePos = src.position();				// Lets save the file pos for miss user.
	src.seek(0);								// Point at first byte of the src file.
	while(src.available()) {				// While not at the end of the src file..
		dest.write(src.read());				// Write the data of the dest file.
	}
	src.seek(filePos);						// Put it back like we found it.					
}



// ************************************************************
// ********************  docFileObj class  ********************
// ************************************************************


// A docFile object MUST have a path to exist. 	
docFileObj::docFileObj(char* filePath) {
ST	
	int numBytes;
	
	docFilePath		= NULL;									// Dynamic things start at NULL.
	editFilePath	= NULL;									// Dynamic things start at NULL.
	mode 				= fClosed;								// We are NOT editing at the moment.
	if (filePath) {											// If we have	a a file path. (Sanity, we'd better!)
		numBytes = strlen(filePath)+1;					// Calculate the number of bytes to store it.
		if (resizeBuff(numBytes,&docFilePath)) {		// If we get the RAM to store it..
			strcpy(docFilePath,filePath);					// Save the path.
		}
	}		
}


docFileObj::~docFileObj(void) {
ST
	closeDocFile();					// Close files and Recycle the editFile stuff.
	resizeBuff(0,&docFilePath);	// Recycle the docFilePath.
	
}


// Called by user to open a document.
//
// If opening for reading, its left open for reading. User MUST call close on it when
// finished.
// 
// If open for editing, this creates a clone of the doc file for editing and leaves that
// open to be used by the user for editing. And of course the user must call closed on it
// when finished. But, before that, saveDocFile() needs to be called for saving changes to
// the original file.
bool docFileObj::openDocFile(int openMode) {
ST
	File	tempFile;
	bool	success;
	
	success = false;																// Not a success yet..
	if (docFilePath) {															// If we have a non-NULL path..
		if (openMode==FILE_WRITE) {											// If they want to edit this file..
			if (mode==fOpenToEdit || mode==fEdited) {						// If already open for editing.
				ourFile.seek(ourFile.size());									// Opening a file to WRITE assumes you start at the end.
				return true;														// Actually nothing else to do, return success!
			}
			if (mode==fOpenToRead) {											// If currently open to read..
				closeDocFile();													// We close the file to open later.
			}
			tempFile = SD.open(docFilePath,FILE_READ);					// Have a go at opening the doc file path.
			if (tempFile) {														// If we were able to open the file..
				if (checkDoc(tempFile)) {										// If the file past the acid test..
					if (createEditPath()) {										// If we were able to create an edit path..
						ourFile = SD.open(editFilePath,FILE_WRITE);		// Have a go at opening the edit file path.	
						if (ourFile) {												// If we were able to open the edit file..
							ourFile.close();										// Close the file. We know we can open it.
							ourFile = TRUNCATE_FILE(editFilePath);
							fcpy(ourFile,tempFile);								// Copy the original to the new editing file.
							mode = fOpenToEdit;									// We are open for editing.
							success = true;										// Success!! The edit file is open and ready to use.
						}
					}
				}
				tempFile.close();													// Close the doc file.
			}
		} else if (openMode==FILE_READ) {									// If they just want to read this file..
			if (mode==fOpenToRead) {											// If we're already open to read..
				ourFile.seek(0);													// Opening to READ assumes you start at the beginning of the file.
				return true;														// We're all set, lets go.
			} else if (mode==fOpenToEdit||mode==fEdited) {				// We're in the middle of editing..
				closeDocFile();													// We try to close the file to open later.
				if (mode!=fClosed) {												// IF the file is NOT been closed..
					return false;													// They didn't close it, Lets bolt!
				}
			}
			ourFile = SD.open(docFilePath,FILE_READ);						// Have a go at opening the doc file path.
			if (ourFile) {															// If we were able to open the file..
				if (checkDoc(ourFile)) {										// If the file past the acid test..
					ourFile.seek(0);												// Point at first byte of the file.
					mode = fOpenToRead;											// We are open for reading.
					success = true;												// Success! The file is open and ready to read.
				}
			}
		}
	}
	return success;
}


// If we are editing a file, we will save the edited version either to a new file path. If
// one has been passed in, or if not, to the original doc file. If we are NOT editing AND
// we are passed in a new file path, we'll clone the doc file to this new path. If, in
// this case, no path is passed in? Its kind of.. "Shrug your shoulders and return false."
// The user is obviously just not getting it.
bool docFileObj::saveDocFile(char* newFilePath) {
ST	
	File	tempFile;
	bool	success;
	
	success = false;													// Not a success yet.
	if (mode==fEdited || mode==fOpenToEdit) {					// If we are using an editFile..
		if (newFilePath) {											// If we have a new path to save to..
			tempFile = SD.open(newFilePath,FILE_WRITE);		// Have a go at opening the new file path.
			if (tempFile) {											// If we got the new file..
				
				
				tempFile.close();											// Close the file. We know we can open it.
				tempFile = TRUNCATE_FILE(newFilePath);
				
				
				fcpy(tempFile,ourFile);								// Copy the edited file to the new file.
				tempFile.close();										// Close the new file.
				mode = fOpenToEdit;									// Clear the "dirty" flag, if any.
				success = true;										// Success!								
			} 
		} else {															// Else, no path was given. Store edit to doc file.
			tempFile = SD.open(docFilePath,FILE_WRITE);		// Have a go at opening the doc file path.
			if (tempFile) {											// If we were able to open the file..
				
				tempFile.close();											// Close the file. We know we can open it.
				tempFile = TRUNCATE_FILE(docFilePath);
				
				fcpy(tempFile,ourFile);								// Copy the editing file to the original file.
				tempFile.close();										// Close the original file.
				mode = fOpenToEdit;									// Clear the "dirty" flag, if any.
				success = true;										// And we are a success!
			}
		}
	} else if (mode==fOpenToRead && newFilePath) {			// If open to read AND we have a new path to save to..												// Set the pointer to byte 0.
		tempFile = SD.open(newFilePath,FILE_WRITE);			// Have a go at opening the new file path.
		if (tempFile) {												// If we got the new file..
			
			tempFile.close();											// Close the file. We know we can open it.
			tempFile = TRUNCATE_FILE(newFilePath);
			
			fcpy(tempFile,ourFile);									// Copy the original file to the new file.
			tempFile.close();											// Close the new file.
			success = true;											// And again, we are a success!
		}
	}
	return success;			
}


// This closes the docFile and if there is a temp file associated with it, that is
// deleted. There is no checking if this is ok to do, or if data will be lost. All that is
// up to the user of this object.
void docFileObj::closeDocFile(void) {
ST
	switch(mode) {
		case fClosed		: break;
		case fOpenToRead	:
			ourFile.close();
			mode = fClosed;
		break;
		case fOpenToEdit	:
		case fEdited :
			ourFile.close();
			SD.remove(editFilePath);
			resizeBuff(0,&editFilePath);
			mode = fClosed;
		break;
	}
}


bool docFileObj::changeDocFile(char* newPath) {
ST
	int	numBytes;
	
	if (mode==fClosed) {											// If the file is closed.
		if (newPath) {												// If we have	a new file path. (Sanity, we'd better!)
			numBytes = strlen(newPath)+1;						// Calculate the number of bytes to store it.
			if (resizeBuff(numBytes,&docFilePath)) {		// If we get the RAM to store it..
				strcpy(docFilePath,newPath);					// Save the path.
				db.trace("Change Success!",false);
				return true;										// Success!
			}
		}
	}
	return false;													// You fail!
}


bool docFileObj::fileEdited(void) { ST return mode==fEdited; }


byte docFileObj::peek(void) {

	if (mode != fClosed) {
		return ourFile.peek();
	} else {
		return 0;
	}
}

uint32_t docFileObj::position(void) {
	
	if (mode != fClosed) {
		return ourFile.position();
	} else {
		return 0;
	}
}

bool docFileObj::seek(uint32_t index) {
	
	if (mode != fClosed) {
		return ourFile.seek(index);
	} else {
		return false;
	}
}

uint32_t docFileObj::size(void) {
	
	if (mode != fClosed) {
		return ourFile.size(); 
	} else {
		return 0;
	}
}


int docFileObj::read(void) {

	if (mode != fClosed) {
		return ourFile.read();
	} else {
		return -1;
	}
}

uint16_t docFileObj::read(byte* buff,uint16_t numBytes) {

	if (mode != fClosed) {
		return ourFile.read(buff,numBytes);
	} else {
		return 0;
	}
}


size_t docFileObj::write(uint8_t aByte) {
ST	
	size_t	result;
	
	result = 0;
	if (mode==fOpenToEdit || mode==fEdited) {
		result = ourFile.write(&aByte,1);
		if (result>0) {
			mode = fEdited;
		}
	}
	return result;
}


size_t docFileObj::write(byte* buff,size_t numBytes) {
ST		
	size_t result;
	
	result = 0;
	if (mode==fOpenToEdit || mode==fEdited) {
		result = ourFile.write(buff,numBytes);
		if (result>0) {
			mode = fEdited;
		}
	}
	return result;
}	

		
// When the inherited opens up a document, this will be called to see if its parsable as
// the kind of file the class is looking for.
bool docFileObj::checkDoc(File inFile) { return false; }


// Find an unused temp file and assign it to the edit file path.
bool docFileObj::createEditPath(void) {
ST	
	char* filePath;
	bool	success;
	
	success = false;														// Start without success.
	filePath = numberedFilePath(TEMP_FOLDER,"efp",".tmp");	// Find an unused valid temp file path.
	if (filePath) {														// If we succeeded in this task..
		db.trace("filePath :",filePath,false);
		if (resizeBuff(strlen(filePath)+1,&editFilePath)) {	// If we can allocate the RAM to save this path.
			strcpy(editFilePath,filePath);							// Set our edit file path to this temp file path.
			success = true;												// Success!
		}																		//
		resizeBuff(0,&filePath);										// Recycle the RAM.
	}
	return success;															// If we got here, we've failed.
}

										
					
