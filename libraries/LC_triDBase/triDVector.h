#ifndef triDVector_h
#define triDVector_h

#include <triDBase.h>


class triDVector {

	public:
				triDVector(void);
				triDVector(float endX,float endY,float endZ);
				triDVector(float startX,float startY,float startZ,float endX,float endY,float endZ);
				triDVector(triDPoint* startPt,triDPoint* endPt);
			
	virtual	~triDVector(void);
				
				void			setVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ);
				void			setFreeVector(float inEndX,float inEndY,float inEndZ);
				void			setStartpoint(float startX,float startY,float startZ);
				void			setStartpoint(triDPoint* inPoint);
				void			setEndpoint(float inEndX,float inEndY,float inEndZ);
				void			setEndpoint(triDPoint* inPt);
				triDPoint	getStartpoint(void);
				triDPoint	getEndpoint(void);
				triDPoint	getFreeVect(void);
				float			dotProduct(triDVector* inVect);
				float			magnitude(void);
				float			angleBetween(triDVector* inVect);
				
	protected:
	
				float stX;
				float	stY;
				float	stZ;
				float endX;
				float endY;
				float endZ;
};


struct triDFacet {
	triDVector		normVect;
	triDTriangle	corners;
};


#endif