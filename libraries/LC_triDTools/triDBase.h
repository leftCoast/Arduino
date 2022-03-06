#ifndef triDBase_h
#define triDBase_h

#include <Arduino.h>


//****************************************************************************************
// twoDPoint:
//
// Two values, the x,y coordinate space.
//
// Needed a better than int 2D point because math. So, this was setup as a tool for doing
// rotations. Etc.
//****************************************************************************************


struct twoDPoint {
	double	x;
	double	y;
};


void rotate(twoDPoint* ptA,double angle);


//****************************************************************************************
// triDPoint:
//
// Three values in the x,y,z coordinate space. Dey everywhere!
//****************************************************************************************


struct triDPoint {
	double	x;
	double	y;
	double	z;
};

// res = ptA + ptB
triDPoint triDPointAdd(triDPoint* ptA,triDPoint* ptB);

// res = ptA - ptB
triDPoint triDPointSub(triDPoint* ptA,triDPoint* ptB);

// res = ptA * ptB
triDPoint triDPointMult(triDPoint* ptA,triDPoint* ptB);

// res = ptA * scale
triDPoint triDPointScale(triDPoint* ptA,double scale);

// Show us what you got.
void printTriDPt(triDPoint* pt);



//****************************************************************************************
// triDTriangle:
//
// Three points in three space that define a triangle. Typically used as facets for 3D
// objects.
//****************************************************************************************


struct triDTriangle {
	triDPoint corners[3];
};

// Returns average of the x,y,z values for a center of the triangle.
triDPoint getCentPt(triDTriangle* triangle);


// Show us what you got.
void printTriDTriangle(triDTriangle* triangle);



//****************************************************************************************
// triDRotation:
//
// Holds pitch, yaw and roll angles.
//****************************************************************************************


struct triDRotation{
	double	xRad;	// Pitch
	double	yRad;	// Yaw
	double	zRad;	// Roll
};


			
#endif
