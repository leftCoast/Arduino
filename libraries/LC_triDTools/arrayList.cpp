#include <arrayList.h>

#include <debug.h>


arrayList::arrayList(triDTriangle* inList,long numItems)
	: facetList() { 
	
	ourList		= inList;
	numFacets	= numItems;
	index			= 0;
}


arrayList::~arrayList(void) { }
	
	
bool arrayList::begin(void) { return true; }


// Pass back the total number facets we hold/offer.
long arrayList::getNumFacets(void) { return numFacets; }



// Pass back this facet given by index. Does NOT increment index
triDFacet arrayList::getTriDFacet(long inIndex) { 
	
	triDFacet	aFacet(&(ourList[inIndex]));
	
	return aFacet;
}


// Reset the internal index to the first facet. Used for..
void arrayList::resetIndex(void) { index = 0; }


// Passes back the next unread facet on our list. DOES increment index.
triDFacet arrayList::getNextTriDFacet(void) { if (index<numFacets) return getTriDFacet(index++); }


