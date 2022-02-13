#include <triDVector.h>

#include <debug.h>


// Constructor for no vector at all.
triDVector::triDVector(void) {

	setVector(0,0,0,0,0,0);
	saveX	= 0;
	saveY	= 0;
	saveZ	= 0;
}


// Constructor for a "free" vector. Meaning: Given an end point, the start point is set
// to 0,0,0.
triDVector::triDVector(float inEndX,float inEndY,float inEndZ) {

	setFreeVector(inEndX,inEndY,inEndZ);
	saveX	= 0;
	saveY	= 0;
	saveZ	= 0;
}


// Given a 6 point vector. Basically a vector between two points in three space.
triDVector::triDVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ) {

	setVector(startX,startY,startZ,inEndX,inEndY,inEndZ);
	saveX	= 0;
	saveY	= 0;
	saveZ	= 0;
}
	

// Same as the 6 point vector but the points come in as addresses to triDPoint structs.
triDVector::triDVector(triDPoint* startPt,triDPoint* endPt) {
	
	setVector(startPt,endPt);
	saveX	= 0;
	saveY	= 0;
	saveZ	= 0;
}


// The lonely destructor. Nothing's been allocated, so there is nothing for us to do here.		
triDVector::~triDVector(void) {  }


void triDVector::setVector(triDPoint* startPt,triDPoint* endPt) {
	
	setVector(startPt->x,startPt->y,startPt->z,endPt->x,endPt->y,endPt->z);
}


// Set all of 'em..
void triDVector::setVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ) {
	
	setStartpoint(startX,startY,startZ); 
	setEndpoint(inEndX,inEndY,inEndZ);
}

// Fill end the end point and clear out the start point.
void triDVector::setFreeVector(float inEndX,float inEndY,float inEndZ) {

	setVector(0,0,0,inEndX,inEndY,inEndZ);
}

void triDVector::setFreeVector(triDPoint* inPoint) {
	
	setFreeVector(inPoint->x,inPoint->y,inPoint->z);
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
	aPoint.z = stZ;
	return aPoint;
}


// Stuff the end point into a triDPoint struct and pass it back.	
triDPoint  triDVector::getEndpoint(void) {

	triDPoint	aPoint;
	
	aPoint.x = endX;
	aPoint.y = endY;
	aPoint.z = endZ;
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


// Actually, move this vector to the origin so that it becomes a fee vector. BUT we'll
// save off the offsets so that..
void triDVector::moveToOrigin(void) {

	saveX	= stX;
	saveY	= stY;
	saveZ	= stZ;
	endX	=  endX - stX;
	endY	=  endY - stY;
	endY	=  endY - stY;
}


// When we've moved to the origin to become a free vector. This will shift us back to
// where we were to begin with. So we can jump to the origin, do free vector math, then
// jump back to see the results in our "real vector" world.
void triDVector::moveBack(void) {

	stX 	= saveX;
	stY 	= saveY;
	stZ 	= saveZ;
	endX	= endX + saveX;
	endY	= endY + saveY;
	endZ	= endZ + saveZ;
	saveX	= 0;
	saveY	= 0;
	saveZ	= 0;
}
				

// Basically asking if all points are 0,0,0. NULL vector.
bool triDVector::isNullVector(void) {

	if (endX==0 && endY==0 && endZ==0) {
		if (stX==0 && stY==0 && stZ==0)	{
			return true;
		}
	}
	return false;
}

	
// If we are a free vector, see above. Do the pitch roll yaw transform on our end
// point. If NOT a free vector? Do it on BOTH points. IE we transverse sideways as if we
// were the end of a free vector in 3Space. THE INPUTTED ANGLE IS A DISPLACEMENT FROM THE
// CURRENT ANGLE.
void  triDVector::rotateVect(triDRotation* rotation) {
	
	float			mag;
	triDVector	shorty;
	triDPoint	shortyEndPt;
	
	mag = magnitude();												// Calc magnitude once.
	if (rotation->xRad) {											// If we rotate around x, y increases.
		endY = mag*sin((asin(endY/mag))+rotation->xRad);	// The formula.. To calculate new y endpoint.
	}																		//
	if (rotation->yRad) {											// If we rotate around y, z increases.
		endZ = mag*sin((asin(endZ/mag))+rotation->yRad);	// Again my magic formula.
	}																		//
	if (rotation->zRad) {											// Rotate around z, x increases.
		endX = mag*sin((asin(endX/mag))+rotation->zRad);	// Lord I hope this fromula works..
	}																		//
	if (stX!=0||stY!=0||stZ!=0) {									// If we have a start point, oh lord..
		shorty.setFreeVector(stX,stY,stZ);						// No problem! Create a free vector from our start point.
		shorty.rotateVect(rotation);								// And recursively rotate THAT vector using whata we did here.
		shortyEndPt = shorty.getEndpoint();						// Grab the resulting endpoint.
		setStartpoint(&shortyEndPt);								// And stuff that into our starting point.
	}
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
ST	
	triDPoint	aPt;
	float			sumSquares;
	float			result;
	
	Serial.print("Calc mag :  ");
	printVector();
	aPt = getFreeVect();
	sumSquares = aPt.x * aPt.x + aPt.y * aPt.y + aPt.z * aPt.z;
	result = sqrt(sumSquares);
	Serial.print("Mag result:  ");Serial.println(result);
	return result;
}


// Angle between us (As a free vector) and a passed in vector. The calculation is given by
// the formula : A•B = ||A|| ||B|| cos Angle
float triDVector::angleBetween(triDVector* inVect) {
ST	
	float dotProd;
	float magA;
	float magB;
	float	result;
	
	printVector();
	dotProd = dotProduct(inVect);
	magA = magnitude();
	Serial.print("magA: ");Serial.println(magA);
	magB = inVect->magnitude();
	Serial.print("    magB: ");Serial.println(magB);
	result = acos(dotProd/(magA * magB));
	Serial.print("    result: ");Serial.println(result);
	return result;
}


void triDVector::printVector(void) {

	Serial.print("stPt x,y,z: ");
	Serial.print(stX);Serial.print(", ");
	Serial.print(stY);Serial.print(", ");
	Serial.print(stZ);
	
	Serial.print("        endPt x,y,z: ");
	Serial.print(endX);Serial.print(", ");
	Serial.print(endY);Serial.print(", ");
	Serial.println(endZ);
}



