#ifndef triDBase_h
#define triDBase_h

#include <Arduino.h>

struct triDPoint {
	
	float	x;
	float	y;
	float	z;
};


struct triDVector {

	float	xRad;
	float	yRad;
	float	zRad;
	float	length;
};


struct triDLine {

	triDPoint lineEnds[2];
};


struct triDTriangle {
	
	triDPoint verticies[3];
};



struct triDFacet {

	triDVector		normVect;
	triDTriangle	corners;
};


extern int8_t STLHeader[];
extern int8_t numFacets;

struct STLFacet {

	float	normVect[3];	// 12 bytes
	float	vertex1[3];		// 12 bytes
	float	vertex2[3];		// 12 bytes
	float	vertex3[3];		// 12 bytes
	int8_t	extra[2];	// 2 bytes
};

extern void printSTLFile(const char* filePath);

			
#endif
