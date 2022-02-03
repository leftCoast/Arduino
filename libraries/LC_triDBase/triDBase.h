#ifndef triDBase_h
#define triDBase_h

#include <Arduino.h>

struct triDPoint {
	
	float	x;
	float	y;
	float	z;
};


struct triDDirection{
	
	float	xRad;
	float	yRad;
	float	zRad;
};

// Free vector : Has direction and magnatude only. Floats in space?
struct triDDirFreeVect {

	triDDirection	dir;
	float				mag;
};


// Rectangular Free vector : Free vector with start point 0,0,0. And an end point x,y,z
struct triDRectFreeVect {

	triDPoint	endPt;
};


// Bound vector : Vector from start point to end.
struct triDBoundVect {

	triDPoint startPt;
	triDPoint endPt;
};


struct triDTriangle {
	
	triDPoint verticies[3];
};



struct triDFacet {

	triDVect			normVect;
	triDTriangle	corners;
};


float dotProduct(triDRectFreeVect* ptA,triDRectFreeVect* ptB);

float	angleBetween(
			
#endif
