#ifndef facetList_h
#define facetList_h

#include <triDBase.h>
#include <triDVector.h>


//****************************************************************************************
// triDFacet:
//
// Holds the normal vector, for direction, and the the triD triangle that is the facet
// itself.
//****************************************************************************************


struct triDFacet {
	triDVector		normVect;
	triDTriangle	facet;
};



//****************************************************************************************
// facetList
//
// This is a purely virtual base class for a facet list. The idea is to make it not matter
// where facets come from or go to. But this is the interface that we can use to stuff
// them into a list or display them.
//****************************************************************************************

class facetList {

	public :
					facetList(void);
	virtual		~facetList(void);
	
	virtual		long			getNumFacets(void)=0;
	virtual		triDFacet*	getFacet(long index);
	virtual		void			resetIndex(void);
	virtual		triDFacet*	getNextFacet(void);
	virtual		void			setFacet(triDFacet* facetPtr,long index);
	virtual		void			insertFacetAfter(triDFacet* facetPtr,long index);
	virtual		void			insertFacetBefore(triDFacet* facetPtr,long index);
};

#endif
	