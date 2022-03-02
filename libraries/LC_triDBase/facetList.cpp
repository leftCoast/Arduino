#include <facetList.h>
#include <resizeBuff.h>


triDFacet*	passingFacet 	= NULL;
int			facetUsers		= 0;

facetList::facetList(void) {

	if (!facetUsers) {													// If no one is using the facet ptr..
		resizeBuff(sizeof(triDFacet),(byte**)&passingFacet);	// We'll allocate it for use.
	}																			//
	facetUsers++;															// And add ourselves to the user count.
}


facetList::~facetList(void) {

	facetUsers--;										// Drop ourselves from the facet ptr count.
	if (!facetUsers) {								// If no one is left using this pointer..
		resizeBuff(0,(byte**)&passingFacet);	// We'll recycle it.
	}
}
	

// Pass back the total number facets we hold/offer.
long facetList::getNumFacets(void) { }


// Pass back a temporary pointer to the info on facet given by index.
triDFacet* facetList::getFacet(long index) { }


// Reset the internal index to the first facet. Used for..
void facetList::resetIndex(void) { }


// Passes back the next unread facet on our list.
triDFacet* facetList::getNextFacet(void) { }


// Take this inputted facet data and save it in the location given by index.
void facetList::setFacet(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it AFTER the location given by index. (Makes the list longer)
void facetList::insertFacetAfter(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it BEFORE the location given by index. (Makes the list longer)
void facetList::insertFacetBefore(triDFacet* facetPtr,long index) { }
