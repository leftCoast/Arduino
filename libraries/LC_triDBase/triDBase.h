#ifndef triDBase_h
#define triDBase_h

#include <Arduino.h>



//****************************************************************************************
// triDPoint:
//
// Three values in the x,y,z coordinate space.
//****************************************************************************************


struct triDPoint {
	
	float	x;
	float	y;
	float	z;
};

// res = ptA + ptB
triDPoint triDPointAdd(triDPoint* ptA,triDPoint* ptB);

// res = ptA - ptB
triDPoint triDPointSub(triDPoint* ptA,triDPoint* ptB);

// res = ptA * ptB
triDPoint triDPointMult(triDPoint* ptA,triDPoint* ptB);

// res = ptA * scale
triDPoint triDPointScale(triDPoint* ptA,float scale);

// Show us what you got.
void printTriDPt(triDPoint* pt);



//****************************************************************************************
// triDTriangle:
//
// Three points in three space that define a triangle. Typically used as facets for 3D
// object.
//****************************************************************************************


struct triDTriangle {
	
	triDPoint corners[3];
};

// returns average of the x,y,z values for a center of the triangle.
triDPoint getCentPt(triDTriangle* triangle);

// Show us what you got.
void printTriDTriangle(triDTriangle* triangle);

//****************************************************************************************
// triDRotation:
//
// Holds pitch, yaw and roll angles.
//****************************************************************************************


struct triDRotation{
	
	float	xRad;	// Pitch
	float	yRad;	// Yaw
	float	zRad;	// Roll
};
			
#endif
