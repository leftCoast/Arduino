#include <triDVector.h>



// Constructor for no vector at all.
triDVector::triDVector(void) { setVector(0,0,0,0,0,0); }


// Constructor for a "free" vector. Meaning: Given an end point, the start point is set
// to 0,0,0.
triDVector::triDVector(float inEndX,float inEndY,float inEndZ) { setFreeVector(inEndX,inEndY,inEndZ); }


// Given a 6 point vector. Basically a vector between two points in three space.
triDVector::triDVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ) {

	setVector(startX,startY,startZ,inEndX,inEndY,inEndZ);
}
	

// Same as the 6 point vector but the points come in as addresses to triDPoint structs.
triDVector::triDVector(triDPoint* startPt,triDPoint* endPt) {
	
	setVector(startPt->x,startPt->y,startPt->z,endPt->x,endPt->y,endPt->z);
}


// The lonely destructor. Nothing's been allocated, so there is nothing for us to do here.		
triDVector::~triDVector(void) {  }


// Set all of 'em..
void triDVector::setVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ) {
	
	setStartpoint(startX,startY,startZ); 
	setEndpoint(inEndX,inEndY,inEndZ);
}

// FIll end the end point and clear out the start point.
void triDVector::setFreeVector(float inEndX,float inEndY,float inEndZ) {

	setVector(0,0,0,inEndX,inEndY,inEndZ);
}


// Only set the start point ones..
void triDVector::setStartpoint(float startX,float startY,float startZ) {

	stX	= startX;
	stY	= startY;
	stZ	= startZ;
}


// Only set the start point ones..
void triDVector::setStartpoint(triDPoint* inPt) {

	stX	= inPt->x;
	stY	= inPt->y;
	stZ	= inPt->z;
}


// Only set the endpoint ones..	
void triDVector::setEndpoint(float inEndX,float inEndY,float inEndZ) {

	endX	= inEndX;
	endY	= inEndY;
	endZ	= inEndZ;
}


// Only set the endpoint ones..
void triDVector::setEndpoint(triDPoint* inPt) {

	endX	= inPt->x;
	endY	= inPt->y;
	endZ	= inPt->z;
}


// Stuff the start point into a triDPoint struct and pass it back.
triDPoint  triDVector::getStartpoint(void) {

	triDPoint	aPoint;
	
	aPoint.x = stX;
	aPoint.y = stY;
	aPoint.y = stZ;
	return aPoint;
}


// Stuff the end point into a triDPoint struct and pass it back.	
triDPoint  triDVector::getEndpoint(void) {

	triDPoint	aPoint;
	
	aPoint.x = endX;
	aPoint.y = endY;
	aPoint.y = endZ;
	return aPoint;
}


// Get our representation as a free vector. Basically, three values calculated by what we
// would get as an endpoint if our start pont was offset to 0,0,0. Hence the return value
// as a triDPoint.
triDPoint triDVector::getFreeVect(void) {
	
	triDPoint	aPoint;
	
	aPoint.x = endX - stX;
	aPoint.y = endX - stY;
	aPoint.z = endX - stZ;
	return aPoint;
}


// You're standard dot product between us and an inputted vector.
float triDVector::dotProduct(triDVector* inVect) {

	triDPoint	ptA;
	triDPoint	ptB;
	float			xVal;
	float			yVal;
	float			zVal;
	float			result;
	
	result = 0;
	if (inVect) {
		ptA = getFreeVect();
		ptB = inVect->getFreeVect();
		xVal		= ptA.x * ptB.x;
		yVal		= ptA.y * ptB.y;
		zVal		= ptA.z * ptB.z;
		result	=  xVal+yVal+zVal;
	}
	return result;
}


// Magnitude This is calculated by finding out free vector valuers and using the formula : 
// Root (x^2 + y^2 + z^2)
float triDVector::magnitude(void) {
	
	triDPoint	aPt;
	float			sumSquares;
	float			result;
	
	
	aPt = getFreeVect();
	sumSquares = aPt.x * aPt.x + aPt.y * aPt.y + aPt.z * aPt.z;
	result = sqrt(sumSquares);
	return result;
}


// Angle between us (As a free vector) and a passed in vector. The calculation is given by
// the formula : A•B = ||A|| ||B|| cos Angle
float triDVector::angleBetween(triDVector* inVect) {
	
	float dotProd;
	float magA;
	float magB;
	
	dotProd = dotProduct(inVect);
	magA = magnitude();
	magB = inVect->magnitude();
	return acos(dotProd/(magA * magB));
}






