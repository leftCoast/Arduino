#include <triDVector.h>

#include <debug.h>


// Constructor for no vector at all.
triDVector::triDVector(void) { setVector(0,0,0); }


// Copy Constructor.
triDVector::triDVector(triDVector* aVector) { setVector(aVector); }


// Constructor given a triDPoint in space.
triDVector::triDVector(triDPoint* inPt) { setVector(inPt); }


// Constructor given three values.
triDVector::triDVector(double inX,double inY,double inZ) { setVector(inX,inY,inZ); }



// The lonely destructor. Nothing's been allocated, so there is nothing for us to do here.		
triDVector::~triDVector(void) {  }


// Copy another vector;
void triDVector::setVector(triDVector* aVector) {

	x = aVector->getX();
	y = aVector->getY();
	z = aVector->getZ();
}


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
	
	triDPoint res;
	
	res = triDPointSub(endPt,stPt);
	setVector(&res);
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
	
	twoDPoint	rotatePt;

	if (rotation->xRad) {											// Rotate around x
		rotatePt.x = y;
		rotatePt.y = z;
		rotate(&rotatePt,rotation->xRad);
		y = rotatePt.x;
		z = rotatePt.y;
	}
	if (rotation->yRad) {											// Rotate around y
		rotatePt.x = z;
		rotatePt.y = x;
		rotate(&rotatePt,rotation->yRad);
		z = rotatePt.x;
		x = rotatePt.y;
	}
	if (rotation->zRad) {											// Rotate around z
		rotatePt.x = x;
		rotatePt.y = y;
		rotate(&rotatePt,rotation->zRad);
		x = rotatePt.x;
		y = rotatePt.y;
	}																	//
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
	double magMult;
	double value;
	
	dotProd = dotProduct(inVect);
	magA = magnitude();
	magB = inVect->magnitude();
	magMult = magA * magB;
	if (magMult<.0001 && magMult>-0.0001) return 0;
	value = dotProd/magMult;
	if (value<-1) return PI;
	if (value>1) return 0;
	return acos(value);
}


void triDVector::printVector(void) {

	Serial.print("Vector x,y,z: ");
	Serial.print(x);Serial.print(", ");
	Serial.print(y);Serial.print(", ");
	Serial.print(z);
	Serial.print("   Magnitude: ");
	Serial.println(magnitude());
}


