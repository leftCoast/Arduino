#ifndef baseImage_h
#define baseImage_h

#include <docFileObj.h>
#include <colorObj.h>
#include <SD.h>

// We have to start somewhere. This'll be better than zero.
#define DEF_WIDTH		240
#define DEF_HEIGHT	320

	
class baseImage :	public docFileObj {

	public:
					baseImage(void);
	virtual		~baseImage(void);

	virtual		bool		newDocFile(int inWidth=DEF_WIDTH,int inHeight=DEF_HEIGHT);
	
	virtual		colorObj	getPixel(int x,int y);
	virtual		bool		getRow(int row,RGBpack* RGBArray,int numPix,int xStart,bool fromTemp=false);
	virtual		bool		getRow(int row,RGBpack* RGBArray,int numPix,File* inFile);
	virtual		bool		setPixel(int x,int y,colorObj* aColor);
	virtual		bool		setRow(int row,RGBpack* RGBPackArray,int numPix,int xStart);
	
	protected:
	
	virtual		RGBpack	getPixel(int x,int y,File* imageFile);
	virtual		void		setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile);
					bool		checkXYLmits(int x, int y);
					bool		checkRowParams(int row,int numPix,int xStart);
					
					int		width;
					int		height;
};

#endif
