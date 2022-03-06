#include <facetList.h>
#include <resizeBuff.h>



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
	
	for (byte i=0;i<3;i++) {
		facet.corners[i].x *= scaler;
		facet.corners[i].y *= scaler;
		facet.corners[i].z *= scaler;
	}
}


void triDFacet::offset(triDPoint* offsetPt) { offset(offsetPt->x,offsetPt->y,offsetPt->z); }


void triDFacet::offset(triDVector* offsetVect) { offset(offsetVect->getX(),offsetVect->getY(),offsetVect->getZ()); }


void triDFacet::offset(double x,double y,double z) {

	for (byte i=0;i<3;i++) {
		facet.corners[i].x += x;
		facet.corners[i].y += y;
		facet.corners[i].z += z;
	}
}


void triDFacet::rotate(triDRotation* rotation) {

	triDVector	aVect;
	triDVector	bVect;
	triDVector	nVect;
	
	for (byte i=0;i<3;i++) {
		aVect.setVector(facet.corners[i].x,facet.corners[i].y,facet.corners[i].z);
		aVect.rotateVect(rotation);
		facet.corners[i].x	= aVect.getX();
		facet.corners[i].y	= aVect.getY();
		facet.corners[i].z	= aVect.getZ();
	}
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
// where facets come from or go to. So this is the interface that we can use to stuff
// them into a list or display them.
//****************************************************************************************


facetList::facetList(void) { }


facetList::~facetList(void) { }


// Some lists may need to be opend (ex:SD card files) before use.
bool facetList::openList(void) { return true; }
 
 
// And those that do need to be opened for use need to be closed after use.
void facetList::closeList(void) {  }
	
	
// Pass back the total number facets we hold/offer.
long facetList::getNumFacets(void) { }


// Pass back a temporary pointer to the info on facet given by index.
triDFacet facetList::getTriDFacet(long index) { }


// Reset the internal index to the first facet. Used for..
void facetList::resetIndex(void) { }


// Passes back the next unread facet on our list.
triDFacet facetList::getNextTriDFacet(void) { }


// Take this inputted facet data and save it in the location given by index.
void facetList::setFacet(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it AFTER the location given by index. (Makes the list longer)
void facetList::insertFacetAfter(triDFacet* facetPtr,long index) { }


// Take this inputted facet data and insert it BEFORE the location given by index. (Makes the list longer)
void facetList::insertFacetBefore(triDFacet* facetPtr,long index) { }
