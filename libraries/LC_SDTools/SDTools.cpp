#include <SDTools.h>
#include <resizeBuff.h>
#include <strTools.h>

//#include <debug.h>




//****************************************************************************************
// little indian integers:
// Many files have the 2 & 4 byte integers stored in reverse byte order than what we use
// here in Arduinoland. These routines swap 'em back.
//
//****************************************************************************************


// For reading two byte numbers.
uint16_t read16(File f) {

    uint16_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }


// For writing two byte numbers.
void write16(uint16_t val, File f) {
  
   f.write(((uint8_t *)&val)[0]);
	f.write(((uint8_t *)&val)[1]);
}


// For reading four byte numbers.
uint32_t read32(File f) {
  
    uint32_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }
  
  
// For writing four byte numbers.  
void write32(uint32_t val, File f) {
  
	f.write(((uint8_t *)&val)[0]);
	f.write(((uint8_t *)&val)[1]);
	f.write(((uint8_t *)&val)[2]);
	f.write(((uint8_t *)&val)[3]);
}  




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
				dirPath[numChars]='\0';						// Clip off the trailing '/'. No '/' no crash.
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
// will return NULL. IF THIS IS A SUCCESS, YOU MUST COPY THE RETURNED STRING. I won't last
// very long before being recycled.
char* numberedFilePath(const char* folderPath,const char* baseName,const char* extension) {

	File	tempFile;
	int	numBytes;
	int	maxNum;
	int	fileNum;
	char	numStr[8];
	bool	done;
	
	if (strlen(baseName)<8 												// If the params make sense..
		&& strlen(extension)<=4 										//
		&& folderPath[0]=='/') {										//
		if (createFolder(folderPath)) {								// If we can find/create the folder..
			maxNum = pow(10,8 - strlen(baseName));					// How many chars we got for a value?
			maxNum--;														// Actually, you get one too many.
			numBytes = strlen(folderPath) + 8 + 4 + 1;			// Path, max name, max extension, '\0'.
			if (resizeBuff(numBytes,&returnStr)) {					// If we can get the RAM..
				fileNum = 1;												// Starting at one.
				done = false;												// 'Cause we ain't.
				do {															// Do for each..
					itoa(fileNum++,numStr,7);							// Setup a number string.
					strcpy(returnStr,folderPath);						// Build up the test path.
					strcat(returnStr,baseName);							// Add the base name.
					strcat(returnStr,numStr);							// Add the number string.
					strcat(returnStr,extension);						// Add the extension.
					tempFile = SD.open(returnStr,FILE_READ);		//	Try to open this file for reading.
					if (tempFile) {										// If the file opened..
						tempFile.close();									// We just close it and move on.
					} else {													// Else, we have a possible candidate here.
						done = true;										// Either its the real deal or an error. In any case, we are done. 
						tempFile = SD.open(returnStr,FILE_WRITE);	// Try to create the file we couldn't open.
						if (tempFile) {									// If we were able to create the file..
							tempFile.close();								// Close it.
							return returnStr;								// And We'll call that a success!
						}														//
					}															//
				} while(!done && fileNum<maxNum);					// Loop while we are not done. (And have numbers to go.)
				resizeBuff(0,&returnStr);								// If we get here, its a failure so recycle the RAM.
			}																	//
		}																		//
	}																			//
	return returnStr;														// And this'll be returning a NULL.
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
	
	unsigned long	filePos;
	maxBuff	cpyBuff(src.size());
	unsigned long	numBytes;
	unsigned long	remaingBytes;
	
	dest.seek(dest.size());												// Point at end of the dest file.
	filePos = src.position();											// Lets save the file pos for miss user.
	src.seek(0);															// Point at first byte of the src file.
	remaingBytes = src.size();											// Get the remaining bytes to copy.
	for (int i=0;i<cpyBuff.numPasses;i++) {						// For every pass through..
		numBytes = min(cpyBuff.numBuffBytes,remaingBytes);		// Use buffer size or remaining bytes.
		src.read(cpyBuff.theBuff,numBytes);							// Fill the buffer.
		dest.write((char*)(cpyBuff.theBuff),numBytes);			// Write out the buffer.
		remaingBytes = remaingBytes - numBytes;					// Recalculate the remaining bytes.
	}																			// 
	src.seek(filePos);													// Put it back like we found it.					
}


// Pass in your extension and a file path. returns if the file extension match.
bool extensionMatch(const char* extension,const char* filePath) {

	char*	ext;
	char* path;
	char*	pathExt;
	int	index;
	bool	success;
	
	success	= false;										// Not a success yet.
	ext		= NULL;										// Local pointers start at NULL
	path		= NULL;										// Yeah, you too!
	heapStr(&ext,extension);							// Save off local copy of the extension.
	heapStr(&path,filePath);							// Save off local copy of the path.
	upCase(ext);											// Up case both;
	upCase(path);											// I said both.
	index = strlen(path)-1;								// Starting index at last character of path.
	while(index && path[index]!='.') index--;		// Run backwards looking for the dot.
	if (index) {											// If not zero, must be the dot.
		pathExt = &(path[index]);						// Grab the address of this character.
		success = !strcmp(ext,pathExt);				// Do a string compare. Save the result
	}															//
	freeStr(&ext);											// Free the local copy of the extension.
	freeStr(&path);										// Free the local copy of the path.
	return success;										// Return our results.
}
	
	