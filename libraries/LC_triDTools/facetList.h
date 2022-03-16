#ifndef facetList_h
#define facetList_h

#include <triDBase.h>
#include <triDVector.h>



//****************************************************************************************
// triDFacet:
//
// The triDFacet class. This is your facet that is able to do trasformations. On itself.
//****************************************************************************************


class triDFacet {

	public :
					triDFacet(void);
					triDFacet(triDTriangle* orderdCorners);
					triDFacet(triDPoint* cornerA,triDPoint* cornerB,triDPoint* cornerC);
	virtual		~triDFacet(void);
	
					void				setFacet(triDTriangle* orderdCorners);
					void				setFacet(triDPoint* cornerA,triDPoint* cornerB,triDPoint* cornerC);
					triDTriangle	getFacet(void);
					triDVector		getNormVect(void);
					triDPoint		getCenterPt(void);
					void				scale(double scaler);
					void				offset(triDPoint* offsetPt);
					void				offset(triDVector* offsetVect);
					void				offset(double x,double y,double z);
					void				rotate(triDRotation* rotation);
					void				calcNormal(void);
					void				printFacet(void);
					
					triDTriangle	facet;
					triDVector		normVect;
};
	



//****************************************************************************************
// facetList
//
// This is a purely virtual base class for a facet list. The idea is to make it not matter
// where facets come from or go to. So this is the interface that we can use to stuff
// them into a list or display them.
//****************************************************************************************

class facetList {

	public :
					facetList(void);
	virtual		~facetList(void);
	
	virtual		bool			begin(void)=0;
	virtual		bool			openList(void);
	virtual		void			closeList(void);
	virtual		long			getNumFacets(void);
	virtual		void			resetIndex(void);
	virtual		triDFacet	getTriDFacet(long index);
	virtual		triDFacet	getNextTriDFacet(void);
	virtual		void			setFacet(triDFacet* facetPtr,long index);
	virtual		void			insertFacetAfter(triDFacet* facetPtr,long index);
	virtual		void			insertFacetBefore(triDFacet* facetPtr,long index);
};

#endif
	