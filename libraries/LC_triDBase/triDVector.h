#ifndef triDVector_h
#define triDVector_h

#include <triDBase.h>


class triDVector {

	public:
				triDVector(void);
				triDVector(triDPoint* inPt);
				triDVector(double inX,double inY,double inZ);
	virtual	~triDVector(void);
				
				void			setVector(triDPoint* inPt);
				void			setVector(double inX,double inY,double inZ);
				void			setVector(triDPoint* stPt,triDPoint* endPt);
				triDPoint	getPoint(void);
				double			getX(void);
				double			getY(void);
				double			getZ(void);
				double			magnitude(void);
				void			scaleBy(double scaler);
				triDVector	crossProd(triDVector* aVect);
				void			normalize(void);
				bool			isNullVector(void);
				void			rotateVect(triDRotation* rotation);
				double			dotProduct(triDVector* inVect);
				double			angleBetween(triDVector* inVect);
				void			printVector(void);
				
	protected:
	
				double x;
				double	y;
				double	z;
};


// struct triDFacet {
// 	triDVector		normVect;
// 	triDTriangle	corners;
// };


#endif