#include <triDBase.h>
#include <SD.h>



float dotProduct(triDRectFreeVect* vctA,triDRectFreeVect* vctB) {

	float xVal;
	float yVal;
	float zVal;
	
	xVal = vctA.endpPt.x * vctB.endpPt.x;
	yVal = vctA.endpPt.y * vctB.endpPt.y;
	zVal = vctA.endpPt.z * vctB.endpPt.z;
	return xVal+yVal+zVal;
}


