#include <facetList.h>
#include <resizeBuff.h>



//****************************************************************************************
// facetPack:
//
// Holds the normal vector, for direction, and the the triD triangle that is the facet
// itself.
//****************************************************************************************


// struct facetPack {
// 	triDVector		normVect;
// 	triDTriangle	facet;
// };



//****************************************************************************************
// triDFacet:
//
// The triDFacet class. This is your facet that is able to do trasformations. on itself.
//****************************************************************************************

triDFacet::triDFacet(void) {
	
	facetPack aNULLFacet;
	aNULLFacet.normVect.setVector(1,1,1);
	aNULLFacet.facet.corners[0].x = 0;
	aNULLFacet.facet.corners[0].y = 0;
	aNULLFacet.facet.corners[0].z = 0;
	aNULLFacet.facet.corners[1].x = 0;
	aNULLFacet.facet.corners[1].y = 0;
	aNULLFacet.facet.corners[1].z = 0;
	aNULLFacet.facet.corners[2].x = 0;
	aNULLFacet.facet.corners[2].y = 0;
	aNULLFacet.facet.corners[2].z = 0;
	setFacet(&aNULLFacet);
}


triDFacet::triDFacet(facetPack* facet) { setFacet(facet); }


triDFacet::~triDFacet(void) {  }

	
void triDFacet::setFacet(facetPack* inFacet) {

	normVect.setVector(&(inFacet->normVect));
	facet = inFacet->facet;
}


triDTriangle triDFacet::getFacet(void) { return facet; }

triDVector triDFacet::getNormVect(void) { return normVect; }

facetPack triDFacet::getFacetPack(void) {
	
	facetPack aPack;
	
	aPack.normVect = normVect;
	aPack.facet = facet;
	return aPack;
}


triDPoint triDFacet::getCenterPt(void) { return getCentPt(&facet); }


void triDFacet::scale(double scaler) {
	
	facet.corners[0].x *= scaler;
	facet.corners[0].y *= scaler;
	facet.corners[0].z *= scaler;
	
	facet.corners[1].x *= scaler;
	facet.corners[1].y *= scaler;
	facet.corners[1].z *= scaler;
	
	facet.corners[2].x *= scaler;
	facet.corners[2].y *= scaler;
	facet.corners[2].z *= scaler;
}


void triDFacet::offset(triDPoint* offsetPt) { offset(offsetPt->x,offsetPt->y,offsetPt->z); }


void triDFacet::offset(triDVector* offsetVect) { offset(offsetVect->getX(),offsetVect->getY(),offsetVect->getZ()); }


void triDFacet::offset(double x,double y,double z) {

	facet.corners[0].x += x;
	facet.corners[0].y += y;
	facet.corners[0].z += z;
	
	facet.corners[1].x += x;
	facet.corners[1].y += y;
	facet.corners[1].z += z;
	
	facet.corners[2].x += x;
	facet.corners[2].y += y;
	facet.corners[2].z += z;

}


void triDFacet::rotate(triDRotation* rotation) {

	triDVector	aVect;
	triDVector	bVect;
	triDVector	nVect;
	
	aVect.setVector(facet.corners[0].x,facet.corners[0].y,facet.corners[0].z);
	aVect.rotateVect(rotation);
	facet.corners[0].x	= aVect.getX();
	facet.corners[0].y	= aVect.getY();
	facet.corners[0].z	= aVect.getZ();
	
	aVect.setVector(facet.corners[1].x,facet.corners[1].y,facet.corners[1].z);
	aVect.rotateVect(rotation);
	facet.corners[1].x	= aVect.getX();
	facet.corners[1].y	= aVect.getY();
	facet.corners[1].z	= aVect.getZ();
	
	aVect.setVector(facet.corners[2].x,facet.corners[2].y,facet.corners[2].z);
	aVect.rotateVect(rotation);
	facet.corners[2].x	= aVect.getX();
	facet.corners[2].y	= aVect.getY();
	facet.corners[2].z	= aVect.getZ();
	
	aVect.setVector(&(facet.corners[0]),&(facet.corners[1]));
	bVect.setVector(&(facet.corners[1]),&(facet.corners[2]));
	nVect = aVect.crossProd(&bVect);
	nVect.normalize();
	normVect.setVector(&nVect);
}


//****************************************************************************************
// facetList
//
// This is a purely virtual base class for a facet list. The idea is to make it not matter
// where facets come from or go to. But this is the interface that we can use to stuff
// them into a list or display them.
//****************************************************************************************


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
	

bool facetList::begin(void) { return true; }


// Pass back the total number facets we hold/offer.
long facetList::getNumFacets(void) { }


// Pass back a temporary pointer to the info on facet given by index.
triDFacet facetList::getFacet(long index) { }


// Reset the internal index to the first facet. Used for..
void facetList::resetIndex(void) { }


// Passes back the next unread facet on our list.
triDFacet facetList::getNextFacet(void) { }


// Take this inputted facet data and save it in the location given by index.
void facetList::setFacet(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it AFTER the location given by index. (Makes the list longer)
void facetList::insertFacetAfter(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it BEFORE the location given by index. (Makes the list longer)
void facetList::insertFacetBefore(triDFacet* facetPtr,long index) { }
