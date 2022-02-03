#ifndef stlObj_h
#define stlObj_h

#include <triDBase.h>

class stlObj :	public drawObj,
					public stlFile {

	public:
				stlObj(int inX,int inY,int inWidth,int inHeight,const char* stlPath);
				stlObj(rect* inRect,const char* stlPath);
	virtual	~stlObj(void);
	
				void		setAmbiantlight(float inIntencity,colorObj inColor);
				void		setlightSource(triDPoint* inLoc,triDDirect* inAngle,float inIntencity,colorObj inColor);
				void		setObjOrent(triDVector* inLocOffset,triDDirect* inRotAngle);
				void		buildindexList(void);
	virtual	void		drawSelf(void);
};


#endif