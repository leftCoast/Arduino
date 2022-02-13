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

void printTriDPt(triDPoint* pt);


//****************************************************************************************
// triDSpace:
//
// Six values in the x,y,z coordinate space that define a rectangular solid. Good for
// showing the limits in size for a 3D object.
//****************************************************************************************

// struct triDSpace {
// 
// 	float		xMin;
// 	float		xMax;
// 	float		yMin;
// 	float		yMax;
// 	float		zMin;
// 	float		zMax;
// };

//triDPoint centerPt(triDSpace* inSpace);

//****************************************************************************************
// triDTriangle:
//
// Three points in three space that define a triangle. Typically used as facets for 3D
// object.
//****************************************************************************************


struct triDTriangle {
	
	triDPoint corners[3];
};

//triDPoint centerPt(triDTriangle* inTriangle);

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
