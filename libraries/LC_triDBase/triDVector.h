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
				
				void			setVector(triDPoint* startPt,triDPoint* endPt);
				void			setVector(float startX,float startY,float startZ,float inEndX,float inEndY,float inEndZ);
				void			setFreeVector(float inEndX,float inEndY,float inEndZ);
				void			setFreeVector(triDPoint* inPoint);
				void			setStartpoint(float startX,float startY,float startZ);
				void			setStartpoint(triDPoint* inPoint);
				void			setEndpoint(float inEndX,float inEndY,float inEndZ);
				void			setEndpoint(triDPoint* inPt);
				triDPoint	getStartpoint(void);
				triDPoint	getEndpoint(void);
				triDPoint	getFreeVect(void);
				void			moveToOrigin(void);
				void			moveBack(void);
				bool			isNullVector(void);
				void			rotateVect(triDRotation* rotation);
				float			dotProduct(triDVector* inVect);
				float			magnitude(void);
				float			angleBetween(triDVector* inVect);
				void			printVector(void);
				
	protected:
	
				float stX;
				float	stY;
				float	stZ;
				float endX;
				float endY;
				float endZ;
				float saveX;
				float saveY;
				float saveZ;
};


struct triDFacet {
	triDVector		normVect;
	triDTriangle	corners;
};


#endif