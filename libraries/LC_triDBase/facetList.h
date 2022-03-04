#ifndef facetList_h
#define facetList_h

#include <triDBase.h>
#include <triDVector.h>



//****************************************************************************************
// facetPack:
//
// Holds the normal vector, for direction, and the the triD triangle that is the facet
// itself.
//****************************************************************************************


struct facetPack {
	triDVector		normVect;
	triDTriangle	facet;
};



//****************************************************************************************
// triDFacet:
//
// The triDFacet class. This is your facet that is able to do trasformations. on itself.
//****************************************************************************************


class triDFacet {

	public :
					triDFacet(void);
					triDFacet(facetPack* facet);
	virtual		~triDFacet(void);
	
					void				setFacet(facetPack* facet);
					triDTriangle	getFacet(void);
					triDVector		getNormVect(void);
					facetPack		getFacetPack(void);
					triDPoint		getCenterPt(void);
					void				scale(double scaler);
					void				offset(triDPoint* offsetPt);
					void				offset(triDVector* offsetVect);
					void				offset(double x,double y,double z);
					void				rotate(triDRotation* rotation);
					
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
	
	virtual		bool			begin(void);
	virtual		long			getNumFacets(void)=0;
	virtual		triDFacet	getFacet(long index);
	virtual		void			resetIndex(void);
	virtual		triDFacet	getNextFacet(void);
	virtual		void			setFacet(triDFacet* facetPtr,long index);
	virtual		void			insertFacetAfter(triDFacet* facetPtr,long index);
	virtual		void			insertFacetBefore(triDFacet* facetPtr,long index);
};

#endif
	