#include <stlFile.h>
#include <strTools.h>
#include <SDTools.h>


#define HDR_BYTES			80
#define FACETNUM_BYTES	4
#define FACET_BYTES		50
#define FACET0_INDEX		84

  

stlFile::stlFile(const char* stlPath) {

	numFacets	= 0;
	isValid		= false;
	fileOpen		= false;
	path			= NULL;
	heapStr(&path,stlPath);
}


// Destructor, duh!	
stlFile::~stlFile(void) {

	if (fileOpen) {			// If the file's open..
		closeBatchRead();		// Close it.
	}								//
	freeStr(&path);			// Free the path string.
}
	
	
// Do our best to see if this is a valid .stl file or not.	
bool stlFile::checkFile(void) {
	
	if (isValid || fileOpen) return true;		// Duh! Already checked.
	if (extensionMatch(".stl",path)) {			// If the extension matches.
		fileObj = SD.open(path,FILE_READ);		// Have a go at opening the file.
		if (fileObj) {									// If we were able to open the file.
			fileObj.close();							// And we close the file.
			return true;								// For now we'll call it good.
		}													//
	}														//
	return false;										// If we got here, its false.
}


// If you want, we'll pass back the file name that is in the file. It MAY have "solid "
// preceding it. Some do, some don't.
char* stlFile::getSavedFileName(void) {
	
	unsigned long	savedLoc;
	bool				alreadyOpen;
	char				strBuff[20];
	char*				strEnd;
	
	freeStr(&returnStr);							// Set the returnStr to NULL.
	if (fileOpen) {								// If the file is already open..
		alreadyOpen = true;						// Save the state of our file.
		savedLoc = fileObj.position();		// Lets save the file pos for miss user.										
	} else {											// Else it wasn't open..
		alreadyOpen = false;						// Save the state of our file.
		openForBatchRead();						// Open our file.
	}													//
	if (fileOpen) {								// If we now have an open file..
		fileObj.seek(0);							// Make sure we're at the beginning.
		fileObj.read(strBuff,20);				// Grab 20 chars.
		strBuff[19] = '\0';						// Stuff a '\0' at the end.
		if (strBuff[0]!=' ') {					// If the first char is NOT a space..
			strEnd = strstr(strBuff,"   ");	// Look for a string of spaces.
			if (strEnd) {							// If we found the string of spaces..
				strEnd[0] = '\0';					// End our local copy at the start of the spaces.
				heapStr(&returnStr,strBuff);	// Stuff the results into returnStr;
			}											//
		}												//
	}													//
	if (alreadyOpen) {							// If the file was already open..
		fileObj.seek(savedLoc);					// We put the file pointer back where we found it.
	} else {											// Else we opened it.
		closeBatchRead();							// Then we'll close it.
	}													//
	return returnStr;								// Pass back returnStr.
}


int32_t stlFile::getNumFacets(void) {
	
	bool				alreadyOpen;
	unsigned long	savedLoc;
	
	if (numFacets) return numFacets;					// Zero means we've not looked.
	if (fileOpen) {										// If the file is already open..
		alreadyOpen = true;								// Save the state of our file.
		savedLoc = fileObj.position();				// Lets save the file pos for miss user.										
	} else {													// Else it wasn't open..
		alreadyOpen = false;								// Save the state of our file.
		openForBatchRead();								// Open our file.
	}															//
	if (fileOpen) {										// If we now have an open file..
		fileObj.seek(HDR_BYTES);						// Jump past header to where the number is stored.
		numFacets = read32(fileObj);					// Grab the value;
	}															//
	if (alreadyOpen) {									// If the file was already open..
		fileObj.seek(savedLoc);							// We put the file pointer back where we found it.
	} else {													// Else we opened it.
		closeBatchRead();									// Then we'll close it.
	}															//
	return numFacets;										// return our result.
}


// This may be called a zillion times. So we make sure there's a quick exit case.
bool stlFile::openForBatchRead(void) {

	if (fileOpen) return true;					// If the file is already open? Bail!
	if (checkFile()) {							// Else if we can pas a checkfile..
		fileObj = SD.open(path,FILE_READ);	// Have a shot at opening the file.
		if (fileObj) {								// If we were able to open the file..
			fileOpen = true;						// Save the file state.
		}
	}
	return fileOpen;
}
		

// CLse the file, it its open. Otherwise just exit.	
void stlFile::closeBatchRead(void) {

	if (fileOpen) {			// If the file is open..
		fileObj.close();		// The'll expect it to be opened at the beginning.
		fileOpen = false;		// Save the file state.
	}
}


// Make sure the file is open then grab a facet. If the facet is not there? Sadly this'll
// grab whatever and pass it back. Its up to YOU to make sure the index makes sense. Index
// goes from 0 to N-1 facets.
STLFacet stlFile::getFacet(long index) {

	STLFacet theFacet;
	
	if (openForBatchRead()) {									// If file is open, or we can open it..
		fileObj.seek(FACET0_INDEX+index*FACET_BYTES);	// Jump to the facet location.
		fileObj.read((void*)&theFacet,FACET_BYTES);		// Grab a facet.
	}																	//
	return theFacet;												// Return the result.
}


// printSTLFile This'll print out our .stl file to the monitor. I'll leave it here for
// doing debug kinda' things.
void stlFile::printSTLFile(void) {

	STLFacet	aFacet;
	
	Serial.println(getSavedFileName());
	Serial.println(getNumFacets());
	if (openForBatchRead()) {
		for(int32_t i=0;i<getNumFacets();i++) {
			aFacet = getFacet(i);
			printFacet(&aFacet,i);
			Serial.println("---------------------------------------");
		}
		closeBatchRead();
	}
}


void stlFile::printFacet(STLFacet* aFacet,int32_t index) {
	
	Serial.print("Facet index [");
	Serial.print(index);
	Serial.println("]");
	
	Serial.print("Normal points x,y,z   : ");
	Serial.print(aFacet->normVect[0]);Serial.print(", ");
	Serial.print(aFacet->normVect[1]);Serial.print(", ");
	Serial.print(aFacet->normVect[2]);Serial.println();
	
	Serial.print("Vertex 1 points x,y,z : ");
	Serial.print(aFacet->vertex1[0]);Serial.print(", ");
	Serial.print(aFacet->vertex1[1]);Serial.print(", ");
	Serial.print(aFacet->vertex1[2]);Serial.println();
	
	Serial.print("Vertex 2 points x,y,z : ");
	Serial.print(aFacet->vertex2[0]);Serial.print(", ");
	Serial.print(aFacet->vertex2[1]);Serial.print(", ");
	Serial.print(aFacet->vertex2[2]);Serial.println();
	
	Serial.print("Vertex 3 points x,y,z : ");
	Serial.print(aFacet->vertex3[0]);Serial.print(", ");
	Serial.print(aFacet->vertex3[1]);Serial.print(", ");
	Serial.print(aFacet->vertex3[2]);Serial.println();
	
	Serial.print("Extra word           : ");
	Serial.print(aFacet->extra);Serial.println();
}


