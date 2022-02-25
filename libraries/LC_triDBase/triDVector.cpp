#include <triDVector.h>

#include <debug.h>


// Constructor for no vector at all.
triDVector::triDVector(void) { setVector(0,0,0); }


// Constructor given a triDPoint in space.
triDVector::triDVector(triDPoint* inPt) { setVector(inPt); }


// Constructor given three values.
triDVector::triDVector(double inX,double inY,double inZ) { setVector(inX,inY,inZ); }



// The lonely destructor. Nothing's been allocated, so there is nothing for us to do here.		
triDVector::~triDVector(void) {  }


// Set to a point.
void triDVector::setVector(triDPoint* inPt) {
	
	x = inPt->x;
	y = inPt->y;
	z = inPt->z;
}


// Set all of 'em..
void triDVector::setVector(double inX,double inY,double inZ) {
	
	x = inX;
	y = inY;
	z = inZ;
}


// Set to, two points.
void triDVector::setVector(triDPoint* stPt,triDPoint* endPt) {
	
	x = endPt->x - stPt->x;
	y = endPt->y - stPt->y;
	z = endPt->z - stPt->z;
}


// Stuff the start point into a triDPoint struct and pass it back.
triDPoint  triDVector::getPoint(void) {

	triDPoint	aPoint;
	
	aPoint.x = x;
	aPoint.y = y;
	aPoint.z = z;
	return aPoint;
}

// Grab each value..
double  triDVector::getX(void) { return x; }
double  triDVector::getY(void) { return y; }
double  triDVector::getZ(void) { return z; }
	
		
// Magnitude This is calculated using the formula :  Root (x^2 + y^2 + z^2)
double triDVector::magnitude(void) {
	
	double			sumSquares;
	
	sumSquares = x*x + y*y + z*z;
	return sqrt(sumSquares);
}

void triDVector::scaleBy(double scaler) {

	x = x * scaler;
	y = y * scaler;
	z = z * scaler;
}


// Do the cross produxt between this and another vector. Return the results.
triDVector triDVector::crossProd(triDVector* aVect) {

	triDVector result;
	
	result.x = y * aVect->z - aVect->y * z;
	result.y = z * aVect->x - aVect->z * x;
	result.z = x * aVect->y - aVect->x * y;
	return result;
}


// Normalize this vector. IE foce it to have magnitude of 1.
void triDVector::normalize(void) {
	
	double mag;
 
	mag = magnitude();
	x = x/mag;
	y = y/mag;
	z = z/mag;
}


// Basically asking if all values are 0,0,0. NULL vector.
bool triDVector::isNullVector(void) { return x==0 && y==0 && z==0; }

	
// Do the pitch roll yaw transforms on each axis.
void  triDVector::rotateVect(triDRotation* rotation) {
	
	twoDPoint	rotatPt;
	
	if (rotation->xRad) {											// Rotate around x
		rotatPt.x = y;
		rotatPt.y = z;
		rotate(&rotatPt,rotation->xRad);
		y = rotatPt.x;
		z = rotatPt.y;
	}
	if (rotation->yRad) {											// Rotate around y
		rotatPt.x = z;
		rotatPt.y = x;
		rotate(&rotatPt,rotation->yRad);
		z = rotatPt.x;
		x = rotatPt.y;
	}
	if (rotation->zRad) {											// Rotate around z
		rotatPt.x = x;
		rotatPt.y = y;
		rotate(&rotatPt,rotation->zRad);
		x = rotatPt.x;
		y = rotatPt.y;
	}																		//
}


// You're standard dot product between us and an inputted vector.
double triDVector::dotProduct(triDVector* inVect) {

	double			xVal;
	double			yVal;
	double			zVal;
	double			result;
	
	result = 0;
	if (inVect) {
		xVal		= x * inVect->x;
		yVal		= y * inVect->y;
		zVal		= z * inVect->z;
		result	=  xVal+yVal+zVal;
	}
	return result;
}


// Angle between us and a passed in vector. The calculation is given by
// the formula : A•B = ||A|| ||B|| cos Angle
double triDVector::angleBetween(triDVector* inVect) {
	
	double dotProd;
	double magA;
	double magB;
	
	dotProd = dotProduct(inVect);
	magA = magnitude();
	magB = inVect->magnitude();
	return acos(dotProd/(magA * magB));
}


void triDVector::printVector(void) {

	Serial.print("Point x,y,z: ");
	Serial.print(x);Serial.print(", ");
	Serial.print(y);Serial.print(", ");
	Serial.print(z);
	Serial.print("   Magnitude: ");
	Serial.println(magnitude());
}


