#ifndef arrayList_h
#define arrayList_h

#include <facetList.h>



//****************************************************************************************
// arrayList
//
// This assumes that the list is an array of triDTriangle.  Pass in the array, and the
// number of triangles, then let it do its stuff.
//****************************************************************************************


class arrayList :	public facetList {

	public :
					arrayList(triDTriangle* inList,long numItems);
	virtual		~arrayList(void);
	
	virtual		bool			begin(void);
	virtual		long			getNumFacets(void);
	virtual		void			resetIndex(void);
	virtual		triDFacet	getTriDFacet(long inIndex);
	virtual		triDFacet	getNextTriDFacet(void);
	
					long				index;
					long				numFacets;
					triDTriangle*	ourList;
};


#endif