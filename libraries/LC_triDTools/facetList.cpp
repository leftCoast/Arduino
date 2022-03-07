#include <facetList.h>
#include <resizeBuff.h>



//****************************************************************************************
// triDFacet:
//
// The triDFacet class. This is your facet that is able to do trasformations. on itself.
//****************************************************************************************


// Default we make up a NULL facet.
triDFacet::triDFacet(void) {
	
	triDPoint aPt;
	
	aPt.x = 0;
	aPt.y = 0;
	aPt.z = 0;
	setFacet(&aPt,&aPt,&aPt);
}


// Create a facet using a triangle with ordered corners. The order gives the direction of
// the normal vector.
triDFacet::triDFacet(triDTriangle* orderdCorners) { setFacet(orderdCorners); }


// Create a facet with just three separate corners. And these need to be in order to
// successfully calculate the normal vector.
triDFacet::triDFacet(triDPoint* cornerA,triDPoint* cornerB,triDPoint* cornerC) { setFacet(cornerA,cornerB,cornerC); } 


// Dump a facet. And there's nothing we need to do here.
triDFacet::~triDFacet(void) {  }


// Set these values to the ones in this ordered triangle. Then calculate our normal
// vector.
void triDFacet::setFacet(triDTriangle* orderdCorners) {

	setFacet(&(orderdCorners->corners[0]),&(orderdCorners->corners[1]),&(orderdCorners->corners[2]));
}


// Set this facet to these three points in 3 space. Then calculate our normal vector.
void triDFacet::setFacet(triDPoint* cornerA,triDPoint* cornerB,triDPoint* cornerC) {
	
	facet.corners[0] = *cornerA;
	facet.corners[1] = *cornerB;
	facet.corners[2] = *cornerC;
	calcNormal();
}


// Hand back our facet triangle.
triDTriangle triDFacet::getFacet(void) { return facet; }


// Hand back our normal vector.
triDVector triDFacet::getNormVect(void) { return normVect; }


// Calculate and hand back the center point of our facet triangle.
triDPoint triDFacet::getCenterPt(void) { return getCentPt(&facet); }


// Scale the size of our facet. Will also tend to shift the size to match other scaled
// facets.
void triDFacet::scale(double scaler) {
	
	for (byte i=0;i<3;i++) {
		facet.corners[i].x *= scaler;
		facet.corners[i].y *= scaler;
		facet.corners[i].z *= scaler;
	}
}


// Move the facet by adding this offset point to it.
void triDFacet::offset(triDPoint* offsetPt) { offset(offsetPt->x,offsetPt->y,offsetPt->z); }


// Move the facet by adding this offset vector to it. 
void triDFacet::offset(triDVector* offsetVect) { offset(offsetVect->getX(),offsetVect->getY(),offsetVect->getZ()); }


// Do the actual calculation for the offsetting calls.
void triDFacet::offset(double x,double y,double z) {

	for (byte i=0;i<3;i++) {
		facet.corners[i].x += x;
		facet.corners[i].y += y;
		facet.corners[i].z += z;
	}
}


// Rotate this facet around the origin point using these radian values.
void triDFacet::rotate(triDRotation* rotation) {

	triDVector	aVect;
	
	for (byte i=0;i<3;i++) {
		aVect.setVector(facet.corners[i].x,facet.corners[i].y,facet.corners[i].z);
		aVect.rotateVect(rotation);
		facet.corners[i].x	= aVect.getX();
		facet.corners[i].y	= aVect.getY();
		facet.corners[i].z	= aVect.getZ();
	}
	calcNormal();
}


// Given that the corners of the facet are set and in order.. Calculate our normal vector.
void triDFacet::calcNormal(void) {

	triDVector	aVect;
	triDVector	bVect;
	triDVector	nVect;
	
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
