#include <stlList.h>
#include <SDTools.h>

stlList::stlList(const char* inPath)
	: stlFile(inPath),
	facetList() { index = 0; }


stlList::~stlList(void) { }
	
	
// We'll check to see if the filepath seems ok and that we can find and upen this file.
bool stlList::begin(void) { return checkFile(); }


// Some lists may need to be opend (ex:SD card files) before use.
bool stlList::openList(void) { return openForBatchRead(); }
 
 
// And those that do need to be opened for use need to be closed after use.
void stlList::closeList(void) { closeBatchRead(); }
	
	
// Pass back the total number facets we hold/offer.
long stlList::getNumFacets(void) { return (long)stlFile::getNumFacets(); }



//****************************************************************************************
//
// struct STLFacet {
// 
// 	float		normVect[3];	// 12 bytes
// 	float		vertex1[3];		// 12 bytes
// 	float		vertex2[3];		// 12 bytes
// 	float		vertex3[3];		// 12 bytes
// 	int16_t	extra;			// 2 bytes
// };
//
//
// struct facetPack {
// 	triDVector		normVect;
// 	triDTriangle	facet;
// };
//
//
// struct triDTriangle {
// 	triDPoint corners[3];
// };
//
//
//
// Takes an STLFacet, and sets those values into the passing facet pointer;
triDFacet stlList::readFileFacet(STLFacet* fileFacet) {
	
	triDFacet	res;

	res.normVect.setVector(fileFacet->normVect[0],fileFacet->normVect[1],fileFacet->normVect[2]);
	res.facet.corners[0].x = fileFacet->vertex1[0];
	res.facet.corners[0].y = fileFacet->vertex1[1];
	res.facet.corners[0].z = fileFacet->vertex1[2];
	res.facet.corners[1].x = fileFacet->vertex2[0];
	res.facet.corners[1].y = fileFacet->vertex2[1];
	res.facet.corners[1].z = fileFacet->vertex2[2];
	res.facet.corners[2].x = fileFacet->vertex3[0];
	res.facet.corners[2].y = fileFacet->vertex3[1];
	res.facet.corners[2].z = fileFacet->vertex3[2];
	
	return res;
}
//
//****************************************************************************************

// Pass back a temporary pointer to the info on facet given by index.
triDFacet stlList::getTriDFacet(long inIndex) {

	STLFacet	fileFacet;
	
	fileFacet = stlFile::getFacet(inIndex);
	index = inIndex + 1;
	return readFileFacet(&fileFacet);
}


// Reset the internal index to the first facet. Used for..
void stlList::resetIndex(void) { index = 0; }


// Passes back the next unread facet on our list.
triDFacet stlList::getNextTriDFacet(void) { return getTriDFacet(index); }

//****************************************************************************************
// For now, we're going to let this  editing stuff slide.


// Take this inputted facet data and save it in the location given by index.
void stlList::setFacet(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it AFTER the location given by index. (Makes
// the list longer)
void stlList::insertFacetAfter(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it BEFORE the location given by index. (Makes
// the list longer)
void stlList::insertFacetBefore(triDFacet* facetPtr,long index) { }

//****************************************************************************************
